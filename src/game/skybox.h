#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "types.h"
typedef const u8 *const SkyboxTexture[80];

extern Gfx *create_skybox_facing_camera(s8 a, s8 background, f32 fov, f32 posX, f32 posY, f32 posZ, f32 focX, f32 focY, f32 focZ);

extern const SkyboxTexture bbh_skybox_ptrlist[];
extern const SkyboxTexture bidw_skybox_ptrlist[];
extern const SkyboxTexture bitfs_skybox_ptrlist[];
extern const SkyboxTexture bits_skybox_ptrlist[];
extern const SkyboxTexture ccm_skybox_ptrlist[];
extern const SkyboxTexture cloud_floor_skybox_ptrlist[];
extern const SkyboxTexture clouds_skybox_ptrlist[];
extern const SkyboxTexture ssl_skybox_ptrlist[];
extern const SkyboxTexture water_skybox_ptrlist[];
extern const SkyboxTexture wdw_skybox_ptrlist[];

#endif /* _SKYBOX_H */
