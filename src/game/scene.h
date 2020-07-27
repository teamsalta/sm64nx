#pragma once

#define SN_TRIP_DOOR 0x01
#define SN_TRIP_STAR 0x02
#define SN_TRIP_CHIMNEY 0x03
#define SN_WARP_POINT 0x04
#define SN_ENTER_WAITING 0x10
#define SN_ENTER_SWIMMING 0x11
#define SN_ENTER_LANDING 0x12
#define SN_ENTER_FALLING 0x13
#define SN_ENTER_ROLLING 0x14
#define SN_ENTER_DOWNING 0x15
#define SN_ENTER_PICTURE 0x16
#define SN_ENTER_FLIGHT 0x17
#define SN_ENTER_WINNER 0x20
#define SN_ENTER_LOSER 0x21
#define SN_ENTER_LANDWINNER 0x22
#define SN_ENTER_LANDLOSER 0x23
#define SN_ENTER_PUSHOUT 0x24
#define SN_ENTER_PUSHDOWN 0x25
#define SN_ENTER_ENDING 0x26
#define SN_ENTER_STAFFROLL 0x27

struct CreditsEntry
{
	/*0x00*/ u8 stage;
	/*0x01*/ u8 scene;
	/*0x02*/ u8 info;
	/*0x03*/ s8 marioAngle;
	/*0x04*/ Vec3s marioPos;
	/*0x0C*/ const char** staff;
};
