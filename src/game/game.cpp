#include <ultra64.h>

#include "sm64.h"
#include "audio/external.h"
#include "buffers/framebuffers.h"
#include "buffers/zbuffer.h"
#include "engine/level_script.h"
#include "main.h"
#include "memory.h"
#include "save_file.h"
#include "seq_ids.h"
#include "sound_init.h"
#include "display.h"
#include "print.h"
#include "segment2.h"
#include "main_entry.h"
#include <prevent_bss_reordering.h>
#include "game.h"
#include "engine/level_script.h"
#include "controller/controllers.h"
#include "player/players.h"

OSMesgQueue D_80339CB8;
OSMesg D_80339CD0;
OSMesg D_80339CD4;
uintptr_t gPhysicalFrameBuffers[3];
uintptr_t gPhysicalZBuffer;
void* D_80339CF0;
void* D_80339CF4;
struct SPTask* gGfxSPTask;
Gfx* gDisplayListHead;
u8* gGfxPoolEnd;
struct GfxPool* gGfxPool;

struct MarioAnimation playerAnimation;
struct MarioAnimation gDemo;
UNUSED u8 filler80339D30[0x90];

void (*D_8032C6A0)(void)	    = NULL;
struct DemoInput* autoDemoPtr	    = NULL; // demo input sequence
u16 gDemoInputListID		    = 0;
struct DemoInput gRecordedDemoInput = {0}; // possibly removed in EU. TODO: Check

// this function records distinct inputs over a 255-frame interval to RAM locations and was likely
// used to record the demo sequences seen in the final game. This function is size.
static void record_demo(void)
{
	// record the player's button mask and current rawStickX and rawStickY.
	u8 buttonMask = ((sm64::player(0).controller().buttonDown & (sm64::hid::A_BUTTON | sm64::hid::B_BUTTON | sm64::hid::Z_TRIG | sm64::hid::START_BUTTON)) >> 8) |
			(sm64::player(0).controller().buttonDown & (sm64::hid::U_CBUTTONS | sm64::hid::D_CBUTTONS | sm64::hid::L_CBUTTONS | sm64::hid::R_CBUTTONS));
	s8 rawStickX = sm64::player(0).controller().rawStickX;
	s8 rawStickY = sm64::player(0).controller().rawStickY;

	// if the stick is in deadzone, set its value to 0 to
	// nullify the effects. We do not record deadzone inputs.
	if(rawStickX > -8 && rawStickX < 8)
	{
		rawStickX = 0;
	}

	if(rawStickY > -8 && rawStickY < 8)
	{
		rawStickY = 0;
	}

	// record the distinct input and timer so long as they
	// are unique. If the timer hits 0xFF, reset the timer
	// for the next demo input.
	if(gRecordedDemoInput.timer == 0xFF || buttonMask != gRecordedDemoInput.buttonMask || rawStickX != gRecordedDemoInput.rawStickX || rawStickY != gRecordedDemoInput.rawStickY)
	{
		gRecordedDemoInput.timer      = 0;
		gRecordedDemoInput.buttonMask = buttonMask;
		gRecordedDemoInput.rawStickX  = rawStickX;
		gRecordedDemoInput.rawStickY  = rawStickY;
	}
	gRecordedDemoInput.timer++;
}

// if a demo sequence exists, this will run the demo
// input list until it is complete. called every frame.
void run_demo_inputs(void)
{
	// eliminate the size bits.
	sm64::player(0).controller().state().button &= VALID_BUTTONS;

	/*
		Check if a demo inputs list
		exists and if so, run the
		active demo input list.
	*/
	if(autoDemoPtr != NULL)
	{
		/*
			clear player 2's inputs if they exist. Player 2's controller
			cannot be used to influence a demo. At some point, Nintendo
			may have planned for there to be a demo where 2 players moved
			around instead of just one, so clearing player 2's influence from
			the demo had to have been necessary to perform this. Co-op mode, perhaps?
		*/

		sm64::player(1).controller().state().stick_x = 0;
		sm64::player(1).controller().state().stick_y = 0;
		sm64::player(1).controller().state().button  = 0;

		// the timer variable being 0 at the current input means the demo is over.
		// set the button to the CONT_EXIT mask to end the demo.
		if(autoDemoPtr->timer == 0)
		{
			sm64::player(0).controller().state().stick_x = 0;
			sm64::player(0).controller().state().stick_y = 0;
			sm64::player(0).controller().state().button  = CONT_EXIT;
		}
		else
		{
			// backup the start button if it is pressed, since we don't want the
			// demo input to override the mask where start may have been pressed.
			u16 startPushed = sm64::player(0).controller().state().button & sm64::hid::START_BUTTON;

			// perform the demo inputs by assigning the current button mask and the stick inputs.
			sm64::player(0).controller().state().stick_x = autoDemoPtr->rawStickX;
			sm64::player(0).controller().state().stick_y = autoDemoPtr->rawStickY;

			/*
				to assign the demo input, the button information is stored in
				an 8-bit mask rather than a 16-bit mask. this is because only
				A, B, Z, Start, and the C-Buttons are used in a demo, as bits
				in that order. In order to assign the mask, we need to take the
				upper 4 bits (A, B, Z, and Start) and shift then left by 8 to
				match the correct input mask. We then add this to the masked
				lower 4 bits to get the correct button mask.
			*/
			sm64::player(0).controller().state().button = ((autoDemoPtr->buttonMask & 0xF0) << 8) + ((autoDemoPtr->buttonMask & 0xF));

			// if start was pushed, put it into the demo sequence being input to
			// end the demo.
			sm64::player(0).controller().state().button |= startPushed;

			// run the current demo input's timer down. if it hits 0, advance the
			// demo input list.

			if(frameCounter % FRAME_RATE_SCALER_INV == 0)
			{
				if(--autoDemoPtr->timer == 0)
				{
					autoDemoPtr++;
				}
			}
		}
	}
}

const void* marioAnim();

void setup_game_memory(void)
{
	UNUSED u8 pad[8];

	set_segment_base_addr(0, (void*)0x80000000);
	osCreateMesgQueue(&D_80339CB8, &D_80339CD4, 1);
	// TODO osCreateMesgQueue(&gGameVblankQueue, &D_80339CD0, 1);
	gPhysicalZBuffer	 = VIRTUAL_TO_PHYSICAL(gZBuffer);
	gPhysicalFrameBuffers[0] = VIRTUAL_TO_PHYSICAL(gFrameBuffer0);
	gPhysicalFrameBuffers[1] = VIRTUAL_TO_PHYSICAL(gFrameBuffer1);
	gPhysicalFrameBuffers[2] = VIRTUAL_TO_PHYSICAL(gFrameBuffer2);
	D_80339CF0		 = main_pool_alloc(0x4000, MEMORY_POOL_LEFT);
	set_segment_base_addr(17, (void*)D_80339CF0);

	func_80278A78(&playerAnimation, (void*)marioAnim(), (Animation*)D_80339CF0);
	D_80339CF4 = main_pool_alloc(2048, MEMORY_POOL_LEFT);
	set_segment_base_addr(24, (void*)D_80339CF4);
	func_80278A78(&gDemo, (void*)demoInputs(), (Animation*)D_80339CF4);
	load_segment(0x10, _entrySegmentRomStart, _entrySegmentRomEnd, MEMORY_POOL_LEFT);
	load_segment_decompress(2, _segment2_mio0SegmentRomStart, _segment2_mio0SegmentRomEnd);
}

static struct LevelCommand* levelCommandAddr;

// main game loop thread. runs forever as long as the game
// continues.
void game_init(void* arg)
{
	setup_game_memory();
	sm64::hid::controllers().scan();

	BuInitBackUp();

	// point levelCommandAddr to the entry point into the level script data.
	levelCommandAddr = (LevelCommand*)segmented_to_virtual(level_script_entry);

	Na_MusicStart(2, SEQUENCE_ARGS(0, SEQ_SOUND_PLAYER), 0);
	set_sound_mode(BuGetSoundMode());

	frameCounter++;
}

void game_loop_one_iteration(void)
{
	// if the reset timer is active, run the process to reset the game.
	if(gResetTimer)
	{
		func_80247D84();
	}

	audio_game_loop_tick();
	func_80247FAC();

	sm64::players().update();

	levelCommandAddr = level_script_execute(levelCommandAddr);
	display_and_vsync();
}
