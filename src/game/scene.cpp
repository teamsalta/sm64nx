#include <ultra64.h>

#include "area.h"
#include "sm64.h"
#include "gfx_dimensions.h"
#include "behavior_data.h"
#include "game.h"
#include "display.h"
#include "object_list_processor.h"
#include "engine/surface_load.h"
#include "ingame_menu.h"
#include "screen_transition.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "print.h"
#include "hud.h"
#include "audio/external.h"
#include "area.h"
#include "rendering_graph_node.h"
#include "level_update.h"
#include "engine/geo_layout.h"
#include "save_file.h"

#include "gfx_dimensions.h"

SpawnInfo starringActor[1];
GraphNode* shapeList[0x100];
Area sceneList[8];

WarpTransition wipeControl;

s16 activeCourseNo;
s16 activeLevelNo;
s16 activeSceneNo;
s16 ramSaveCourse;
s16 mesgEvent;
s16 mesgLatch;

SpawnInfo* marioActor = &starringActor[0];

GraphNode** stageShapes = shapeList;
Area* stageScenes	= sceneList;

Area* snSceneInfo	    = NULL;
CreditsEntry* snEndingScene = NULL;

Vp* snViewPort		 = NULL;
Vp* snTrimming		 = NULL;
s16 faderDelay		 = 0;
u32 backColor		 = 0;
u32 gWarpTransFBSetColor = 0;
u8 gWarpTransRed	 = 0;
u8 gWarpTransGreen	 = 0;
u8 gWarpTransBlue	 = 0;

s16 activePlayerNo = 1;
s16 activeStageNo  = 1;

const BehaviorScript* pathNameList[] = {
    sm64::bhv::bhvDoorWarp(), sm64::bhv::bhvStar(),    sm64::bhv::bhvExitPodiumWarp(), sm64::bhv::bhvWarp(),	sm64::bhv::bhvWarpPipe(), sm64::bhv::bhvFadingWarp(), sm64::bhv::bhvWarps60(),
    sm64::bhv::bhvWarps64(),  sm64::bhv::bhvWarps68(), sm64::bhv::bhvWarps6C(),	       sm64::bhv::bhvWarps70(), sm64::bhv::bhvWarps74(),  sm64::bhv::bhvWarps78(),    sm64::bhv::bhvWarps94(),
    sm64::bhv::bhvWarps7C(),  sm64::bhv::bhvWarps80(), sm64::bhv::bhvWarps88(),	       sm64::bhv::bhvWarps84(), sm64::bhv::bhvWarps8C(),  sm64::bhv::bhvWarps90(),
};

u8 portNameList[] = {
    SN_TRIP_DOOR,     SN_TRIP_STAR,	SN_TRIP_CHIMNEY, SN_TRIP_CHIMNEY,   SN_TRIP_CHIMNEY, SN_WARP_POINT,  SN_ENTER_WAITING,	  SN_ENTER_LANDING,   SN_ENTER_FALLING, SN_ENTER_ROLLING,
    SN_ENTER_DOWNING, SN_ENTER_PICTURE, SN_ENTER_FLIGHT, SN_ENTER_SWIMMING, SN_ENTER_WINNER, SN_ENTER_LOSER, SN_ENTER_LANDWINNER, SN_ENTER_LANDLOSER, SN_ENTER_PUSHOUT, SN_ENTER_PUSHDOWN,
};

Vp viewport = {{
    {640, 480, 511, 0},
    {640, 480, 511, 0},
}};

void SnSetViewPort(Vp* a, Vp* b, u8 c, u8 d, u8 e)
{
	u16 color = ((c >> 3) << 11) | ((d >> 3) << 6) | ((e >> 3) << 1) | 1;

	backColor  = (color << 16) | color;
	snViewPort = a;
	snTrimming = b;
}

static void SnSetBlankColor(u8 red, u8 green, u8 blue)
{
	u16 warpTransitionRGBA16 = ((red >> 3) << 11) | ((green >> 3) << 6) | ((blue >> 3) << 1) | 1;

	gWarpTransFBSetColor = (warpTransitionRGBA16 << 16) | warpTransitionRGBA16;
	gWarpTransRed	     = red;
	gWarpTransGreen	     = green;
	gWarpTransBlue	     = blue;
}

static int scale_x_to_correct_aspect_center(int x)
{
	f32 aspect = GFX_DIMENSIONS_ASPECT_RATIO;
	return x + (SCREEN_HEIGHT * aspect / 2) - (SCREEN_WIDTH / 2);
}

void print_intro_text(void)
{
	if(((frameCounter / FRAME_RATE_SCALER_INV) & 0x1F) < 20)
	{
		if(sm64::players().size() == 0)
		{
			dcprintf(scale_x_to_correct_aspect_center(SCREEN_WIDTH / 2), 20, "NO CONTROLLER");
		}
		else
		{
			dcprintf(60, 38, "PRESS");
			dcprintf(60, 20, "START");
		}
	}
}

void SnDisplayDemoMessage(void)
{
	if(sm64::players().size() == 0)
	{
		dcprintf(160, 20, "NO CONTROLLER");
	}
	else
	{
		dcprintf(160, 164, "SUPER MARIO 64 NX");
		dcprintf(160, 58, "PRESS START");
		dmprintf(0, 30, "V8");
	}
}

u32 SnGetPortType(Object* o)
{
	s32 i;
	const BehaviorScript* behavior = (const BehaviorScript*)virtual_to_segmented(0x13, o->behavior);

	for(i = 0; i < 20; i++)
	{
		if(pathNameList[i] == behavior)
		{
			return portNameList[i];
		}
	}
	return 0;
}

struct ObjectWarpNode* SnGetPortPtr(u8 code)
{
	ObjectWarpNode* node = NULL;

	for(node = snSceneInfo->warpNodes; node != NULL; node = node->next)
	{
		if(node->node.id == code)
		{
			break;
		}
	}
	return node;
}

ObjectWarpNode* SnFindPortPtr(struct Object* o)
{
	u8 code = (o->oBehParams & 0x00FF0000) >> 16;

	return SnGetPortPtr(code);
}

static void InitPorts(void)
{
	ObjectWarpNode* port;
	Object* node = (Object*)strategyGroup.children;

	do
	{
		Object* stratp = node;

		if(stratp->activeFlags && SnGetPortType(stratp) != 0)
		{
			port = SnFindPortPtr(stratp);
			if(port != NULL)
			{
				port->object = stratp;
			}
		}
	} while((node = (struct Object*)node->header.gfx.node.next) != (struct Object*)strategyGroup.children);
}

void clear_areas(void)
{
	s32 count;

	snSceneInfo		   = NULL;
	wipeControl.isActive	   = FALSE;
	wipeControl.pauseRendering = FALSE;
	marioActor->areaIndex	   = -1;

	for(count = 0; count < 8; count++)
	{
		sceneList[count].index		   = count;
		sceneList[count].flags		   = 0;
		sceneList[count].terrainType	   = 0;
		sceneList[count].unk04		   = NULL;
		sceneList[count].terrainData	   = NULL;
		sceneList[count].surfaceRooms	   = NULL;
		sceneList[count].macroObjects	   = NULL;
		sceneList[count].warpNodes	   = NULL;
		sceneList[count].paintingWarpNodes = NULL;
		sceneList[count].instantWarps	   = NULL;
		sceneList[count].objectSpawnInfos  = NULL;
		sceneList[count].camera		   = NULL;
		sceneList[count].unused28	   = NULL;
		sceneList[count].whirlpools[0]	   = NULL;
		sceneList[count].whirlpools[1]	   = NULL;
		sceneList[count].dialog[0]	   = 255;
		sceneList[count].dialog[1]	   = 255;
		sceneList[count].musicParam	   = 0;
		sceneList[count].musicParam2	   = 0;
	}
}

void SnDisposeScene(void)
{
	s32 count;

	if(snSceneInfo != NULL)
	{
		MapSendMessage(snSceneInfo->unk04, GEO_CONTEXT_AREA_UNLOAD);
		snSceneInfo	     = NULL;
		wipeControl.isActive = FALSE;
	}

	for(count = 0; count < 8; count++)
	{
		if(sceneList[count].unk04 != NULL)
		{
			MapSendMessage(sceneList[count].unk04, GEO_CONTEXT_AREA_INIT);
			sceneList[count].unk04 = NULL;
		}
	}
}

void SnOpenScene(s32 index)
{
	if(snSceneInfo == NULL && sceneList[index].unk04 != NULL)
	{
		snSceneInfo   = &sceneList[index];
		activeSceneNo = snSceneInfo->index;

		if(snSceneInfo->terrainData != NULL)
		{
			mcInitBGCheck(index, snSceneInfo->terrainData, snSceneInfo->surfaceRooms, snSceneInfo->macroObjects);
		}

		if(snSceneInfo->objectSpawnInfos != NULL)
		{
			SetStrategy(0, snSceneInfo->objectSpawnInfos);
		}

		InitPorts();
		MapSendMessage(snSceneInfo->unk04, GEO_CONTEXT_AREA_LOAD);
	}
}

void SnCloseScene(void)
{
	if(snSceneInfo != NULL)
	{
		RemoveStrategy(0, snSceneInfo->index);
		MapSendMessage(snSceneInfo->unk04, GEO_CONTEXT_AREA_UNLOAD);

		snSceneInfo->flags   = 0;
		snSceneInfo	     = NULL;
		wipeControl.isActive = FALSE;
	}
}

void SnEnterPlayer(void)
{
	Na_LevelSeAllStop();
	SnOpenScene(marioActor->areaIndex);

	if(snSceneInfo->index == marioActor->areaIndex)
	{
		snSceneInfo->flags |= 0x01;
		SetStrategy(0, marioActor);
	}
}

void SnExitPlayer(void)
{
	if(snSceneInfo != NULL && (snSceneInfo->flags & 0x01))
	{
		RemoveStrategy(0, marioActor->activeAreaIndex);

		snSceneInfo->flags &= ~0x01;
		if(snSceneInfo->flags == 0)
		{
			SnCloseScene();
		}
	}
}

void SnChangeScene(s32 index)
{
	s32 entrant = snSceneInfo->flags;

	if(activeSceneNo != index)
	{
		SnCloseScene();
		SnOpenScene(index);

		snSceneInfo->flags		   = entrant;
		gMarioObject->oActiveParticleFlags = 0;
	}

	if(entrant & 0x01)
	{
		gMarioObject->header.gfx.unk18 = index, marioActor->areaIndex = index;
	}
}

void SnExecuteStrategy(void)
{
	animationCounter++;
	update_objects(0);
}

/*
 * Sets up the information needed to play a warp transition, including the
 * transition type, time in frames, and the RGB color that will fill the screen.
 */
void SnStartFader(s16 mode, s16 time, u8 red, u8 green, u8 blue)
{
	wipeControl.isActive	   = TRUE;
	wipeControl.type	   = mode;
	wipeControl.time	   = time * FRAME_RATE_SCALER_INV;
	wipeControl.pauseRendering = FALSE;

	// The lowest bit of mode determines if the transition is fading in or out.
	if(mode & 1)
	{
		SnSetBlankColor(red, green, blue);
	}
	else
	{
		red = gWarpTransRed, green = gWarpTransGreen, blue = gWarpTransBlue;
	}

	if(mode < 8)
	{ // if transition is RGB
		wipeControl.data.red   = red;
		wipeControl.data.green = green;
		wipeControl.data.blue  = blue;
	}
	else
	{ // if transition is textured
		wipeControl.data.red   = red;
		wipeControl.data.green = green;
		wipeControl.data.blue  = blue;

		// Both the start and end textured transition are always located in the middle of the screen.
		// If you really wanted to, you could place the start at one corner and the end at
		// the opposite corner. This will make the transition image look like it is moving
		// across the screen.
		wipeControl.data.startTexX = SCREEN_WIDTH / 2;
		wipeControl.data.startTexY = SCREEN_HEIGHT / 2;
		wipeControl.data.endTexX   = SCREEN_WIDTH / 2;
		wipeControl.data.endTexY   = SCREEN_HEIGHT / 2;

		wipeControl.data.texTimer = 0;

		if(mode & 1) // Is the image fading in?
		{
			wipeControl.data.startTexRadius = SCREEN_WIDTH;
			if(mode >= 0x0F)
			{
				wipeControl.data.endTexRadius = 16;
			}
			else
			{
				wipeControl.data.endTexRadius = 0;
			}
		}
		else // The image is fading out. (Reverses start & end circles)
		{
			if(mode >= 0x0E)
			{
				wipeControl.data.startTexRadius = 16;
			}
			else
			{
				wipeControl.data.startTexRadius = 0;
			}
			wipeControl.data.endTexRadius = SCREEN_WIDTH;
		}
	}
}

/*
 * Sets up the information needed to play a warp transition, including the
 * transition type, time in frames, and the RGB color that will fill the screen.
 * The transition will play only after a number of frames specified by 'delay'
 */
void SnDelayFader(s16 transType, s16 time, u8 red, u8 green, u8 blue, s16 delay)
{
	faderDelay = delay * FRAME_RATE_SCALER_INV; // Number of frames to delay playing the transition.
	SnStartFader(transType, time, red, green, blue);
}

void SnDrawScreen(void)
{
	if(snSceneInfo != NULL && !wipeControl.pauseRendering)
	{
		DrawScene((GraphNodeRoot*)snSceneInfo->unk04, snViewPort, snTrimming, backColor);

		gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&viewport));

		gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, BORDER_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - BORDER_HEIGHT);
		MeterGaugeEvent();

		gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		DrawMessage();
		EndingDemoMessageEvent();
		DrawStaffRoll();
		gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, BORDER_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - BORDER_HEIGHT);
		mesgEvent = MessageEvent();

		if(mesgEvent != 0)
		{
			mesgLatch = mesgEvent;
		}

		if(snTrimming != NULL)
		{
			SetTrimmingRectangle(snTrimming);
		}
		else
			gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, BORDER_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - BORDER_HEIGHT);

		if(wipeControl.isActive)
		{
			if(faderDelay == 0)
			{
				wipeControl.isActive = !Wipe(0, wipeControl.type, wipeControl.time, &wipeControl.data);

				if(!wipeControl.isActive)
				{
					if(wipeControl.type & 1)
					{
						wipeControl.pauseRendering = TRUE;
					}
					else
					{
						SnSetBlankColor(0, 0, 0);
					}
				}
			}
			else
			{
				faderDelay--;
			}
		}
	}
	else
	{
		DrawMessage();
		if(snTrimming != 0)
		{
			SoftwareTrimBlanking(snTrimming, gWarpTransFBSetColor);
		}
		else
		{
			SoftwareBlanking(gWarpTransFBSetColor);
		}
	}

	snViewPort = NULL;
	snTrimming = 0;
}
