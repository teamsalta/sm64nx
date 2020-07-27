#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "types.h"

#define GFX_POOL_SIZE 6400

struct GfxPool
{
	Gfx buffer[GFX_POOL_SIZE];
	struct SPTask spTask;
};

extern u16 frameBufferIndex;
extern u32 frameCounter;

// extern ? my_rdp_init(?);
// extern ? my_rsp_init(?);
// extern ? clear_z_buffer(?);
// extern ? display_frame_buffer(?);
extern void SoftwareBlanking(s32);
extern void SoftwareTrimBlanking(Vp*, s32);
// extern ? draw_screen_borders(?);
void SetTrimmingRectangle(Vp* viewport);
extern void init_render_image(void);
extern void end_master_display_list(void);
extern void func_80247D84(void);
extern void func_80247ED8(void);
extern void func_80247FAC(void);
extern void display_and_vsync(void);

#endif /* _DISPLAY_H */
