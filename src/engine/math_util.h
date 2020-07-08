#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_
#include "types.h"

/*
 * The sine and cosine tables overlap, but "#define gCosineTable (gSineTable +
 * 0x400)" doesn't give expected codegen; gSineTable and gCosineTable need to
 * be different symbols for code to match. Most likely the tables were placed
 * adjacent to each other, and gSineTable cut short, such that reads overflow
 * into gCosineTable.
 *
 * These kinds of out of bounds reads are undefined behavior, and break on
 * e.g. GCC (which doesn't place the tables next to each other, and probably
 * exploits array sizes for range analysis-based optimizations as well).
 * Thus, for non-IDO compilers we use the standard-compliant version.
 */
extern f32 gSineTable[];
#ifdef AVOID_UB
#define gCosineTable (gSineTable + 0x400)
#else
extern f32 gCosineTable[];
#endif

#define sins(x) gSineTable[(u16) (x) >> 4]
#define coss(x) gCosineTable[(u16) (x) >> 4]

//#define min(a, b) ((a) <= (b) ? (a) : (b))
//#define max(a, b) ((a) > (b) ? (a) : (b))

#define sqr(x) ((x) * (x))

Vec3f* vec3f_copy(Vec3f& dest, const Vec3f src);
void vec3f_pcopy(Vec3f* dest, const Vec3f src);
Vec3f* vec3f_set(Vec3f& dest, const f32 x, const f32 y, const f32 z);
Vec3f* vec3f_add(Vec3f& dest, const Vec3f a);
Vec3f* vec3f_sum(Vec3f& dest, const Vec3f a, const Vec3f b);
Vec3s* vec3s_copy(Vec3s& dest, const Vec3s src);
Vec3s* vec3s_set(Vec3s& dest, const s16 x, const s16 y, const s16 z);
short* short3_set(short* dest, const s16 x, const s16 y, const s16 z);
Vec3s* vec3s_add(Vec3s& dest, const Vec3s a);
Vec3s* vec3s_sum(Vec3s& dest, const Vec3s a, const Vec3s b);
Vec3s* vec3s_sub(Vec3s& dest, const Vec3s a);
Vec3f* vec3s_to_vec3f(Vec3f& dest, const Vec3s a);
Vec3s* vec3f_to_vec3s(Vec3s& dest, const Vec3f a);
Vec3f* find_vector_perpendicular_to_plane(Vec3f& dest, const Vec3f a, const Vec3f b, const Vec3f c);
Vec3f* vec3f_cross(Vec3f& dest, const Vec3f a, const Vec3f b);
Vec3f* vec3f_normalize(Vec3f& dest);

void mtxf_copy(Mat4& dest, const Mat4 src);
void mtxf_identity(Mat4& mtx);
void mtxf_translate(Mat4& dest, const Vec3f b);
void mtxf_lookat(Mat4& mtx, const Vec3f from, const Vec3f to, const s16 roll);
void mtxf_rotate_zxy_and_translate(Mat4& dest, const Vec3f translate, const Vec3s rotate);
void mtxf_rotate_xyz_and_translate(Mat4& dest, const Vec3f b, const Vec3s c);
void mtxf_billboard(Mat4& dest, const Mat4 mtx, const Vec3f position, const s16 angle);
void mtxf_align_terrain_normal(Mat4& dest, const Vec3f upDir, const Vec3f pos, const s16 yaw);
void mtxf_align_terrain_triangle(Mat4& mtx, const Vec3f pos, const s16 yaw, const f32 radius);
void mtxf_mul(Mat4& dest, const Mat4 a, const Mat4 b);
void mtxf_scale_vec3f(Mat4& dest, const Mat4 mtx, const Vec3f s);
void mtxf_mul_vec3s(const Mat4 mtx, Vec3s& b);
void mtxf_to_mtx(Mtx *dest, const Mat4 src);
void mtxf_rotate_xy(Mtx *a, s16 b);
void get_pos_from_transform_mtx(Vec3f& dest, const Mat4 objMtx, const Mat4 camMtx);
void vec3f_get_dist_and_angle(const Vec3f from, const Vec3f to, f32 *dist, s16 *pitch, s16 *yaw);
void vec3f_set_dist_and_angle(Vec3f from, Vec3f to, f32  dist, s16  pitch, s16  yaw);
s32 approach_s32(const s32 current, const s32 target, const s32 inc, const s32 dec);
f32 approach_f32(const f32 current, const f32 target, const f32 inc, const f32 dec);
s16 atan2s(f32 a, f32 b);
f32 atan2f(f32 a, f32 b);
void spline_get_weights(Vec4f& result, const f32 t, const s32 c);
void anim_spline_init(Vec4s* a);
s32 anim_spline_poll(Vec3f& a);

float scale_scaler(float n, float scale);

#endif
