#ifndef RENDER_COLOR_H
#define RENDER_COLOR_H

#include <stdint.h>
#include <wlr/util/addon.h>

enum wlr_color_transform_type {
	COLOR_TRANSFORM_SRGB,
	COLOR_TRANSFORM_LUT_3D,
};

struct wlr_color_transform {
	int ref_count;
	struct wlr_addon_set addons; // per-renderer helper state

	enum wlr_color_transform_type type;
};

/**
 * The formula is approximated via a 3D look-up table. A 3D LUT is a
 * three-dimensional array where each element is an RGB triplet. The flat lut_3d
 * array has a length of dim_len³.
 *
 * Color channel values in the range [0.0, 1.0] are mapped linearly to
 * 3D LUT indices such that 0.0 maps exactly to the first element and 1.0 maps
 * exactly to the last element in each dimension.
 *
 * The offset of the RGB triplet given red, green and blue indices r_index,
 * g_index and b_index is:
 *
 *     offset = 3 * (r_index + dim_len * g_index + dim_len * dim_len * b_index)
 */
struct wlr_color_transform_lut3d {
	struct wlr_color_transform base;

	float *lut_3d;
	size_t dim_len;
};

/**
 * Gets a wlr_color_transform_lut3d from a generic wlr_color_transform.
 * Asserts that the base type is COLOR_TRANSFORM_LUT_3D
 */
struct wlr_color_transform_lut3d *wlr_color_transform_lut3d_from_base(
	struct wlr_color_transform *tr);

#endif
