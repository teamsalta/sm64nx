#include <ultra64.h>

#include "sm64.h"
#include "engine/behavior_script.h"
#include "object_helpers.h"
#include "audio/external.h"
#include "spawn_sound.h"
#include "object_list_processor.h"
#include "behavior_data.h"
#include "game/motor.h"

/*
 * execute an object's current sound state with a provided array
 * of sound states. Used for the stepping sounds of various
 * objects. (King Bobomb, Bowser, King Whomp)
 */
void s_enemysound(struct SoundState* soundStates)
{
	s32 stateIdx = gCurrentObject->oSoundStateID;

	switch(soundStates[stateIdx].playSound)
	{
		// since we have an array of sound states cooresponding to
		// various behaviors, not all entries intend to play sounds. the
		// boolean being 0 for size entries skips these states.
		case FALSE:
			break;
		case TRUE:
		{
			s32 animFrame;

			// in the sound state information, -1 (0xFF) is for empty
			// animFrame entries. These checks skips them.
			if((animFrame = soundStates[stateIdx].animFrame1) >= 0)
			{
				if(s_check_animenumber(animFrame))
				{
					objsound(soundStates[stateIdx].soundMagic);
				}
			}

			if((animFrame = soundStates[stateIdx].animFrame2) >= 0)
			{
				if(s_check_animenumber(animFrame))
				{
					objsound(soundStates[stateIdx].soundMagic);
				}
			}
		}
		break;
	}
}

/*
 * Create a sound spawner for objects that need a sound play once.
 * (Breakable walls, King Bobomb exploding, etc)
 */
void obj_remove_sound(s32 soundMagic)
{
	struct Object* obj = s_makeobj_nowpos(gCurrentObject, 0, sm64::bhv::bhvSoundSpawner());

	obj->oSoundEffectUnkF4 = soundMagic;
}

/*
 * The following 2 functions are relevent to the sound state function
 * above. While only objsound is used, they may have been intended as
 * seperate left/right leg functions that went size.
 */
void objsound_level(s32 soundMagic)
{
	if(gCurrentObject->header.gfx.node.flags & 0x0001)
	{
		AudStartSound(soundMagic, gCurrentObject->header.gfx.cameraToObject);
	}
}

// duplicate function, but its the used one
void objsound(s32 soundMagic)
{
	if(gCurrentObject->header.gfx.node.flags & 0x0001)
	{
		AudStartSound(soundMagic, gCurrentObject->header.gfx.cameraToObject);

		switch(soundMagic)
		{
			case NA_SE3_KUPAWALK:
				SendMotorEvent(3, 60);
				break;
			case SOUND_OBJ_POUNDING_LOUD:
				SendMotorEvent(3, 60);
				break;
			case SOUND_OBJ_WHOMP_LOWPRIO:
				SendMotorEvent(5, 80);
				break;
		}
	}
}

/*
 * These 2 functions below are completely unreferenced in all versions
 * of Super Mario 64. They are likely functions which facilitated
 * calculation of distance of an object to volume, since these are
 * common implementations of such a concept, and the fact they are
 * adjacent to other sound functions. The fact there are 2 functions
 * might show that the developers were testing several ranges, or certain
 * objects had different ranges, or had these for other unknown purposes.
 * Technically, these functions are only educated guesses. Trust these
 * interpretations at your own discrection.
 */
int calc_dist_to_volume_range_1(f32 distance) // range from 60-124
{
	s32 volume;

	if(distance < 500.0f)
	{
		volume = 127;
	}
	else if(1500.0f < distance)
	{
		volume = 0;
	}
	else
	{
		volume = (((distance - 500.0f) / 1000.0f) * 64.0f) + 60.0f;
	}

	return volume;
}

int calc_dist_to_volume_range_2(f32 distance) // range from 79.2-143.2
{
	s32 volume;

	if(distance < 1300.0f)
	{
		volume = 127;
	}
	else if(2300.0f < distance)
	{
		volume = 0;
	}
	else
	{
		volume = (((distance - 1000.0f) / 1000.0f) * 64.0f) + 60.0f;
	}

	return volume;
}
