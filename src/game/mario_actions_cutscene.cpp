#include <ultra64.h>

#include "sm64.h"
#include "gfx_dimensions.h"
#include "game.h"
#include "sound_init.h"
#include "level_update.h"
#include "interaction.h"
#include "mario.h"
#include "mario_step.h"
#include "mario_actions_moving.h"
#include "save_file.h"
#include "area.h"
#include "camera.h"
#include "object_helpers.h"
#include "object_helpers2.h"
#include "moving_texture.h"
#include "ingame_menu.h"
#include "audio/external.h"
#include "seq_ids.h"
#include "engine/math_util.h"
#include "engine/graph_node.h"
#include "engine/surface_collision.h"
#include "engine/behavior_script.h"
#include "behavior_data.h"
#include "object_list_processor.h"
#include "level_table.h"
#include "dialog_ids.h"
#include "game/options.h"
#include "player/players.h"
#include "game/motor.h"

// TODO: put this elsewhere
enum SaveOption
{
	SAVE_OPT_SAVE_AND_CONTINUE = 1,
	SAVE_OPT_SAVE_AND_QUIT,
	SAVE_OPT_CONTINUE_DONT_SAVE
};

static struct Object* sIntroWarpPipeObj;
static struct Object* sEndPeachObj;
static struct Object* sEndRightToadObj;
static struct Object* sEndLeftToadObj;
static struct Object* sEndJumboStarObj;
static UNUSED s32 sUnused;
static s16 sEndPeachAnimation;
static s16 sEndToadAnims[2];

static Vp sEndCutsceneVp		      = {{{640, 480, 511, 0}, {640, 480, 511, 0}}};
static struct CreditsEntry* sDispCreditsEntry = NULL;

// related to peach gfx?
static s8 D_8032CBE4	= 0;
static s8 D_8032CBE8	= 0;
static s8 D_8032CBEC[7] = {2, 3, 2, 1, 2, 3, 2};

static u8 sStarsNeededForDialog[6] = {1, 3, 8, 30, 50, 70};

/**
 * Data for the jumbo star cutscene. It specifies the flight path after triple
 * jumping. Each entry is one keyframe.
 * The first number is playback speed, 1000 is the maximum and means it lasts
 * 1 frame. 20 means that it lasts 1000/20 = 50 frames.
 * Speed 0 marks the last keyframe. Since the cubic spline looks 3 keyframes
 * ahead, there should be at least 2 more entries afterwards.
 * The last three numbers of each entry are x, y and z coordinates of points
 * that define the curve.
 */
static Vec4s sJumboStarKeyframes[27] = {
    {20, 0, 678, -2916},     {30, 0, 680, -3500},     {40, 1000, 700, -4000}, {50, 2500, 750, -3500},	{50, 3500, 800, -2000},	  {50, 4000, 850, 0},	{50, 3500, 900, 2000},	 {50, 2000, 950, 3500},	  {50, 0, 1000, 4000},
    {50, -2000, 1050, 3500}, {50, -3500, 1100, 2000}, {50, -4000, 1150, 0},   {50, -3500, 1200, -2000}, {50, -2000, 1250, -3500}, {50, 0, 1300, -4000}, {50, 2000, 1350, -3500}, {50, 3500, 1400, -2000}, {50, 4000, 1450, 0},
    {50, 3500, 1500, 2000},  {50, 2000, 1600, 3500},  {50, 0, 1700, 4000},    {50, -2000, 1800, 3500},	{50, -3500, 1900, 2000},  {30, -4000, 2000, 0}, {0, -3500, 2100, -2000}, {0, -2000, 2200, -3500}, {0, 0, 2300, -4000},
};

static s32 sSparkleGenTheta = 0;
static s32 sSparkleGenPhi   = 0;

// blink twice then have half-shut eyes (see end_peach_cutscene_kiss_from_peach)
static u8 sMarioBlinkOverride[20] = {
    MARIO_EYES_HALF_CLOSED, MARIO_EYES_HALF_CLOSED, MARIO_EYES_CLOSED,	    MARIO_EYES_CLOSED,	    MARIO_EYES_HALF_CLOSED, MARIO_EYES_HALF_CLOSED, MARIO_EYES_OPEN,	    MARIO_EYES_OPEN,	    MARIO_EYES_HALF_CLOSED, MARIO_EYES_HALF_CLOSED,
    MARIO_EYES_CLOSED,	    MARIO_EYES_CLOSED,	    MARIO_EYES_HALF_CLOSED, MARIO_EYES_HALF_CLOSED, MARIO_EYES_OPEN,	    MARIO_EYES_OPEN,	    MARIO_EYES_HALF_CLOSED, MARIO_EYES_HALF_CLOSED, MARIO_EYES_CLOSED,	    MARIO_EYES_CLOSED,
};

/**
 * get_credits_str_width: Calculate width of a Credits String
 * Loop over each character in a credits string and increment the length. If the
 * character is a space, increment by 4; otherwise increment by 7. Once the next
 * character is a null character (equal to 0), stop counting the length since
 * that's the end of the string.
 */
s32 get_credits_str_width(char* str)
{
	u32 c;
	s32 length = 0;

	while((c = *str++) != 0)
	{
		length += (c == ' ' ? 4 : 7);
	}

	return length;
}

#define CREDIT_TEXT_MARGIN_X ((s32)(GFX_DIMENSIONS_ASPECT_RATIO * 21))
#define CREDIT_TEXT_X_LEFT GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(CREDIT_TEXT_MARGIN_X)
#define CREDIT_TEXT_X_RIGHT GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(CREDIT_TEXT_MARGIN_X)

/**
 * DrawStaffRoll: Print the current displaying Credits Entry
 * Called in SnDrawScreen. This function checks if sDispCreditsEntry points to a
 * credits entry (see act_credits_cutscene), and if so, display it. The reason
 * this is called every frame in SnDrawScreen is because the credits need to
 * display on top of everything else.
 * To print a credits entry, we take the first character of the first string,
 * subtract the value of the 0 character, and use that as the number of lines to
 * print, excluding the title. Then, we print the title (after advancing the
 * pointer by 1) at X 28, Y either 28 or 172, depending on the print at bottom
 * flag. Finally, we print each line using the number of lines previously stored
 * as a counter, and increase the Y value by either the constant 16 (JP only) or
 * by the value of lineHeight.
 */
void DrawStaffRoll(void)
{
	char** currStrPtr;
	char* titleStr;
	s16 numLines;
	s16 strY;
	s16 lineHeight;

	if(sDispCreditsEntry != NULL)
	{
		currStrPtr = (char**)sDispCreditsEntry->staff;
		titleStr   = *currStrPtr++;
		numLines   = *titleStr++ - '0';

		strY	   = (sDispCreditsEntry->info & 0x20 ? 28 : 172) + (numLines == 1) * 16;
		lineHeight = 16;

		BeginEndingMessage();
		EndingMessage(CREDIT_TEXT_X_LEFT, strY, titleStr);

		switch(numLines)
		{
			case 4:
				EndingMessage(CREDIT_TEXT_X_LEFT, strY + 24, *currStrPtr++);
				numLines   = 2;
				lineHeight = 24;
				break;
			case 5:
				EndingMessage(CREDIT_TEXT_X_LEFT, strY + 16, *currStrPtr++);
				numLines = 3;
				break;
		}

		// smart dev here, thinking ahead for when the cosmic ray hits the rdram
		// chips 23 years later and nearly causes upwarp 2
		while(numLines-- > 0)
		{
			EndingMessage(CREDIT_TEXT_X_RIGHT - get_credits_str_width(*currStrPtr), strY, *currStrPtr);

			strY += lineHeight;

			currStrPtr++;
		}

		EndEndingMessage();
		sDispCreditsEntry = NULL;
	}
}

void BehEndPeachLoop(void)
{
	s_set_skelanimeNo(sEndPeachAnimation);
	if(s_check_animeend())
	{
		// anims: 0-3, 4, 5, 6-8, 9, 10, 11
		if(sEndPeachAnimation < 3 || sEndPeachAnimation == 6 || sEndPeachAnimation == 7)
		{
			sEndPeachAnimation++;
		}
	}
}

void BehEndToadLoop(void)
{
	s32 toadAnimIndex = (gCurrentObject->oPosX >= 0.0f);

	s_set_skelanimeNo(sEndToadAnims[toadAnimIndex]);
	if(s_check_animeend())
	{
		// 0-1, 2-3, 4, 5, 6, 7
		if(sEndToadAnims[toadAnimIndex] == 0 || sEndToadAnims[toadAnimIndex] == 2)
		{
			sEndToadAnims[toadAnimIndex]++;
		}
	}
}

static void generate_yellow_sparkles(s16 x, s16 y, s16 z, f32 radius)
{
	s16 offsetX = radius * coss(sSparkleGenTheta) * sins(sSparkleGenPhi);
	s16 offsetY = radius * sins(sSparkleGenTheta);
	s16 offsetZ = radius * coss(sSparkleGenTheta) * coss(sSparkleGenPhi);

	s_makeobj_absolute(gCurrentObject, 0, MODEL_NONE, sm64::bhv::bhvSparkleSpawn(), x + offsetX, y + offsetY, z + offsetZ, 0, 0, 0);

	//! copy paste error
	offsetX = offsetX * 4 / 3;
	offsetX = offsetY * 4 / 3;
	offsetX = offsetZ * 4 / 3;

	s_makeobj_absolute(gCurrentObject, 0, MODEL_NONE, sm64::bhv::bhvSparkleSpawn(), x - offsetX, y - offsetY, z - offsetZ, 0, 0, 0);

	sSparkleGenTheta += 0x3800 / FRAME_RATE_SCALER_INV;
	sSparkleGenPhi += 0x6000 / FRAME_RATE_SCALER_INV;
}

// not sure what this does, returns the height of the floor, but idk about the
// other stuff (animation related?)
static f32 func_8025BC14(struct Object* o)
{
	struct Surface* surf;
	Vec3s sp24;
	f32 sp20;
	f32 sp1C;
	f32 sp18;

	find_mario_anim_flags_and_translation(o, o->header.gfx.angle[1], sp24);

	sp20 = o->header.gfx.pos[0] + sp24[0];
	sp1C = o->header.gfx.pos[1] + 10.0f;
	sp18 = o->header.gfx.pos[2] + sp24[2];

	return mcBGGroundCheck(sp20, sp1C, sp18, &surf);
}

/**
 * mario_ready_to_speak: Determine if Mario is able to speak to a NPC
 * The following conditions must be met in order for Mario to be considered
 * ready to speak.
 * 1: Mario's action must be in the stationary or moving action groups, or if
 *    not, he must be in the "waiting for dialog" state.
 * 2: Mario mat not be riding a shell or be invulnerable.
 * 3: Mario must not be in first person mode.
 */
s32 mario_ready_to_speak(void)
{
	u32 actionGroup	   = marioWorks->status & ACT_GROUP_MASK;
	s32 isReadyToSpeak = FALSE;

	if((marioWorks->status == ACT_WAITING_FOR_DIALOG || actionGroup == ACT_GROUP_STATIONARY || actionGroup == ACT_GROUP_MOVING) && (!(marioWorks->status & (ACT_FLAG_RIDING_SHELL | ACT_FLAG_INVULNERABLE)) && marioWorks->status != ACT_FIRST_PERSON))
	{
		isReadyToSpeak = TRUE;
	}

	return isReadyToSpeak;
}

// (can) place mario in dialog?
// initiate dialog?
// return values:
// 0 = not in dialog
// 1 = starting dialog
// 2 = speaking
s32 CtrlPlayerDialog(s32 actionArg)
{
	s32 dialogState = 0;

	// in dialog
	if(marioWorks->status == ACT_READING_NPC_DIALOG)
	{
		if(marioWorks->actionState < 8)
		{
			dialogState = 1; // starting dialog
		}
		if(marioWorks->actionState == 8)
		{
			if(actionArg == 0)
			{
				marioWorks->actionState++; // exit dialog
			}
			else
			{
				dialogState = 2;
			}
		}
	}
	else if(actionArg != 0 && mario_ready_to_speak())
	{
		marioWorks->usedObj = gCurrentObject;
		marioWorks->ChangePlayerStatus(ACT_READING_NPC_DIALOG, actionArg);
		dialogState = 1; // starting dialog
	}

	return dialogState;
}

// Geo switch case function for controlling Peach's eye state.
s32 geo_switch_peach_eyes(s32 run, struct GraphNode* node, UNUSED s32 a2)
{
	struct GraphNodeSwitchCase* switchCase = (struct GraphNodeSwitchCase*)node;
	s16 timer;

	if(run == TRUE)
	{
		if(D_8032CBE4 == 0)
		{
			timer = (animationCounter + 0x20 * FRAME_RATE_SCALER_INV) >> 1 & 0x1F;

			if(timer < 7 * FRAME_RATE_SCALER_INV)
			{
				switchCase->selectedCase = D_8032CBE8 * 4 + D_8032CBEC[timer / FRAME_RATE_SCALER_INV];
			}
			else
			{
				switchCase->selectedCase = D_8032CBE8 * 4 + 1;
			}
		}
		else
		{
			switchCase->selectedCase = D_8032CBE8 * 4 + D_8032CBE4 - 1;
		}
	}

	return 0;
}

// size
static void Unknown80256FF8(u16* a0)
{
	if(GetMessageNo() == -1)
	{
		*a0 = 0;
	}
}

/**
 * get_star_collection_dialog: Determine what dialog should show when Mario
 * collects a star.
 * Determines if Mario has collected enough stars to get a dialog for it, and
 * if so, return the dialog ID. Otherwise, return 0
 */
s32 PlayerRecord::get_star_collection_dialog()
{
	s32 i;
	s32 dialogID = 0;
	s32 numStarsRequired;

	for(i = 0; i < 6; i++)
	{
		numStarsRequired = sStarsNeededForDialog[i];
		if(this->reserved2 < numStarsRequired && this->numStars >= numStarsRequired)
		{
			dialogID = i + DIALOG_141;
			break;
		}
	}

	this->reserved2 = this->numStars;
	return dialogID;
}

// save menu handler
void PlayerRecord::handle_save_menu()
{
	s32 dialogID;
	// wait for the menu to show up
	// mario_finished_animation() ? (not my file, not my problem)
	if(this->is_anim_past_end() && mesgLatch != 0)
	{
		// save and continue / save and quit
		if(mesgLatch == SAVE_OPT_SAVE_AND_CONTINUE || mesgLatch == SAVE_OPT_SAVE_AND_QUIT)
		{
			BuWriteStorage(activePlayerNo - 1);

			if(mesgLatch == SAVE_OPT_SAVE_AND_QUIT)
			{
				PL_ExitGameMode(-2, 0); // reset game
			}
		}

		// not quitting
		if(mesgLatch != SAVE_OPT_SAVE_AND_QUIT)
		{
			disable_time_stop();
			this->faceAngle[1] += 0x8000;
			// figure out what dialog to show, if we should
			dialogID = get_star_collection_dialog();
			if(dialogID != 0)
			{
				play_peachs_jingle();
				// look up for dialog
				this->ChangePlayerStatus(ACT_READING_AUTOMATIC_DIALOG, dialogID);
			}
			else
			{
				this->ChangePlayerStatus(ACT_IDLE, 0);
			}
		}
	}
}

/**
 * spawn_obj_at_mario_rel_yaw: Spawns object at Mario with relative yaw.
 * Spawns object with given behavior and model and copies over Mario's position
 * and yaw plus relative yaw.
 */
Object* PlayerRecord::spawn_obj_at_mario_rel_yaw(s32 model, const BehaviorScript* behavior, s16 relYaw)
{
	struct Object* o = s_makeobj_nowpos(this->marioObj, model, behavior);

	o->oFaceAngleYaw = this->faceAngle[1] + relYaw;
	o->oPosX	 = this->pos[0];
	o->oPosY	 = this->pos[1];
	o->oPosZ	 = this->pos[2];

	return o;
}

/**
 * cutscene_take_cap_off: Put Mario's cap on.
 * Clears "cap on head" flag, sets "cap in hand" flag, plays sound
 * SOUND_ACTION_UNKNOWN43D.
 */
void PlayerRecord::cutscene_take_cap_off()
{
	this->flags &= ~MARIO_CAP_ON_HEAD;
	this->flags |= MARIO_CAP_IN_HAND;
	AudStartSound(SOUND_ACTION_UNKNOWN43D, this->marioObj->header.gfx.cameraToObject);
}

/**
 * cutscene_put_cap_on: Put Mario's cap on.
 * Clears "cap in hand" flag, sets "cap on head" flag, plays sound
 * SOUND_ACTION_UNKNOWN43E.
 */
void PlayerRecord::cutscene_put_cap_on()
{
	this->flags &= ~MARIO_CAP_IN_HAND;
	this->flags |= MARIO_CAP_ON_HEAD;
	AudStartSound(SOUND_ACTION_UNKNOWN43E, this->marioObj->header.gfx.cameraToObject);
}

// actionargs:
// 1 : no head turn
// 2 : look up
// 3 : look down
// actionstate values:
// 0 - 7: looking toward npc
// 8: in dialog
// 9 - 22: looking away from npc
// 23: end
s32 PlayerRecord::act_reading_npc_dialog()
{
	s32 headTurnAmount = 0;
	s16 angleToNPC;

	if(this->actionArg == 2)
	{
		headTurnAmount = -1024;
	}
	if(this->actionArg == 3)
	{
		headTurnAmount = 384;
	}

	if(this->actionState < 8)
	{
		// turn to NPC
		angleToNPC	   = mario_angle_to_object(this, this->usedObj);
		this->faceAngle[1] = angleToNPC - approach_s32((angleToNPC - this->faceAngle[1]) << 16 >> 16, 0, 2048 / FRAME_RATE_SCALER_INV, 2048 / FRAME_RATE_SCALER_INV);
		// turn head to npc
		this->actionTimer += headTurnAmount;
		// set animation
		this->set_mario_animation(this->heldObj == NULL ? MARIO_ANIM_FIRST_PERSON : MARIO_ANIM_IDLE_WITH_LIGHT_OBJ);
	}
	else if(this->actionState >= 9 && this->actionState < 17)
	{
		// look back from facing NPC
		this->actionTimer -= headTurnAmount;
	}
	else if(this->actionState == 23)
	{
		if(this->flags & MARIO_CAP_IN_HAND)
		{
			this->ChangePlayerStatus(ACT_PUTTING_ON_CAP, 0);
		}
		else
		{
			this->ChangePlayerStatus(this->heldObj == NULL ? ACT_IDLE : ACT_HOLD_IDLE, 0);
		}
	}
	vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
	vec3s_set(this->marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);
	vec3s_set(this->marioBodyState->headAngle, this->actionTimer, 0, 0);

	if(this->actionState != 8)
	{
		this->actionState++;
	}

	return FALSE;
}

// puts mario in a state where he's waiting for (npc) dialog; doesn't do much
s32 PlayerRecord::act_waiting_for_dialog()
{
	this->set_mario_animation(this->heldObj == NULL ? MARIO_ANIM_FIRST_PERSON : MARIO_ANIM_IDLE_WITH_LIGHT_OBJ);
	vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
	vec3s_set(this->marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);
	return FALSE;
}

// makes mario disappear and triggers warp
s32 PlayerRecord::act_disappeared()
{
	this->set_mario_animation(MARIO_ANIM_A_POSE);
	stop_and_set_height_to_floor();
	this->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
	if(this->actionArg)
	{
		this->actionArg--;
		if((this->actionArg & 0xFFFF) == 0)
		{
			PL_StartFadeout(this, this->actionArg >> 16);
		}
	}
	return FALSE;
}

s32 PlayerRecord::act_reading_automatic_dialog()
{
	u32 actionArg;

	this->actionState++;
	if(this->actionState == 2)
	{
		enable_time_stop();
	}
	if(this->actionState < 9)
	{
		this->set_mario_animation(this->oldstatus == ACT_STAR_DANCE_WATER ? MARIO_ANIM_WATER_IDLE : MARIO_ANIM_FIRST_PERSON);
		// always look up for automatic dialogs
		this->actionTimer -= 1024;
	}
	else
	{
		// set mario dialog
		if(this->actionState == 9)
		{
			actionArg = this->actionArg;
			if(GET_HIGH_U16_OF_32(actionArg) == 0)
			{
				CallMessage(GET_LOW_U16_OF_32(actionArg));
			}
			else
			{
				CallMessageInt(GET_HIGH_U16_OF_32(actionArg), GET_LOW_U16_OF_32(actionArg));
			}
		}
		// wait until dialog is done
		else if(this->actionState == 10)
		{
			if(GetMessageNo() >= 0)
			{
				this->actionState--;
			}
		}
		// look back down
		else if(this->actionState < 19)
		{
			this->actionTimer += 1024;
		}
		// finished action
		else if(this->actionState == 25)
		{
			disable_time_stop();
			if(mesgCastle)
			{
				mesgCastle = FALSE;
				play_cutscene_music(SEQUENCE_ARGS(0, SEQ_LEVEL_INSIDE_CASTLE));
			}
			if(this->oldstatus == ACT_STAR_DANCE_WATER)
			{
				this->ChangePlayerStatus(ACT_WATER_IDLE, 0); // 100c star?
			}
			else
			{
				// make mario walk into door after star dialog
				this->ChangePlayerStatus(this->oldstatus == ACT_UNLOCKING_STAR_DOOR ? ACT_WALKING : ACT_IDLE, 0);
			}
		}
	}
	// apply head turn
	vec3s_set(this->marioBodyState->headAngle, this->actionTimer, 0, 0);
	return FALSE;
}

s32 PlayerRecord::act_reading_sign()
{
	struct Object* marioObj = this->marioObj;

	this->play_sound_if_no_flag(SOUND_ACTION_READ_SIGN, MARIO_ACTION_SOUND_PLAYED);

	switch(this->actionState)
	{
		// start dialog
		case 0:
			ViewingCam(1);
			enable_time_stop();
			// reading sign
			this->set_mario_animation(MARIO_ANIM_FIRST_PERSON);
			this->actionState = 1;
			// intentional fall through
		// turn toward sign
		case 1:
			this->faceAngle[1] += marioObj->oMarioPoleUnk108 / 11.0f * FRAME_RATE_SCALER;
			this->pos[0] += marioObj->oMarioReadingSignDPosX / 11.0f * FRAME_RATE_SCALER;
			this->pos[2] += marioObj->oMarioReadingSignDPosZ / 11.0f * FRAME_RATE_SCALER;
			// create the text box
			if(this->actionTimer++ == 10 * FRAME_RATE_SCALER_INV)
			{
				CallMessageFuda(this->usedObj->oBehParams2ndByte);
				this->actionState = 2;
			}
			break;
			// in dialog
		case 2:
			// dialog finished
			if(gCamera->cutscene == 0)
			{
				disable_time_stop();
				this->ChangePlayerStatus(ACT_IDLE, 0);
			}
			break;
	}

	vec3f_copy(marioObj->header.gfx.pos, this->pos);
	vec3s_set(marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);
	return FALSE;
}

// debug free move action
s32 PlayerRecord::act_debug_free_move()
{
	struct Surface* surf;
	f32 floorHeight;
	Vec3f pos;
	f32 speed;
	u32 action;

	// integer immediates, generates convert instructions for some reason
	speed = sm64::player(0).controller().buttonDown & sm64::hid::B_BUTTON ? 4 : 1;
	if(sm64::player(0).controller().buttonDown & sm64::hid::L_TRIG)
	{
		speed = 0.01f;
	}

	this->set_mario_animation(MARIO_ANIM_A_POSE);
	vec3f_copy(pos, this->pos);

	if(sm64::player(0).controller().buttonDown & sm64::hid::U_JPAD)
	{
		pos[1] += 16.0f * speed;
	}
	if(sm64::player(0).controller().buttonDown & sm64::hid::D_JPAD)
	{
		pos[1] -= 16.0f * speed;
	}

	if(this->intendedMag > 0)
	{
		pos[0] += 32.0f * FRAME_RATE_SCALER * speed * sins(this->intendedYaw);
		pos[2] += 32.0f * FRAME_RATE_SCALER * speed * coss(this->intendedYaw);
	}

	resolve_and_return_wall_collisions(pos, 60.0f, 50.0f);

	floorHeight = mcBGGroundCheck(pos[0], pos[1], pos[2], &surf);
	if(surf != NULL)
	{
		if(pos[1] < floorHeight)
		{
			pos[1] = floorHeight;
		}
		vec3f_copy(this->pos, pos);
	}

	this->faceAngle[1] = this->intendedYaw;
	vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
	vec3s_set(this->marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);

	if(sm64::player(0).controller().buttonPressed == sm64::hid::A_BUTTON)
	{
		if(this->pos[1] <= this->waterLevel - 100)
		{
			action = ACT_WATER_IDLE;
		}
		else
		{
			action = ACT_IDLE;
		}
		this->ChangePlayerStatus(action, 0);
	}

	return FALSE;
}

// star dance handler
void PlayerRecord::general_star_dance_handler(s32 isInWater)
{
	s32 dialogID;
	if(this->actionState == 0)
	{
		switch(++this->actionTimer)
		{
			case 1 * FRAME_RATE_SCALER_INV:
				s_makeobj_nowpos(this->marioObj, MODEL_STAR, sm64::bhv::bhvCelebrationStar());
				func_80248D48();
				if(this->actionArg & 1)
				{
					play_course_clear();
				}
				else
				{
					if(activeStageNo == LEVEL_BOWSER_1 || activeStageNo == LEVEL_BOWSER_2)
					{
						Na_MusicStart(1, SEQUENCE_ARGS(15, SEQ_EVENT_CUTSCENE_COLLECT_KEY), 0);
					}
					else
					{
						Na_MusicStart(1, SEQUENCE_ARGS(15, SEQ_EVENT_CUTSCENE_COLLECT_STAR), 0);
					}
				}
				break;

			case 42 * FRAME_RATE_SCALER_INV:
				AudStartSound(SOUND_MARIO_HERE_WE_GO, this->marioObj->header.gfx.cameraToObject);
				break;

			case 80 * FRAME_RATE_SCALER_INV:
				if((this->actionArg & 1) == 0)
				{
					PL_StartFadeout(this, PL_FADE_WINNING);
				}
				else
				{
					enable_time_stop();
					CallMessageYesNo(gLastCompletedStarNum == 7 ? DIALOG_013 : DIALOG_014);
					this->actionState = 1;
				}
				break;
		}
	}
	else if(this->actionState == 1 && gDialogResponse)
	{
		if(gDialogResponse == 1)
		{
			BuWriteStorage(activePlayerNo - 1);
		}
		this->actionState = 2;
	}
	else if(this->actionState == 2 && this->is_anim_at_end())
	{
		disable_time_stop();
		AudUnlockSound();
		dialogID = get_star_collection_dialog();
		if(dialogID != 0)
		{
			// look up for dialog
			this->ChangePlayerStatus(ACT_READING_AUTOMATIC_DIALOG, dialogID);
		}
		else
		{
			CmdRet cutscene_exit_painting_end(struct Camera* c);
			this->ChangePlayerStatus(isInWater ? ACT_WATER_IDLE : ACT_IDLE, 0);

			set_fov_function(CAM_FOV_DEFAULT);
			if (isInWater)
			{
				cutscene_exit_painting_end(area->camera);
			}
		}
	}
}

s32 PlayerRecord::act_star_dance()
{
	this->faceAngle[1] = this->area->camera->yaw;
	this->set_mario_animation(this->actionState == 2 ? MARIO_ANIM_RETURN_FROM_STAR_DANCE : MARIO_ANIM_STAR_DANCE);
	general_star_dance_handler(0);
	if(this->actionState != 2 && this->actionTimer >= 40 * FRAME_RATE_SCALER_INV)
	{
		this->marioBodyState->handState = MARIO_HAND_PEACE_SIGN;
	}
	stop_and_set_height_to_floor();
	return FALSE;
}

s32 PlayerRecord::act_star_dance_water()
{
	this->faceAngle[1] = this->area->camera->yaw;
	this->set_mario_animation(this->actionState == 2 ? MARIO_ANIM_RETURN_FROM_WATER_STAR_DANCE : MARIO_ANIM_WATER_STAR_DANCE);
	vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
	vec3s_set(this->marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);
	general_star_dance_handler(1);
	if(this->actionState != 2 && this->actionTimer >= 62 * FRAME_RATE_SCALER_INV)
	{
		this->marioBodyState->handState = MARIO_HAND_PEACE_SIGN;
	}
	return FALSE;
}

s32 PlayerRecord::act_fall_after_star_grab()
{
	if(this->pos[1] < this->waterLevel - 130)
	{
		AudStartSound(SOUND_ACTION_UNKNOWN430, this->marioObj->header.gfx.cameraToObject);
		this->particleFlags |= PARTICLE_WATER_SPLASH;
		return this->ChangePlayerStatus(ACT_STAR_DANCE_WATER, this->actionArg);
	}
	if(perform_air_step(1) == AIR_STEP_LANDED)
	{
		this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
		this->ChangePlayerStatus(this->actionArg & 1 ? ACT_STAR_DANCE_NO_EXIT : ACT_STAR_DANCE_EXIT, this->actionArg);
	}
	this->set_mario_animation(MARIO_ANIM_GENERAL_FALL);
	return FALSE;
}

// general death hander
s32 PlayerRecord::common_death_handler(s32 animation, s32 frameToDeathWarp)
{
	s32 animFrame = this->set_mario_animation(animation);
	if(animFrame == frameToDeathWarp)
	{
		PL_StartFadeout(this, PL_FADE_LOSING);
	}
	this->marioBodyState->eyeState = MARIO_EYES_DEAD;
	stop_and_set_height_to_floor();
	return animFrame;
}

s32 PlayerRecord::act_standing_death()
{
	if(this->input & INPUT_IN_POISON_GAS)
	{
		return this->ChangePlayerStatus(ACT_SUFFOCATION, 0);
	}

	this->play_sound_if_no_flag(SOUND_MARIO_DYING, MARIO_ACTION_SOUND_PLAYED);
	common_death_handler(MARIO_ANIM_DYING_FALL_OVER, 80);
	if(this->marioObj->header.gfx.unk38.frame() == 77)
	{
		this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
	}
	return FALSE;
}

s32 PlayerRecord::act_electrocution()
{
	this->play_sound_if_no_flag(SOUND_MARIO_DYING, MARIO_ACTION_SOUND_PLAYED);
	common_death_handler(MARIO_ANIM_ELECTROCUTION, 43);
	return FALSE;
}

s32 PlayerRecord::act_suffocation()
{
	this->play_sound_if_no_flag(SOUND_MARIO_DYING, MARIO_ACTION_SOUND_PLAYED);
	common_death_handler(MARIO_ANIM_SUFFOCATING, 86);
	return FALSE;
}

s32 PlayerRecord::act_death_on_back()
{
	this->play_sound_if_no_flag(SOUND_MARIO_DYING, MARIO_ACTION_SOUND_PLAYED);
	if(common_death_handler(MARIO_ANIM_DYING_ON_BACK, 54) == 40)
	{
		this->play_mario_heavy_landing_sound(SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
	}
	return FALSE;
}

s32 PlayerRecord::act_death_on_stomach()
{
	this->play_sound_if_no_flag(SOUND_MARIO_DYING, MARIO_ACTION_SOUND_PLAYED);
	if(common_death_handler(MARIO_ANIM_DYING_ON_STOMACH, 37) == 37)
	{
		this->play_mario_heavy_landing_sound(SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
	}
	return FALSE;
}

s32 PlayerRecord::act_quicksand_death()
{
	if(this->actionState == 0)
	{
		this->set_mario_animation(MARIO_ANIM_DYING_IN_QUICKSAND);
		this->set_anim_to_frame(60);
		this->actionState = 1;
	}
	if(this->actionState == 1)
	{
		if(this->sinking >= 100.0f)
		{
			this->play_sound_if_no_flag(SOUND_MARIO_WAAAOOOW, MARIO_ACTION_SOUND_PLAYED);
		}
		if((this->sinking += 5.0f * FRAME_RATE_SCALER) >= 180.0f)
		{
			PL_StartFadeout(this, PL_FADE_LOSING);
			this->actionState = 2;
		}
	}
	stationary_ground_step();
	AudStartSound(SOUND_MOVING_QUICKSAND_DEATH, this->marioObj->header.gfx.cameraToObject);
	return FALSE;
}

s32 PlayerRecord::act_eaten_by_bubba()
{
	this->play_sound_if_no_flag(SOUND_MARIO_DYING, MARIO_ACTION_SOUND_PLAYED);
	this->set_mario_animation(MARIO_ANIM_A_POSE);
	this->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
	this->health = 0xFF;
	if(this->actionTimer++ == 60 * FRAME_RATE_SCALER_INV)
	{
		PL_StartFadeout(this, PL_FADE_LOSING);
	}
	return FALSE;
}

// set animation and forwardVel; when perform_air_step returns AIR_STEP_LANDED,
// set the new action
s32 PlayerRecord::launch_mario_until_land(s32 endAction, s32 animation, f32 forwardVel)
{
	s32 airStepLanded;
	this->mario_set_forward_vel(forwardVel);
	this->set_mario_animation(animation);
	airStepLanded = (perform_air_step(0) == AIR_STEP_LANDED);
	if(airStepLanded)
	{
		this->ChangePlayerStatus(endAction, 0);
	}
	return airStepLanded;
}

s32 PlayerRecord::act_unlocking_key_door()
{
	this->faceAngle[1] = this->usedObj->oMoveAngleYaw;

	this->pos[0] = this->usedObj->oPosX + coss(this->faceAngle[1]) * 75.0f;
	this->pos[2] = this->usedObj->oPosZ + sins(this->faceAngle[1]) * 75.0f;

	if(this->actionArg & 2)
	{
		this->faceAngle[1] += 0x8000;
	}

	if(this->actionTimer == 0)
	{
		spawn_obj_at_mario_rel_yaw(MODEL_BOWSER_KEY_CUTSCENE, sm64::bhv::bhvBowserKeyUnlockDoor(), 0);
		this->set_mario_animation(MARIO_ANIM_UNLOCK_DOOR);
	}

	switch(this->marioObj->header.gfx.unk38.frame())
	{
		case 79:
			AudStartSound(SOUND_GENERAL_DOOR_INSERT_KEY, this->marioObj->header.gfx.cameraToObject);
			break;
		case 111:
			AudStartSound(SOUND_GENERAL_DOOR_TURN_KEY, this->marioObj->header.gfx.cameraToObject);
			break;
	}

	this->update_mario_pos_for_anim();
	stop_and_set_height_to_floor();

	if(this->is_anim_at_end())
	{
		if(this->usedObj->oBehParams >> 24 == 1)
		{
			BuSetItemFlag(SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR);
			BuClrItemFlag(SAVE_FLAG_HAVE_KEY_2);
		}
		else
		{
			BuSetItemFlag(SAVE_FLAG_UNLOCKED_BASEMENT_DOOR);
			BuClrItemFlag(SAVE_FLAG_HAVE_KEY_1);
		}
		this->ChangePlayerStatus(ACT_WALKING, 0);
	}

	this->actionTimer++;
	return FALSE;
}

s32 PlayerRecord::act_unlocking_star_door()
{
	switch(this->actionState)
	{
		case 0:
			this->faceAngle[1] = this->usedObj->oMoveAngleYaw;
			if(this->actionArg & 2)
			{
				this->faceAngle[1] += 0x8000;
			}
			this->marioObj->oMarioReadingSignDPosX = this->pos[0];
			this->marioObj->oMarioReadingSignDPosZ = this->pos[2];
			this->set_mario_animation(MARIO_ANIM_SUMMON_STAR);
			this->actionState++;
			break;
		case 1:
			if(this->is_anim_at_end())
			{
				s_makeobj_nowpos(this->marioObj, MODEL_STAR, sm64::bhv::bhvUnlockDoorStar());
				this->actionState++;
			}
			break;
		case 2:
			if(this->actionTimer++ == 70 * FRAME_RATE_SCALER_INV)
			{
				this->set_mario_animation(MARIO_ANIM_RETURN_STAR_APPROACH_DOOR);
				this->actionState++;
			}
			break;
		case 3:
			if(this->is_anim_at_end())
			{
				BuSetItemFlag(get_door_save_file_flag(this->usedObj));
				this->ChangePlayerStatus(ACT_READING_AUTOMATIC_DIALOG, DIALOG_038);
			}
			break;
	}

	this->pos[0] = this->marioObj->oMarioReadingSignDPosX;
	this->pos[2] = this->marioObj->oMarioReadingSignDPosZ;

	this->update_mario_pos_for_anim();
	stop_and_set_height_to_floor();

	return FALSE;
}

// not sure what kind of door this is
s32 PlayerRecord::act_entering_star_door()
{
	f32 targetDX;
	f32 targetDZ;
	s16 targetAngle;

	if(this->actionTimer++ == 0)
	{
		this->interactObj->oInteractStatus = 0x00010000;

		// ~30 degrees / 1/12 rot
		targetAngle = this->usedObj->oMoveAngleYaw + 0x1555;
		if(this->actionArg & 2)
		{
			targetAngle += 0x5556 / FRAME_RATE_SCALER_INV; // ~120 degrees / 1/3 rot (total 150d / 5/12)
		}

		// targetDX and targetDZ are the offsets to add to mario's position to
		// have mario stand 150 units in front of the door

		targetDX = this->usedObj->oPosX + 150.0f * sins(targetAngle) - this->pos[0];
		targetDZ = this->usedObj->oPosZ + 150.0f * coss(targetAngle) - this->pos[2];

		this->marioObj->oMarioReadingSignDPosX = targetDX / 20.0f;
		this->marioObj->oMarioReadingSignDPosZ = targetDZ / 20.0f;

		this->faceAngle[1] = atan2s(targetDZ, targetDX);
	}

	// set mario's animation
	if(this->actionTimer < 15 * FRAME_RATE_SCALER_INV)
	{
		this->set_mario_animation(MARIO_ANIM_FIRST_PERSON);
	}

	// go through door? for 20 frames
	else if(this->actionTimer < 35 * FRAME_RATE_SCALER_INV)
	{
		this->pos[0] += this->marioObj->oMarioReadingSignDPosX * FRAME_RATE_SCALER;
		this->pos[2] += this->marioObj->oMarioReadingSignDPosZ * FRAME_RATE_SCALER;

		this->set_mario_anim_with_accel(MARIO_ANIM_WALKING, 0x00028000);
	}

	else
	{
		this->faceAngle[1] = this->usedObj->oMoveAngleYaw;

		if(this->actionArg & 2)
		{
			this->faceAngle[1] += 0x8000;
		}

		this->pos[0] += 12.0f * FRAME_RATE_SCALER * sins(this->faceAngle[1]);
		this->pos[2] += 12.0f * FRAME_RATE_SCALER * coss(this->faceAngle[1]);

		this->set_mario_anim_with_accel(MARIO_ANIM_WALKING, 0x00028000);
	}

	stop_and_set_height_to_floor();

	if(this->actionTimer == 48 * FRAME_RATE_SCALER_INV)
	{
		this->ChangePlayerStatus(ACT_IDLE, 0);
	}

	return FALSE;
}

s32 PlayerRecord::act_going_through_door()
{
	if(this->actionTimer == 0)
	{
		if(this->actionArg & 1)
		{
			this->interactObj->oInteractStatus = 0x00010000;
			this->set_mario_animation(MARIO_ANIM_PULL_DOOR_WALK_IN);
		}
		else
		{
			this->interactObj->oInteractStatus = 0x00020000;
			this->set_mario_animation(MARIO_ANIM_PUSH_DOOR_WALK_IN);
		}
	}
	this->faceAngle[1] = this->usedObj->oMoveAngleYaw;
	this->pos[0]	   = this->usedObj->oPosX;
	this->pos[2]	   = this->usedObj->oPosZ;

	this->update_mario_pos_for_anim();
	stop_and_set_height_to_floor();

	if(this->actionArg & 4)
	{
		if(this->actionTimer == 16 * FRAME_RATE_SCALER_INV)
		{
			PL_StartFadeout(this, PL_FADE_TRIPDOOR);
		}
	}
	else
	{
		if(this->is_anim_at_end())
		{
			if(this->actionArg & 2)
			{
				this->faceAngle[1] += 0x8000;
			}
			this->ChangePlayerStatus(ACT_IDLE, 0);
		}
	}

	this->actionTimer++;
	return FALSE;
}

s32 PlayerRecord::act_warp_door_spawn()
{
	if(this->actionState == 0)
	{
		this->actionState = 1;
		if(this->actionArg & 1)
		{
			this->usedObj->oInteractStatus = 0x00040000;
		}
		else
		{
			this->usedObj->oInteractStatus = 0x00080000;
		}
	}
	else if(this->usedObj->oAction == 0)
	{
		if(mesgCastle == TRUE && activeStageNo == LEVEL_CASTLE)
		{
			this->ChangePlayerStatus(ACT_READING_AUTOMATIC_DIALOG, DIALOG_021);
		}
		else
		{
			this->ChangePlayerStatus(ACT_IDLE, 0);
		}
	}
	this->set_mario_animation(MARIO_ANIM_FIRST_PERSON);
	stop_and_set_height_to_floor();
	return FALSE;
}

s32 PlayerRecord::act_emerge_from_pipe()
{
	struct Object* marioObj = this->marioObj;

	if(this->actionTimer++ < 11 * FRAME_RATE_SCALER_INV)
	{
		marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
		return FALSE;
	}

	marioObj->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;

	this->play_sound_if_no_flag(SOUND_MARIO_YAHOO, MARIO_MARIO_SOUND_PLAYED);

	if(activeStageNo == 13)
	{
		if(activeSceneNo == 2)
		{
			this->play_sound_if_no_flag(SOUND_MENU_EXIT_PIPE, MARIO_ACTION_SOUND_PLAYED);
		}
		else
		{
			this->play_sound_if_no_flag(SOUND_MENU_ENTER_PIPE, MARIO_ACTION_SOUND_PLAYED);
		}
	}

	if(launch_mario_until_land(ACT_JUMP_LAND_STOP, MARIO_ANIM_SINGLE_JUMP, 8.0f))
	{
		this->mario_set_forward_vel(0.0f);
		this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
	}
	return FALSE;
}

s32 PlayerRecord::act_spawn_spin_airborne()
{
	// entered water, exit action
	if(this->pos[1] < this->waterLevel - 100)
	{
		PL_DisplayMessage(0);
		return this->set_water_plunge_action();
	}

	// updates all velocity variables based on this->forwardVel
	this->mario_set_forward_vel(this->forwardVel);

	// landed on floor, play spawn land animation
	if(perform_air_step(0.0) == AIR_STEP_LANDED)
	{
		this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
		this->ChangePlayerStatus(ACT_SPAWN_SPIN_LANDING, 0);
	}

	// is 300 units above floor, spin and play woosh sounds
	if(this->actionState == 0 && this->pos[1] - this->floorHeight > 300.0f)
	{
		if(this->set_mario_animation(MARIO_ANIM_FORWARD_SPINNING) == 0)
		{ // first anim frame
			AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
		}
	}

	// under 300 units above floor, enter freefall animation
	else
	{
		this->actionState = 1;
		this->set_mario_animation(MARIO_ANIM_GENERAL_FALL);
	}

	return FALSE;
}

s32 PlayerRecord::act_spawn_spin_landing()
{
	stop_and_set_height_to_floor();
	this->set_mario_animation(MARIO_ANIM_GENERAL_LAND);
	if(this->is_anim_at_end())
	{
		PL_DisplayMessage(0);
		this->ChangePlayerStatus(ACT_IDLE, 0);
	}
	return FALSE;
}

/**
 * act_exit_airborne: Jump out of a level after collecting a Power Star (no
 ** sparkles)
 * Mario always faces a level entrance when he launches out of it, whether he
 * died or he collected a star/key. Because of that, we need him to move away
 * from the painting by setting his speed to -32.0f and have him face away from
 * the painting by adding 0x8000 (180 deg) to his graphics angle. We also set
 * his heal counter to 31 to restore 7.75 units of his health, and enable the
 * particle flag that generates sparkles.
 */
s32 PlayerRecord::act_exit_airborne()
{
	if(15 * FRAME_RATE_SCALER_INV < this->actionTimer++ && launch_mario_until_land(ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_GENERAL_FALL, -32.0f))
	{
		// heal mario
		this->healCounter = 31;
	}

	// rotate him to face away from the entrance
	this->marioObj->header.gfx.angle[1] += 0x8000;
	this->particleFlags |= PARTICLE_SPARKLES;
	return FALSE;
}

s32 PlayerRecord::act_falling_exit_airborne()
{
	if(launch_mario_until_land(ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_GENERAL_FALL, 0.0f))
	{
		// heal mario
		this->healCounter = 31;
	}
	// rotate mario to face away from the entrance
	this->marioObj->header.gfx.angle[1] += 0x8000;
	this->particleFlags |= PARTICLE_SPARKLES;
	return FALSE;
}

s32 PlayerRecord::act_exit_land_save_dialog()
{
	s32 animFrame;
	stationary_ground_step();
	this->play_mario_landing_sound_once(SOUND_ACTION_TERRAIN_LANDING);

	switch(this->actionState)
	{
		// determine type of exit
		case 0:
			this->set_mario_animation(this->actionArg == 0 ? MARIO_ANIM_GENERAL_LAND : MARIO_ANIM_LAND_FROM_SINGLE_JUMP);
			if(this->is_anim_past_end())
			{
				if(gLastCompletedCourseNum != COURSE_BITDW && gLastCompletedCourseNum != COURSE_BITFS)
				{
					enable_time_stop();
				}

				CallSelectMessage(RENDER_COURSE_DONE_SCREEN);
				mesgLatch = 0;

				this->actionState = 3; // star exit with cap
				if(!(this->flags & MARIO_CAP_ON_HEAD))
				{
					this->actionState = 2; // star exit without cap
				}
				if(gLastCompletedCourseNum == COURSE_BITDW || gLastCompletedCourseNum == COURSE_BITFS)
				{
					this->actionState = 1; // key exit
				}
			}
			break;
			// key exit
		case 1:
			animFrame = this->set_mario_animation(MARIO_ANIM_THROW_CATCH_KEY);
			switch(animFrame)
			{
				case -1:
					spawn_obj_at_mario_rel_yaw(MODEL_BOWSER_KEY_CUTSCENE, sm64::bhv::bhvBowserKeyCourseExit(), -32768);
					//! fall through
				case 67:
					AudStartSound(SOUND_ACTION_KEY_SWISH, this->marioObj->header.gfx.cameraToObject);
					//! fall through
				case 83:
					AudStartSound(SOUND_ACTION_PAT_BACK, this->marioObj->header.gfx.cameraToObject);
					//! fall through
				case 111:
					AudStartSound(SOUND_ACTION_UNKNOWN45C, this->marioObj->header.gfx.cameraToObject);
					// no break
			}
			handle_save_menu();
			break;
			// exit without cap
		case 2:
			animFrame = this->set_mario_animation(MARIO_ANIM_MISSING_CAP);
			if((animFrame >= 18 && animFrame < 55) || (animFrame >= 112 && animFrame < 134))
			{
				this->marioBodyState->handState = MARIO_HAND_OPEN;
			}
			if(!(animFrame < 109) && animFrame < 154)
			{
				this->marioBodyState->eyeState = MARIO_EYES_HALF_CLOSED;
			}

			handle_save_menu();
			break;
			// exit with cap
		case 3:
			animFrame = this->set_mario_animation(MARIO_ANIM_TAKE_CAP_OFF_THEN_ON);
			switch(animFrame)
			{
				case 12:
					cutscene_take_cap_off();
					break;
				case 37:
					// fall through
				case 53:
					AudStartSound(SOUND_ACTION_BRUSH_HAIR, this->marioObj->header.gfx.cameraToObject);
					break;
				case 82:
					cutscene_put_cap_on();
					break;
			}
			handle_save_menu();
			break;
	}

	this->marioObj->header.gfx.angle[1] += 0x8000;
	return FALSE;
}

s32 PlayerRecord::act_death_exit()
{
	if(15 * FRAME_RATE_SCALER_INV < this->actionTimer++ && launch_mario_until_land(ACT_DEATH_EXIT_LAND, MARIO_ANIM_GENERAL_FALL, -32.0f))
	{
		AudStartSound(SOUND_MARIO_OOOF2, this->marioObj->header.gfx.cameraToObject);
		SendMotorEvent(5, 80);
		if(!sm64::config().cheats().unlimitedLives())
		{
			this->numLives--;
		}
		// restore 7.75 units of health
		this->healCounter = 31;
	}
	// one unit of health
	this->health = 0x0100;
	return FALSE;
}

s32 PlayerRecord::act_unused_death_exit()
{
	if(launch_mario_until_land(ACT_FREEFALL_LAND_STOP, MARIO_ANIM_GENERAL_FALL, 0.0f))
	{
		AudStartSound(SOUND_MARIO_OOOF2, this->marioObj->header.gfx.cameraToObject);
		if(!sm64::config().cheats().unlimitedLives())
		{
			this->numLives--;
		}
		// restore 7.75 units of health
		this->healCounter = 31;
	}
	// one unit of health
	this->health = 0x0100;
	return FALSE;
}

s32 PlayerRecord::act_falling_death_exit()
{
	if(launch_mario_until_land(ACT_DEATH_EXIT_LAND, MARIO_ANIM_GENERAL_FALL, 0.0f))
	{
		AudStartSound(SOUND_MARIO_OOOF2, this->marioObj->header.gfx.cameraToObject);
		SendMotorEvent(5, 80);
		if(!sm64::config().cheats().unlimitedLives())
		{
			this->numLives--;
		}
		// restore 7.75 units of health
		this->healCounter = 31;
	}
	// one unit of health
	this->health = 0x0100;
	return FALSE;
}

// waits 11 frames before actually executing, also has reduced fvel
s32 PlayerRecord::act_special_exit_airborne()
{
	struct Object* marioObj = this->marioObj;

	this->play_sound_if_no_flag(SOUND_MARIO_YAHOO, MARIO_MARIO_SOUND_PLAYED);

	if(this->actionTimer++ < 11 * FRAME_RATE_SCALER_INV)
	{
		marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
		return FALSE;
	}

	if(launch_mario_until_land(ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_SINGLE_JUMP, -24.0f))
	{
		// heal mario
		this->healCounter = 31;
		this->actionArg	  = 1;
	}

	this->particleFlags |= PARTICLE_SPARKLES;
	// rotate mario to face away from the entrance
	marioObj->header.gfx.angle[1] += 0x8000;
	// show mario
	marioObj->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;

	return FALSE;
}

s32 PlayerRecord::act_special_death_exit()
{
	struct Object* marioObj = this->marioObj;

	if(this->actionTimer++ < 11 * FRAME_RATE_SCALER_INV)
	{
		marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
		return FALSE;
	}

	if(launch_mario_until_land(ACT_HARD_BACKWARD_GROUND_KB, MARIO_ANIM_BACKWARD_AIR_KB, -24.0f))
	{
		SendMotorEvent(5, 80);
		if(!sm64::config().cheats().unlimitedLives())
		{
			this->numLives--;
		}
		this->healCounter = 31;
	}
	// show mario
	marioObj->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;
	// one unit of health
	this->health = 0x0100;

	return FALSE;
}

s32 PlayerRecord::act_spawn_no_spin_airborne()
{
	launch_mario_until_land(ACT_SPAWN_NO_SPIN_LANDING, MARIO_ANIM_GENERAL_FALL, 0.0f);
	if(this->pos[1] < this->waterLevel - 100)
	{
		this->set_water_plunge_action();
	}
	return FALSE;
}

s32 PlayerRecord::act_spawn_no_spin_landing()
{
	this->play_mario_landing_sound_once(SOUND_ACTION_TERRAIN_LANDING);
	this->set_mario_animation(MARIO_ANIM_GENERAL_LAND);
	stop_and_set_height_to_floor();
	if(this->is_anim_at_end())
	{
		PL_DisplayMessage(0);
		this->ChangePlayerStatus(ACT_IDLE, 0);
	}
	return FALSE;
}

s32 PlayerRecord::act_bbh_enter_spin()
{
	f32 floorDist;
	f32 scale;
	f32 cageDX;
	f32 cageDZ;
	f32 cageDist;
	f32 forwardVel;

	cageDX	 = this->usedObj->oPosX - this->pos[0];
	cageDZ	 = this->usedObj->oPosZ - this->pos[2];
	cageDist = sqrtf(cageDX * cageDX + cageDZ * cageDZ);

	if(cageDist > 20.0f)
	{
		forwardVel = 10.0f;
	}
	else
	{
		forwardVel = cageDist / 2.0f;
	}
	if(forwardVel < 0.5f)
	{
		forwardVel = 0.0f;
	}

	switch(this->actionState)
	{
		case 0:
			floorDist    = 512.0f - (this->pos[1] - this->floorHeight);
			this->vel[1] = floorDist > 0 ? sqrtf(4.0f * floorDist + 1.0f) - 1.0f : 2.0f;

			this->actionState = 1;
			this->actionTimer = 100 * FRAME_RATE_SCALER_INV;
			// fall through

		case 1:
			this->faceAngle[1] = atan2s(cageDZ, cageDX);
			this->mario_set_forward_vel(forwardVel);

			if(this->set_mario_animation(MARIO_ANIM_FORWARD_SPINNING) == 0)
			{
				AudStartSound(SOUND_ACTION_SPIN, this->marioObj->header.gfx.cameraToObject);
			}

			this->flags &= ~MARIO_UNKNOWN_08;
			perform_air_step(0);
			if(this->vel[1] <= 0)
			{
				this->actionState = 2;
			}
			break;

		case 2:
			// fall through
		case 3:
			this->faceAngle[1] = atan2s(cageDZ, cageDX);
			this->mario_set_forward_vel(forwardVel);
			this->flags &= ~MARIO_UNKNOWN_08;
			if(perform_air_step(0) == AIR_STEP_LANDED)
			{
				PL_StartFadeout(this, PL_FADE_TRIPKAGO);
				SendMotorEvent(15, 80);
				this->actionState = 4;
			}
			if(this->actionState == 2)
			{
				if(this->marioObj->header.gfx.unk38.frame() == 0)
				{
					this->actionState = 3;
				}
			}
			else
			{
				this->play_sound_if_no_flag(SOUND_ACTION_SHRINK_INTO_BBH, MARIO_ACTION_SOUND_PLAYED);
				this->set_mario_animation(MARIO_ANIM_DIVE);
				this->marioObj->header.gfx.angle[0] = atan2s(this->forwardVel, -this->vel[1]);
			}

			this->squishTimer = 0xFF * FRAME_RATE_SCALER_INV;

			if(this->actionTimer >= 11 * FRAME_RATE_SCALER_INV)
			{
				this->actionTimer -= 6;
				scale = this->actionTimer / FRAME_RATE_SCALER_INV / 100.0f;
				vec3f_set(this->marioObj->header.gfx.scale, scale, scale, scale);
			}
			break;

		case 4:
			stop_and_set_height_to_floor();
			this->marioObj->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
			break;
	}

	return FALSE;
}

s32 PlayerRecord::act_bbh_enter_jump()
{
	f32 cageDX;
	f32 cageDZ;
	f32 cageDist;

	this->play_mario_action_sound(this->flags & MARIO_METAL_CAP ? SOUND_ACTION_METAL_JUMP : SOUND_ACTION_TERRAIN_JUMP, 1);
	this->play_mario_jump_sound();

	if(this->actionState == 0)
	{
		cageDX	 = this->usedObj->oPosX - this->pos[0];
		cageDZ	 = this->usedObj->oPosZ - this->pos[2];
		cageDist = sqrtf(cageDX * cageDX + cageDZ * cageDZ);

		this->vel[1]	   = 60.0f;
		this->faceAngle[1] = atan2s(cageDZ, cageDX);
		this->mario_set_forward_vel(cageDist / 20.0f);

		this->flags &= ~MARIO_UNKNOWN_08;
		this->actionState = 1;
	}

	this->set_mario_animation(MARIO_ANIM_DOUBLE_JUMP_RISE);
	perform_air_step(0);

	if(this->vel[1] <= 0.0f)
	{
		this->ChangePlayerStatus(ACT_BBH_ENTER_SPIN, 0);
	}

	return FALSE;
}

s32 PlayerRecord::act_teleport_fade_out()
{
	this->play_sound_if_no_flag(SOUND_ACTION_TELEPORT, MARIO_ACTION_SOUND_PLAYED);
	this->set_mario_animation(this->oldstatus == ACT_CROUCHING ? MARIO_ANIM_CROUCHING : MARIO_ANIM_FIRST_PERSON);

	if(this->actionTimer == 0)
	{
		SendMotorEvent(30, 70);
	}

	this->flags |= MARIO_TELEPORTING;

	if(this->actionTimer < 32 * FRAME_RATE_SCALER_INV)
	{
		this->fadeWarpOpacity = ((-this->actionTimer / FRAME_RATE_SCALER_INV) << 3) + 0xF8;
	}

	if(this->actionTimer++ == 20 * FRAME_RATE_SCALER_INV)
	{
		PL_StartFadeout(this, PL_FADE_WARPIN);
	}

	stop_and_set_height_to_floor();

	return FALSE;
}

s32 PlayerRecord::act_teleport_fade_in()
{
	this->play_sound_if_no_flag(SOUND_ACTION_TELEPORT, MARIO_ACTION_SOUND_PLAYED);
	this->set_mario_animation(MARIO_ANIM_FIRST_PERSON);

	if(this->actionTimer == 0)
	{
		SendMotorEvent(30, 70);
	}

	if(this->actionTimer < 32 * FRAME_RATE_SCALER_INV)
	{
		this->flags |= MARIO_TELEPORTING;
		this->fadeWarpOpacity = (this->actionTimer / FRAME_RATE_SCALER_INV) << 3;
	}
	else
	{
		this->flags &= ~MARIO_TELEPORTING;
	}

	if(this->actionTimer++ == 32 * FRAME_RATE_SCALER_INV)
	{
		if(this->pos[1] < this->waterLevel - 100)
		{
			// Check if the camera is not underwater.
			if(this->area->camera->mode != CAMERA_MODE_WATER_SURFACE)
			{
				set_camera_mode(this->area->camera, CAMERA_MODE_WATER_SURFACE, 1);
			}
			this->ChangePlayerStatus(ACT_WATER_IDLE, 0);
		}
		else
		{
			this->ChangePlayerStatus(ACT_IDLE, 0);
		}
	}

	stop_and_set_height_to_floor();

	return FALSE;
}

// act shocked :omega:
s32 PlayerRecord::act_shocked()
{
	this->play_sound_if_no_flag(SOUND_MARIO_WAAAOOOW, MARIO_ACTION_SOUND_PLAYED);
	AudStartSound(SOUND_MOVING_SHOCKED, this->marioObj->header.gfx.cameraToObject);
	set_camera_shake_from_hit(SHAKE_SHOCK);

	if(this->set_mario_animation(MARIO_ANIM_SHOCKED) == 0)
	{
		this->actionTimer++;
		this->flags |= MARIO_METAL_SHOCK;
	}

	if(this->actionArg == 0)
	{
		this->mario_set_forward_vel(0.0f);
		if(perform_air_step(1) == AIR_STEP_LANDED)
		{
			this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
			this->actionArg = 1;
		}
	}
	else
	{
		if(this->actionTimer >= 6 * FRAME_RATE_SCALER_INV)
		{
			this->invincTimer = 30 * FRAME_RATE_SCALER_INV;
			this->ChangePlayerStatus(this->health < 0x0100 ? ACT_ELECTROCUTION : ACT_IDLE, 0);
		}
		stop_and_set_height_to_floor();
	}

	return FALSE;
}

s32 PlayerRecord::act_squished()
{
	UNUSED s32 pad;
	f32 squishAmount;
	f32 spaceUnderCeil;
	s16 surfAngle;
	s32 underSteepSurf = FALSE; // seems to be responsible for setting velocity?

	if((spaceUnderCeil = this->ceilHeight - this->floorHeight) < 0)
	{
		spaceUnderCeil = 0;
	}

	switch(this->actionState)
	{
		case 0:
			if(spaceUnderCeil > 160.0f)
			{
				this->squishTimer = 0;
				return this->ChangePlayerStatus(ACT_IDLE, 0);
			}

			this->squishTimer = 0xFF * FRAME_RATE_SCALER_INV;

			if(spaceUnderCeil >= 10.1f)
			{
				// mario becomes a pancake
				squishAmount = spaceUnderCeil / 160.0f;
				vec3f_set(this->marioObj->header.gfx.scale, 2.0f - squishAmount, squishAmount, 2.0f - squishAmount);
			}
			else
			{
				if(!(this->flags & MARIO_METAL_CAP) && this->invincTimer == 0)
				{
					// cap on: 3 units; cap off: 4.5 units
					this->hurtCounter += this->flags & MARIO_CAP_ON_HEAD ? 12 : 18;
					this->play_sound_if_no_flag(SOUND_MARIO_ATTACKED, MARIO_MARIO_SOUND_PLAYED);
				}

				// Both of the 1.8's are really floats, but one of them has to
				// be written as a double for this to match on EU.
				vec3f_set(this->marioObj->header.gfx.scale, 1.8, 0.05f, 1.8f);
				SendMotorEvent(10, 80);
				this->actionState = 1;
			}
			break;
		case 1:
			if(spaceUnderCeil >= 30.0f)
			{
				this->actionState = 2;
			}
			break;
		case 2:
			this->actionTimer++;
			if(this->actionTimer >= 15 * FRAME_RATE_SCALER_INV)
			{
				// 1 unit of health
				if(this->health < 0x0100)
				{
					PL_StartFadeout(this, PL_FADE_LOSING);
					// woosh, he's gone!
					this->ChangePlayerStatus(ACT_DISAPPEARED, 0);
				}
				else if(this->hurtCounter == 0)
				{
					// un-squish animation
					this->squishTimer = 30 * FRAME_RATE_SCALER_INV;
					this->ChangePlayerStatus(ACT_IDLE, 0);
				}
			}
			break;
	}

	// steep floor
	if(this->floor != NULL && this->floor->normal.y < 0.5f)
	{
		surfAngle      = atan2s(this->floor->normal.z, this->floor->normal.x);
		underSteepSurf = TRUE;
	}
	// steep ceiling
	if(this->ceil != NULL && -0.5f < this->ceil->normal.y)
	{
		surfAngle      = atan2s(this->ceil->normal.z, this->ceil->normal.x);
		underSteepSurf = TRUE;
	}

	if(underSteepSurf)
	{
		this->vel[0] = sins(surfAngle) * 10.0f;
		this->vel[2] = coss(surfAngle) * 10.0f;
		this->vel[1] = 0;

		// check if there's no floor 10 units away from the surface
		if(perform_ground_step() == GROUND_STEP_LEFT_GROUND)
		{
			// instant un-squish
			this->squishTimer = 0;
			this->ChangePlayerStatus(ACT_IDLE, 0);
			return FALSE;
		}
	}

	// squished for more than 10 seconds, so kill mario
	if(this->actionArg++ > 300)
	{
		// 0 units of health
		this->health	  = 0x00FF;
		this->hurtCounter = 0;
		PL_StartFadeout(this, PL_FADE_LOSING);
		// woosh, he's gone!
		this->ChangePlayerStatus(ACT_DISAPPEARED, 0);
	}
	stop_and_set_height_to_floor();
	this->set_mario_animation(MARIO_ANIM_A_POSE);
	return FALSE;
}

s32 PlayerRecord::act_putting_on_cap()
{
	s32 animFrame = this->set_mario_animation(MARIO_ANIM_PUT_CAP_ON);

	if(animFrame == 0)
	{
		enable_time_stop();
	}

	if(animFrame == 28)
	{
		cutscene_put_cap_on();
	}

	if(this->is_anim_at_end())
	{
		this->ChangePlayerStatus(ACT_IDLE, 0);
		disable_time_stop();
	}

	stationary_ground_step();
	return FALSE;
}

void PlayerRecord::stuck_in_ground_handler(s32 animation, s32 unstuckFrame, s32 target2, s32 target3, s32 endAction)
{
	s32 animFrame = this->set_mario_animation(animation);

	if(this->input & INPUT_A_PRESSED)
	{
		this->actionTimer++;
		if(this->actionTimer >= 5 * FRAME_RATE_SCALER_INV && animFrame < unstuckFrame - 1)
		{
			animFrame = unstuckFrame - 1;
			this->set_anim_to_frame(animFrame);
		}
	}

	stop_and_set_height_to_floor();

	if(animFrame == -1)
	{
		this->play_sound_and_spawn_particles(SOUND_ACTION_TERRAIN_STUCK_IN_GROUND, 1);
	}
	else if(animFrame == unstuckFrame)
	{
		SendMotorEvent(5, 80);
		this->play_sound_and_spawn_particles(SOUND_ACTION_UNSTUCK_FROM_GROUND, 1);
	}
	else if(animFrame == target2 || animFrame == target3)
	{
		this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
	}

	if(this->is_anim_at_end())
	{
		this->ChangePlayerStatus(endAction, 0);
	}
}

s32 PlayerRecord::act_head_stuck_in_ground()
{
	stuck_in_ground_handler(MARIO_ANIM_HEAD_STUCK_IN_GROUND, 96, 105, 135, ACT_IDLE);
	return FALSE;
}

s32 PlayerRecord::act_butt_stuck_in_ground()
{
	stuck_in_ground_handler(MARIO_ANIM_BOTTOM_STUCK_IN_GROUND, 127, 136, -2, ACT_GROUND_POUND_LAND);
	return FALSE;
}

s32 PlayerRecord::act_feet_stuck_in_ground()
{
	stuck_in_ground_handler(MARIO_ANIM_LEGS_STUCK_IN_GROUND, 116, 129, -2, ACT_IDLE);
	return FALSE;
}

/**
 * advance_cutscene_step: Advances the current step in the current cutscene.
 * Resets action state and action timer, adds 1 to the action arg (responsible
 * for keeping track of what step of the cutscene Mario is in.)
 */
void PlayerRecord::advance_cutscene_step()
{
	this->actionState = 0;
	this->actionTimer = 0;
	this->actionArg++;
}

void PlayerRecord::intro_cutscene_hide_hud_and_mario()
{
	playerMeter.flags		   = HUD_DISPLAY_NONE;
	this->statusForCamera->cameraEvent = CAM_EVENT_START_INTRO;
	this->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
	advance_cutscene_step();
}

void PlayerRecord::intro_cutscene_peach_lakitu_scene()
{
	if((s16)this->statusForCamera->cameraEvent != CAM_EVENT_START_INTRO)
	{
		if(this->actionTimer++ == Duration(37))
		{
			sIntroWarpPipeObj = s_makeobj_absolute(gCurrentObject, 0, MODEL_CASTLE_GROUNDS_WARP_PIPE, sm64::bhv::bhvStaticObject(), -1328, 60, 4664, 0, 180, 0);
			advance_cutscene_step();
		}
	}
}

void PlayerRecord::intro_cutscene_raise_pipe()
{
	sIntroWarpPipeObj->oPosY = camera_approach_f32_symmetric(sIntroWarpPipeObj->oPosY, 260.0f, 10.0f * FRAME_RATE_SCALER);

	if(this->actionTimer == 0)
	{
		AudStartSound(SOUND_MENU_EXIT_PIPE, sIntroWarpPipeObj->header.gfx.cameraToObject);
	}

	if(this->actionTimer++ == Duration(38))
	{
		this->vel[1] = 60.0f;
		advance_cutscene_step();
	}
}

void PlayerRecord::intro_cutscene_jump_out_of_pipe()
{
	if(this->actionTimer == Duration(25))
	{
		playerMeter.flags = HUD_DISPLAY_DEFAULT;
	}

	if(this->actionTimer++ >= 118 * FRAME_RATE_SCALER_INV)
	{
		this->marioObj->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;

		this->play_sound_if_no_flag(SOUND_MARIO_YAHOO, MARIO_MARIO_SOUND_PLAYED);
		this->play_sound_if_no_flag(SOUND_ACTION_HIT_3, MARIO_ACTION_SOUND_PLAYED);

		this->set_mario_animation(MARIO_ANIM_SINGLE_JUMP);
		this->mario_set_forward_vel(10.0f);
		if(perform_air_step(0) == AIR_STEP_LANDED)
		{
			sound_banks_enable(2, 0x0330);
			this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
			AudStartSound(SOUND_MARIO_HAHA, this->marioObj->header.gfx.cameraToObject);
			advance_cutscene_step();
		}
	}
}

void PlayerRecord::intro_cutscene_land_outside_pipe()
{
	this->set_mario_animation(MARIO_ANIM_LAND_FROM_SINGLE_JUMP);

	if(this->is_anim_at_end())
	{
		advance_cutscene_step();
	}

	stop_and_set_height_to_floor();
}

void PlayerRecord::intro_cutscene_lower_pipe()
{
	if(this->actionTimer++ == 0)
	{
		AudStartSound(SOUND_MENU_ENTER_PIPE, sIntroWarpPipeObj->header.gfx.cameraToObject);
		this->set_mario_animation(MARIO_ANIM_FIRST_PERSON);
	}

	sIntroWarpPipeObj->oPosY -= 5.0f;
	if(sIntroWarpPipeObj->oPosY <= 50.0f)
	{
		s_remove_obj(sIntroWarpPipeObj);
		advance_cutscene_step();
	}

	stop_and_set_height_to_floor();
}

void PlayerRecord::intro_cutscene_set_mario_to_idle()
{
	if(gCamera->cutscene == 0)
	{
		gameCameraControl &= ~CAM_MOVE_C_UP_MODE;
		this->ChangePlayerStatus(ACT_IDLE, 0);
	}

	stop_and_set_height_to_floor();
}

enum
{
	INTRO_CUTSCENE_HIDE_HUD_AND_MARIO,
	INTRO_CUTSCENE_PEACH_LAKITU_SCENE,
	INTRO_CUTSCENE_RAISE_PIPE,
	INTRO_CUTSCENE_JUMP_OUT_OF_PIPE,
	INTRO_CUTSCENE_LAND_OUTSIDE_PIPE,
	INTRO_CUTSCENE_LOWER_PIPE,
	INTRO_CUTSCENE_SET_MARIO_TO_IDLE
};

s32 PlayerRecord::act_intro_cutscene()
{
	switch(this->actionArg)
	{
		case INTRO_CUTSCENE_HIDE_HUD_AND_MARIO:
			intro_cutscene_hide_hud_and_mario();
			break;
		case INTRO_CUTSCENE_PEACH_LAKITU_SCENE:
			intro_cutscene_peach_lakitu_scene();
			break;
		case INTRO_CUTSCENE_RAISE_PIPE:
			intro_cutscene_raise_pipe();
			break;
		case INTRO_CUTSCENE_JUMP_OUT_OF_PIPE:
			intro_cutscene_jump_out_of_pipe();
			break;
		case INTRO_CUTSCENE_LAND_OUTSIDE_PIPE:
			intro_cutscene_land_outside_pipe();
			break;
		case INTRO_CUTSCENE_LOWER_PIPE:
			intro_cutscene_lower_pipe();
			break;
		case INTRO_CUTSCENE_SET_MARIO_TO_IDLE:
			intro_cutscene_set_mario_to_idle();
			break;
	}
	return FALSE;
}

// jumbo star cutscene: mario lands after grabbing the jumbo star
void PlayerRecord::jumbo_star_cutscene_falling()
{
	if(this->actionState == 0)
	{
		this->input |= INPUT_A_DOWN;
		this->flags |= (MARIO_WING_CAP | MARIO_CAP_ON_HEAD);

		this->faceAngle[1] = -0x8000;
		this->pos[0]	   = 0.0f;
		this->pos[2]	   = 0.0f;

		this->mario_set_forward_vel(0.0f);
		this->set_mario_animation(MARIO_ANIM_GENERAL_FALL);

		if(perform_air_step(1) == AIR_STEP_LANDED)
		{
			play_cutscene_music(SEQUENCE_ARGS(15, SEQ_EVENT_CUTSCENE_VICTORY));
			this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
			this->actionState++;
		}
	}
	else
	{
		this->set_mario_animation(MARIO_ANIM_GENERAL_LAND);
		if(this->is_anim_at_end())
		{
			this->statusForCamera->cameraEvent = CAM_EVENT_START_GRAND_STAR;
			advance_cutscene_step();
		}
	}
}

// jumbo star cutscene: mario takes off
s32 PlayerRecord::jumbo_star_cutscene_taking_off()
{
	struct Object* marioObj = this->marioObj;
	s32 animFrame;

	if(this->actionState == 0)
	{
		this->set_mario_animation(MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN);
		marioObj->rawData.asF32[0x22] = 0.0f;

		if(this->is_anim_past_end())
		{
			this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
			this->actionState++;
		}
	}
	else
	{
		if(!this->marioObj->header.gfx.unk38.isPartialFrame())
		{
			animFrame = this->set_mario_animation(MARIO_ANIM_FINAL_BOWSER_WING_CAP_TAKE_OFF);
			if(animFrame == 3 || animFrame == 28 || animFrame == 60)
			{
				this->play_sound_and_spawn_particles(SOUND_ACTION_TERRAIN_JUMP, 1);
			}
			if(animFrame >= 3)
			{
				marioObj->rawData.asF32[0x22] -= 32.0f * FRAME_RATE_SCALER;
			}

			switch(animFrame)
			{
				case 3:
					AudStartSound(SOUND_MARIO_YAH_WAH_HOO + (gAudioRandom % 3 << 16), this->marioObj->header.gfx.cameraToObject);
					break;

				case 28:
					AudStartSound(SOUND_MARIO_HOOHOO, this->marioObj->header.gfx.cameraToObject);
					break;

				case 60:
					AudStartSound(SOUND_MARIO_YAHOO, this->marioObj->header.gfx.cameraToObject);
					break;
			}
		}
		this->particleFlags |= PARTICLE_SPARKLES;

		if(this->is_anim_past_end())
		{
			advance_cutscene_step();
		}
	}

	vec3f_set(this->pos, 0.0f, 307.0, marioObj->rawData.asF32[0x22]);
	this->update_mario_pos_for_anim();
	vec3f_copy(marioObj->header.gfx.pos, this->pos);
	vec3s_set(marioObj->header.gfx.angle, 0, this->faceAngle[1], 0);

	// not sure why they did this, probably was from being used to action
	// functions
	return FALSE;
}

// jumbo star cutscene: mario flying
s32 PlayerRecord::jumbo_star_cutscene_flying()
{
	Vec3f targetPos;
	UNUSED struct Object* marioObj = this->marioObj;
	f32 targetDX;
	f32 targetDY;
	f32 targetDZ;
	f32 targetHyp;
	s16 targetAngle;

	switch(this->actionState)
	{
		case 0:
			this->set_mario_animation(MARIO_ANIM_WING_CAP_FLY);
			anim_spline_init(sJumboStarKeyframes);
			this->actionState++;
			// fall through
		case 1:
			if(anim_spline_poll(targetPos))
			{
				// lol does this twice
				this->ChangePlayerStatus(ACT_FREEFALL, 0);
				this->actionState++;
			}
			else
			{
				targetDX    = targetPos[0] - this->pos[0];
				targetDY    = targetPos[1] - this->pos[1];
				targetDZ    = targetPos[2] - this->pos[2];
				targetHyp   = sqrtf(targetDX * targetDX + targetDZ * targetDZ);
				targetAngle = atan2s(targetDZ, targetDX);

				vec3f_copy(this->pos, targetPos);
				this->marioObj->header.gfx.angle[0] = -atan2s(targetHyp, targetDY);
				this->marioObj->header.gfx.angle[1] = targetAngle;
				this->marioObj->header.gfx.angle[2] = ((this->faceAngle[1] - targetAngle) << 16 >> 16) * 20;
				this->faceAngle[1]		    = targetAngle;
			}
			break;
		case 2:
			this->ChangePlayerStatus(ACT_FREEFALL, 0);
			break;
	}

	this->marioBodyState->handState = MARIO_HAND_RIGHT_OPEN;
	vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
	this->particleFlags |= PARTICLE_SPARKLES;

	if(this->actionTimer++ == 500 * FRAME_RATE_SCALER_INV)
	{
		PL_StartFadeout(this, PL_FADE_ENDING);
	}

	return FALSE;
}

enum
{
	JUMBO_STAR_CUTSCENE_FALLING,
	JUMBO_STAR_CUTSCENE_TAKING_OFF,
	JUMBO_STAR_CUTSCENE_FLYING
};

s32 PlayerRecord::act_jumbo_star_cutscene()
{
	switch(this->actionArg)
	{
		case JUMBO_STAR_CUTSCENE_FALLING:
			jumbo_star_cutscene_falling();
			break;
		case JUMBO_STAR_CUTSCENE_TAKING_OFF:
			jumbo_star_cutscene_taking_off();
			break;
		case JUMBO_STAR_CUTSCENE_FLYING:
			jumbo_star_cutscene_flying();
			break;
	}
	return FALSE;
}

// make mario fall and soften wing cap gravity
void PlayerRecord::end_peach_cutscene_mario_falling()
{
	if(this->actionTimer == 1 * FRAME_RATE_SCALER_INV)
	{
		this->statusForCamera->cameraEvent = CAM_EVENT_START_ENDING;
	}

	this->input |= INPUT_A_DOWN;
	this->flags |= (MARIO_WING_CAP | MARIO_CAP_ON_HEAD);

	this->set_mario_animation(MARIO_ANIM_GENERAL_FALL);
	this->mario_set_forward_vel(0.0f);

	if(perform_air_step(0) == AIR_STEP_LANDED)
	{
		this->play_mario_landing_sound(SOUND_ACTION_TERRAIN_LANDING);
		advance_cutscene_step();
	}
}

// set mario on the ground, wait and spawn the jumbo star outside the castle.
void PlayerRecord::end_peach_cutscene_mario_landing()
{
	this->set_mario_animation(MARIO_ANIM_GENERAL_LAND);
	stop_and_set_height_to_floor();

	if(this->is_anim_at_end())
	{
		// make wing cap run out
		this->capTimer = 60 * FRAME_RATE_SCALER_INV;

		sEndJumboStarObj = s_makeobj_absolute(gCurrentObject, 0, MODEL_STAR, sm64::bhv::bhvStaticObject(), 0, 2528, -1800, 0, 0, 0);
		scale_object(sEndJumboStarObj, 3.0);
		advance_cutscene_step();
	}
}

// raise hand animation, lower hand animation, do some special effects
void PlayerRecord::end_peach_cutscene_summon_jumbo_star()
{
	this->set_mario_animation(this->actionState == 0 ? MARIO_ANIM_CREDITS_RAISE_HAND : MARIO_ANIM_CREDITS_LOWER_HAND);

	if(this->actionState == 0 && this->is_anim_past_end())
	{
		this->actionState++;
	}
	if(this->actionTimer == 90 * FRAME_RATE_SCALER_INV)
	{
		play_cutscene_music(SEQUENCE_ARGS(0, SEQ_EVENT_CUTSCENE_ENDING));
	}
	if(this->actionTimer == 255 * FRAME_RATE_SCALER_INV)
	{
		advance_cutscene_step();
	}

	sEndJumboStarObj->oFaceAngleYaw += 0x0400 / FRAME_RATE_SCALER_INV;
	generate_yellow_sparkles(0, 2528, -1800, 250.0f);
	AudStartSound(SOUND_AIR_PEACH_TWINKLE, sEndJumboStarObj->header.gfx.cameraToObject);
}

// free peach from the stained glass window
void PlayerRecord::end_peach_cutscene_spawn_peach()
{
	if(this->actionTimer == 1 * FRAME_RATE_SCALER_INV)
	{
		SnStartFader(WARP_TRANSITION_FADE_INTO_COLOR, 14, 255, 255, 255);
	}
	if(this->actionTimer == 2 * FRAME_RATE_SCALER_INV)
	{
		AudStartSound(SOUND_MENU_STAR_SOUND, gDefaultSoundArgs);
	}

	if(this->actionTimer == 40 * FRAME_RATE_SCALER_INV)
	{
		s_remove_obj(sEndJumboStarObj);

		sEndPeachObj   = s_makeobj_absolute(gCurrentObject, 0, MODEL_PEACH, sm64::bhv::bhvEndPeach(), 0, 2428, -1300, 0, 0, 0);
		gCutsceneFocus = sEndPeachObj;

		sEndRightToadObj = s_makeobj_absolute(gCurrentObject, 0, MODEL_TOAD, sm64::bhv::bhvEndToad(), 200, 906, -1290, 0, 0, 0);

		sEndLeftToadObj = s_makeobj_absolute(gCurrentObject, 0, MODEL_TOAD, sm64::bhv::bhvEndToad(), -200, 906, -1290, 0, 0, 0);

		sEndPeachObj->oOpacity	   = 127;
		sEndRightToadObj->oOpacity = 255;
		sEndLeftToadObj->oOpacity  = 255;

		D_8032CBE4	   = 4;
		sEndPeachAnimation = 4;

		sEndToadAnims[0] = 4;
		sEndToadAnims[1] = 5;
	}

	if(this->actionTimer == 44 * FRAME_RATE_SCALER_INV)
	{
		SnStartFader(WARP_TRANSITION_FADE_FROM_COLOR, 192, 255, 255, 255);
	}

	if(this->actionTimer >= 276 * FRAME_RATE_SCALER_INV)
	{
		sEndPeachObj->oOpacity = camera_approach_f32_symmetric(sEndPeachObj->oOpacity, 255.0f, 2.0f * FRAME_RATE_SCALER);
	}
	if(this->actionTimer >= 40 * FRAME_RATE_SCALER_INV)
	{
		generate_yellow_sparkles(0, 2628, -1300, 150.0f);
	}
	if(this->actionTimer == 355 * FRAME_RATE_SCALER_INV)
	{
		advance_cutscene_step();
	}
	// probably added sounds later and missed the previous >= 40 check
	if(this->actionTimer >= 40 * FRAME_RATE_SCALER_INV)
	{
		AudStartSound(SOUND_AIR_PEACH_TWINKLE, sEndPeachObj->header.gfx.cameraToObject);
	}
}

// descend peach
void PlayerRecord::end_peach_cutscene_descend_peach()
{
	generate_yellow_sparkles(0, sEndPeachObj->oPosY, -1300, 150.0f);

	if(sEndPeachObj->oPosY >= 1300.0f)
	{
		if(this->actionState < 60)
		{
			this->actionState += 5;
		}
	}
	else
	{
		if(this->actionState >= 27)
		{
			this->actionState -= 2;
		}
		this->set_mario_animation(MARIO_ANIM_CREDITS_RETURN_FROM_LOOK_UP);
	}

	if((sEndPeachObj->oPosY -= this->actionState / 10) <= 907.0f)
	{
		sEndPeachObj->oPosY = 906.0f;
	}

	AudStartSound(SOUND_AIR_PEACH_TWINKLE, sEndPeachObj->header.gfx.cameraToObject);

	if(this->actionTimer >= 584 * FRAME_RATE_SCALER_INV)
	{
		advance_cutscene_step();
	}
}

// mario runs to peach
void PlayerRecord::end_peach_cutscene_run_to_peach()
{
	struct Surface* surf;

	if(this->actionTimer == 22 * FRAME_RATE_SCALER_INV)
	{
		sEndPeachAnimation = 5;
	}

	if((this->pos[2] -= 20.0f * FRAME_RATE_SCALER) <= -1181.0f)
	{
		this->pos[2] = -1180.0f;
		advance_cutscene_step();
	}

	this->pos[1] = mcBGGroundCheck(this->pos[0], this->pos[1], this->pos[2], &surf);

	this->set_mario_anim_with_accel(MARIO_ANIM_RUNNING, 0x00080000 / FRAME_RATE_SCALER_INV);
	func_80263AD4(9, 45);

	vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
	this->particleFlags |= PARTICLE_DUST;
}

// dialog 1
// "Mario!"
// "The power of the Stars is restored to the castle..."
void PlayerRecord::end_peach_cutscene_dialog_1()
{
	s32 animFrame = this->set_mario_animation(this->actionState == 0 ? MARIO_ANIM_CREDITS_TAKE_OFF_CAP : MARIO_ANIM_CREDITS_LOOK_UP);

	if(this->actionState == 0)
	{
		if(animFrame == 8)
		{
			cutscene_take_cap_off();
		}

		if(this->is_anim_at_end())
		{
			this->actionState++;
		}
	}

	switch(this->actionTimer)
	{
		case 80 * FRAME_RATE_SCALER_INV:
			sEndPeachAnimation = 6;
			break;

		case 81 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 3;
			break;

		case 145 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 2;
			break;

		case 228 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 1;
			D_8032CBE8 = 1;
			break;

		case 230 * FRAME_RATE_SCALER_INV:
			CallEndingDemoMessage(160, 227, 0, 30);
			Na_SeqVolMute(0, 60, 40);
			AudStartSound(SOUND_PEACH_MARIO, sEndPeachObj->header.gfx.cameraToObject);
			break;

		case 275 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 0;
			D_8032CBE8 = 0;
			break;

		case 290 * FRAME_RATE_SCALER_INV:
			CallEndingDemoMessage(160, 227, 1, 60);
			AudStartSound(SOUND_PEACH_POWER_OF_THE_STARS, sEndPeachObj->header.gfx.cameraToObject);
			break;

		case 480 * FRAME_RATE_SCALER_INV:
			advance_cutscene_step();
			break;
	}
}

// dialog 2
// "...and it's all thanks to you!"
// "Thank you Mario!"
// "We have to do something special for you..."
void PlayerRecord::end_peach_cutscene_dialog_2()
{
	sEndPeachAnimation = 9;

	switch(this->actionTimer)
	{
		case 29 * FRAME_RATE_SCALER_INV:
			CallEndingDemoMessage(160, 227, 2, 30);
			AudStartSound(SOUND_PEACH_THANKS_TO_YOU, sEndPeachObj->header.gfx.cameraToObject);
			break;

		case 45 * FRAME_RATE_SCALER_INV:
			D_8032CBE8 = 1;
			break;

		case 75 * FRAME_RATE_SCALER_INV:
			CallEndingDemoMessage(160, 227, 3, 30);
			AudStartSound(SOUND_PEACH_THANK_YOU_MARIO, sEndPeachObj->header.gfx.cameraToObject);
			break;

		case 130 * FRAME_RATE_SCALER_INV:
			CallEndingDemoMessage(160, 227, 4, 40);
			AudStartSound(SOUND_PEACH_SOMETHING_SPECIAL, sEndPeachObj->header.gfx.cameraToObject);
			break;

		case 200 * FRAME_RATE_SCALER_INV:
			advance_cutscene_step();
			break;
	}
}

void PlayerRecord::end_peach_cutscene_kiss_from_peach()
{
	sEndPeachAnimation = 10;

	if(this->actionTimer >= 90 * FRAME_RATE_SCALER_INV)
	{
		this->marioBodyState->eyeState = this->actionTimer < 110 ? sMarioBlinkOverride[(this->actionTimer) - 90] : MARIO_EYES_HALF_CLOSED;
	}

	switch(this->actionTimer)
	{
		case 8 * FRAME_RATE_SCALER_INV:
			D_8032CBE8 = 0;
			break;

		case 10 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 3;
			break;

		case 50 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 4;
			break;

		case 75 * FRAME_RATE_SCALER_INV:
			this->marioBodyState->eyeState = MARIO_EYES_HALF_CLOSED;
			break;

		case 76 * FRAME_RATE_SCALER_INV:
			this->marioBodyState->eyeState = MARIO_EYES_CLOSED;
			break;

		case 100 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 3;
			break;

		case 136 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 0;
			break;

		case 140 * FRAME_RATE_SCALER_INV:
			advance_cutscene_step();
			break;
	}
}

void PlayerRecord::end_peach_cutscene_star_dance()
{
	s32 animFrame = this->set_mario_animation(MARIO_ANIM_CREDITS_PEACE_SIGN);

	if(animFrame == 77)
	{
		cutscene_put_cap_on();
	}
	if(animFrame == 88)
	{
		AudStartSound(SOUND_MARIO_HERE_WE_GO, this->marioObj->header.gfx.cameraToObject);
	}
	if(animFrame >= 98)
	{
		this->marioBodyState->handState = MARIO_HAND_PEACE_SIGN;
	}

	if(this->actionTimer < 52 * FRAME_RATE_SCALER_INV)
	{
		this->marioBodyState->eyeState = MARIO_EYES_HALF_CLOSED;
	}

	switch(this->actionTimer)
	{
		case 70 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 1;
			break;

		case 86 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 2;
			break;

		case 90 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 3;
			break;

		case 120 * FRAME_RATE_SCALER_INV:
			D_8032CBE4 = 0;
			break;

		case 140 * FRAME_RATE_SCALER_INV:
			Na_SeqVolRecover(0, 60);
			play_cutscene_music(SEQUENCE_ARGS(15, SEQ_EVENT_CUTSCENE_CREDITS));
			break;

		case 142 * FRAME_RATE_SCALER_INV:
			advance_cutscene_step();
			break;
	}
}

// dialog 3
// "Listen everybody"
// "let's bake a delicious cake..."
// "...for Mario..."
void PlayerRecord::end_peach_cutscene_dialog_3()
{
	this->set_mario_animation(MARIO_ANIM_FIRST_PERSON);

	sEndPeachObj->oPosY	= func_8025BC14(sEndPeachObj);
	sEndRightToadObj->oPosY = func_8025BC14(sEndRightToadObj);
	sEndLeftToadObj->oPosY	= func_8025BC14(sEndLeftToadObj);

	switch(this->actionTimer)
	{
		case 1 * FRAME_RATE_SCALER_INV:
			sEndPeachAnimation = 0;
			sEndToadAnims[0]   = 0;
			sEndToadAnims[1]   = 2;
			D_8032CBE8	   = 1;
			CallEndingDemoMessage(160, 227, 5, 30);
			AudStartSound(SOUND_PEACH_BAKE_A_CAKE, sEndPeachObj->header.gfx.cameraToObject);
			break;

		case 55 * FRAME_RATE_SCALER_INV:
			CallEndingDemoMessage(160, 227, 6, 40);
			break;

		case 130 * FRAME_RATE_SCALER_INV:
			CallEndingDemoMessage(160, 227, 7, 50);
			AudStartSound(SOUND_PEACH_FOR_MARIO, sEndPeachObj->header.gfx.cameraToObject);
			break;
	}

	if(this->actionTimer == 350 * FRAME_RATE_SCALER_INV)
	{
		advance_cutscene_step();
	}
}

// "Mario!"
void PlayerRecord::end_peach_cutscene_run_to_castle()
{
	this->set_mario_animation(this->actionState == 0 ? MARIO_ANIM_CREDITS_START_WALK_LOOK_UP : MARIO_ANIM_CREDITS_LOOK_BACK_THEN_RUN);

	this->marioObj->header.gfx.pos[1] = func_8025BC14(this->marioObj);

	if(this->actionState == 0 && this->is_anim_past_end())
	{
		this->actionState = 1;
	}

	if(this->actionTimer == 95 * FRAME_RATE_SCALER_INV)
	{
		CallEndingDemoMessage(160, 227, 0, 40);
		AudStartSound(SOUND_PEACH_MARIO2, sEndPeachObj->header.gfx.cameraToObject);
	}
	if(this->actionTimer == 389 * FRAME_RATE_SCALER_INV)
	{
		advance_cutscene_step();
	}
}

void PlayerRecord::end_peach_cutscene_fade_out()
{
	if(this->actionState == 0)
	{
		PL_StartFadeout(this, PL_FADE_STAFFROLL);
		gPaintingMarioYEntry = 1500.0f; // ensure medium water level in WDW credits cutscene
		this->actionState    = 1;
	}
}

enum
{
	END_PEACH_CUTSCENE_MARIO_FALLING,
	END_PEACH_CUTSCENE_MARIO_LANDING,
	END_PEACH_CUTSCENE_SUMMON_JUMBO_STAR,
	END_PEACH_CUTSCENE_SPAWN_PEACH,
	END_PEACH_CUTSCENE_DESCEND_PEACH,
	END_PEACH_CUTSCENE_RUN_TO_PEACH,
	END_PEACH_CUTSCENE_DIALOG_1,
	END_PEACH_CUTSCENE_DIALOG_2,
	END_PEACH_CUTSCENE_KISS_FROM_PEACH,
	END_PEACH_CUTSCENE_STAR_DANCE,
	END_PEACH_CUTSCENE_DIALOG_3,
	END_PEACH_CUTSCENE_RUN_TO_CASTLE,
	END_PEACH_CUTSCENE_FADE_OUT
};

s32 PlayerRecord::act_end_peach_cutscene()
{
	switch(this->actionArg)
	{
		case END_PEACH_CUTSCENE_MARIO_FALLING:
			end_peach_cutscene_mario_falling();
			break;
		case END_PEACH_CUTSCENE_MARIO_LANDING:
			end_peach_cutscene_mario_landing();
			break;
		case END_PEACH_CUTSCENE_SUMMON_JUMBO_STAR:
			end_peach_cutscene_summon_jumbo_star();
			break;
		case END_PEACH_CUTSCENE_SPAWN_PEACH:
			end_peach_cutscene_spawn_peach();
			break;
		case END_PEACH_CUTSCENE_DESCEND_PEACH:
			end_peach_cutscene_descend_peach();
			break;
		case END_PEACH_CUTSCENE_RUN_TO_PEACH:
			end_peach_cutscene_run_to_peach();
			break;
		case END_PEACH_CUTSCENE_DIALOG_1:
			end_peach_cutscene_dialog_1();
			break;
		case END_PEACH_CUTSCENE_DIALOG_2:
			end_peach_cutscene_dialog_2();
			break;
		case END_PEACH_CUTSCENE_KISS_FROM_PEACH:
			end_peach_cutscene_kiss_from_peach();
			break;
		case END_PEACH_CUTSCENE_STAR_DANCE:
			end_peach_cutscene_star_dance();
			break;
		case END_PEACH_CUTSCENE_DIALOG_3:
			end_peach_cutscene_dialog_3();
			break;
		case END_PEACH_CUTSCENE_RUN_TO_CASTLE:
			end_peach_cutscene_run_to_castle();
			break;
		case END_PEACH_CUTSCENE_FADE_OUT:
			end_peach_cutscene_fade_out();
			break;
	}

	this->actionTimer++;

	sEndCutsceneVp.vp.vscale[0] = 640;
	sEndCutsceneVp.vp.vscale[1] = 360;
	sEndCutsceneVp.vp.vtrans[0] = 640;
	sEndCutsceneVp.vp.vtrans[1] = 480;
	SnSetViewPort(NULL, &sEndCutsceneVp, 0, 0, 0);

	return FALSE;
}

s32 PlayerRecord::act_credits_cutscene()
{
	s32 width;
	s32 height;

	this->statusForCamera->cameraEvent = CAM_EVENT_START_CREDITS;
	// checks if mario is underwater (JRB, DDD, SA, etc.)
	if(this->pos[1] < this->waterLevel - 100)
	{
		if(this->area->camera->mode != CAMERA_MODE_BEHIND_MARIO)
		{
			set_camera_mode(this->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
		}
		this->set_mario_animation(MARIO_ANIM_WATER_IDLE);
		vec3f_copy(this->marioObj->header.gfx.pos, this->pos);
		// will copy over roll and pitch, if set
		vec3s_copy(this->marioObj->header.gfx.angle, this->faceAngle);
		this->particleFlags |= PARTICLE_WATER_BUBBLES;
	}
	else
	{
		this->set_mario_animation(MARIO_ANIM_FIRST_PERSON);
		if(this->actionTimer > 0)
		{
			stop_and_set_height_to_floor();
		}
	}

	if(this->actionTimer >= 61 * FRAME_RATE_SCALER_INV)
	{
		if(this->actionState < 40)
		{
			this->actionState += 2;
		}

		width  = this->actionState * 640 / 100;
		height = this->actionState * 480 / 100;

		sEndCutsceneVp.vp.vscale[0] = 640 - width;
		sEndCutsceneVp.vp.vscale[1] = 480 - height;
		sEndCutsceneVp.vp.vtrans[0] = (snEndingScene->info & 0x10 ? width : -width) * 56 / 100 + 640;
		sEndCutsceneVp.vp.vtrans[1] = (snEndingScene->info & 0x20 ? height : -height) * 66 / 100 + 480;

		SnSetViewPort(&sEndCutsceneVp, 0, 0, 0, 0);
	}

	if(this->actionTimer == 90 * FRAME_RATE_SCALER_INV)
	{
		ClearEndingMessage();
	}

	if(this->actionTimer >= 90 * FRAME_RATE_SCALER_INV)
	{
		sDispCreditsEntry = snEndingScene;
	}

	if(this->actionTimer++ == 200 * FRAME_RATE_SCALER_INV)
	{
		PL_StartFadeout(this, 24);
	}

	this->marioObj->header.gfx.angle[1] += ((snEndingScene->info & 0xC0) << 8) * FRAME_RATE_SCALER;

	return FALSE;
}

s32 PlayerRecord::act_end_waving_cutscene()
{
	if(this->actionState == 0)
	{
		this->statusForCamera->cameraEvent = CAM_EVENT_START_END_WAVING;

		sEndPeachObj = s_makeobj_absolute(gCurrentObject, 0, MODEL_PEACH, sm64::bhv::bhvEndPeach(), 60, 906, -1180, 0, 0, 0);

		sEndRightToadObj = s_makeobj_absolute(gCurrentObject, 0, MODEL_TOAD, sm64::bhv::bhvEndToad(), 180, 906, -1170, 0, 0, 0);

		sEndLeftToadObj = s_makeobj_absolute(gCurrentObject, 0, MODEL_TOAD, sm64::bhv::bhvEndToad(), -180, 906, -1170, 0, 0, 0);

		sEndPeachObj->oOpacity	   = 255;
		sEndRightToadObj->oOpacity = 255;
		sEndLeftToadObj->oOpacity  = 255;

		sEndPeachAnimation = 11;
		sEndToadAnims[0]   = 6;
		sEndToadAnims[1]   = 7;

		this->actionState = 1;
	}

	this->set_mario_animation(MARIO_ANIM_CREDITS_WAVING);
	stop_and_set_height_to_floor();

	this->marioObj->header.gfx.angle[1] += 0x8000;
	this->marioObj->header.gfx.pos[0] -= 60.0f * FRAME_RATE_SCALER;
	this->marioBodyState->handState = MARIO_HAND_RIGHT_OPEN;

	if(this->actionTimer++ == 300 * FRAME_RATE_SCALER_INV)
	{
		PL_StartFadeout(this, PL_FADE_GAMEEND);
	}

	return FALSE;
}

s32 PlayerRecord::check_for_instant_quicksand()
{
	if(this->floor->type == SURFACE_INSTANT_QUICKSAND && this->status & ACT_FLAG_INVULNERABLE && this->status != ACT_QUICKSAND_DEATH)
	{
		this->update_mario_sound_and_camera();
		return this->drop_and_set_mario_action(ACT_QUICKSAND_DEATH, 0);
	}
	return FALSE;
}

s32 PlayerRecord::mario_execute_cutscene_action()
{
	s32 cancel;

	if(check_for_instant_quicksand())
	{
		return TRUE;
	}

	/* clang-format off */
	switch(this->status)
	{
		case ACT_DISAPPEARED:                cancel = act_disappeared();                break;
		case ACT_INTRO_CUTSCENE:             cancel = act_intro_cutscene();             break;
		case ACT_STAR_DANCE_EXIT:            cancel = act_star_dance();                 break;
		case ACT_STAR_DANCE_NO_EXIT:         cancel = act_star_dance();                 break;
		case ACT_STAR_DANCE_WATER:           cancel = act_star_dance_water();           break;
		case ACT_FALL_AFTER_STAR_GRAB:       cancel = act_fall_after_star_grab();       break;
		case ACT_READING_AUTOMATIC_DIALOG:   cancel = act_reading_automatic_dialog();   break;
		case ACT_READING_NPC_DIALOG:         cancel = act_reading_npc_dialog();         break;
		case ACT_DEBUG_FREE_MOVE:            cancel = act_debug_free_move();            break;
		case ACT_READING_SIGN:               cancel = act_reading_sign();               break;
		case ACT_JUMBO_STAR_CUTSCENE:        cancel = act_jumbo_star_cutscene();        break;
		case ACT_WAITING_FOR_DIALOG:         cancel = act_waiting_for_dialog();         break;
		case ACT_STANDING_DEATH:             cancel = act_standing_death();             break;
		case ACT_QUICKSAND_DEATH:            cancel = act_quicksand_death();            break;
		case ACT_ELECTROCUTION:              cancel = act_electrocution();              break;
		case ACT_SUFFOCATION:                cancel = act_suffocation();                break;
		case ACT_DEATH_ON_STOMACH:           cancel = act_death_on_stomach();           break;
		case ACT_DEATH_ON_BACK:              cancel = act_death_on_back();              break;
		case ACT_EATEN_BY_BUBBA:             cancel = act_eaten_by_bubba();             break;
		case ACT_END_PEACH_CUTSCENE:         cancel = act_end_peach_cutscene();         break;
		case ACT_CREDITS_CUTSCENE:           cancel = act_credits_cutscene();           break;
		case ACT_END_WAVING_CUTSCENE:        cancel = act_end_waving_cutscene();        break;
		case ACT_PULLING_DOOR:
		case ACT_PUSHING_DOOR:               cancel = act_going_through_door();         break;
		case ACT_WARP_DOOR_SPAWN:            cancel = act_warp_door_spawn();            break;
		case ACT_EMERGE_FROM_PIPE:           cancel = act_emerge_from_pipe();           break;
		case ACT_SPAWN_SPIN_AIRBORNE:        cancel = act_spawn_spin_airborne();        break;
		case ACT_SPAWN_SPIN_LANDING:         cancel = act_spawn_spin_landing();         break;
		case ACT_EXIT_AIRBORNE:              cancel = act_exit_airborne();              break;
		case ACT_EXIT_LAND_SAVE_DIALOG:      cancel = act_exit_land_save_dialog();      break;
		case ACT_DEATH_EXIT:                 cancel = act_death_exit();                 break;
		case ACT_UNUSED_DEATH_EXIT:          cancel = act_unused_death_exit();          break;
		case ACT_FALLING_DEATH_EXIT:         cancel = act_falling_death_exit();         break;
		case ACT_SPECIAL_EXIT_AIRBORNE:      cancel = act_special_exit_airborne();      break;
		case ACT_SPECIAL_DEATH_EXIT:         cancel = act_special_death_exit();         break;
		case ACT_FALLING_EXIT_AIRBORNE:      cancel = act_falling_exit_airborne();      break;
		case ACT_UNLOCKING_KEY_DOOR:         cancel = act_unlocking_key_door();         break;
		case ACT_UNLOCKING_STAR_DOOR:        cancel = act_unlocking_star_door();        break;
		case ACT_ENTERING_STAR_DOOR:         cancel = act_entering_star_door();         break;
		case ACT_SPAWN_NO_SPIN_AIRBORNE:     cancel = act_spawn_no_spin_airborne();     break;
		case ACT_SPAWN_NO_SPIN_LANDING:      cancel = act_spawn_no_spin_landing();      break;
		case ACT_BBH_ENTER_JUMP:             cancel = act_bbh_enter_jump();             break;
		case ACT_BBH_ENTER_SPIN:             cancel = act_bbh_enter_spin();             break;
		case ACT_TELEPORT_FADE_OUT:          cancel = act_teleport_fade_out();          break;
		case ACT_TELEPORT_FADE_IN:           cancel = act_teleport_fade_in();           break;
		case ACT_SHOCKED:                    cancel = act_shocked();                    break;
		case ACT_SQUISHED:                   cancel = act_squished();                   break;
		case ACT_HEAD_STUCK_IN_GROUND:       cancel = act_head_stuck_in_ground();       break;
		case ACT_BUTT_STUCK_IN_GROUND:       cancel = act_butt_stuck_in_ground();       break;
		case ACT_FEET_STUCK_IN_GROUND:       cancel = act_feet_stuck_in_ground();       break;
		case ACT_PUTTING_ON_CAP:             cancel = act_putting_on_cap();             break;
	}
	/* clang-format on */

	if(!cancel)
	{
		if(this->input & INPUT_IN_WATER)
		{
			this->particleFlags |= PARTICLE_SURFACE_WAVES;
		}
	}

	return cancel;
}
