#ifndef HUD_H
#define HUD_H

#include "types.h"

enum PowerMeterAnimation
{
	POWER_METER_HIDDEN,
	POWER_METER_EMPHASIZED,
	POWER_METER_DEEMPHASIZING,
	POWER_METER_HIDING,
	POWER_METER_VISIBLE
};

// Segment 3
#include "actors/common1.h"
/*
extern u8 *power_meter_health_segments_lut[];
extern Gfx* dl_power_meter_base;
extern Gfx* dl_power_meter_health_segments_begin;
extern Gfx* dl_power_meter_health_segments_end;*/

// Functions
extern void CameraIconSet(s16 status);
extern void MeterGaugeEvent(void);

#endif /* HUD_H */
