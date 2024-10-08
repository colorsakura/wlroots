#include <assert.h>
#include <stdlib.h>
#include <wlr/render/color.h>
#include "render/color.h"

struct wlr_color_transform *wlr_color_transform_init_srgb(void) {
	struct wlr_color_transform *tx = calloc(1, sizeof(struct wlr_color_transform));
	if (!tx) {
		return NULL;
	}
	tx->type = COLOR_TRANSFORM_SRGB;
	tx->ref_count = 1;
	wlr_addon_set_init(&tx->addons);
	return tx;
}

static void color_transform_destroy(struct wlr_color_transform *tr) {
	switch (tr->type) {
	case COLOR_TRANSFORM_SRGB:
		break;
	case COLOR_TRANSFORM_LUT_3D:;
		struct wlr_color_transform_lut3d *lut3d =
			wlr_color_transform_lut3d_from_base(tr);
		free(lut3d->lut_3d);
		break;
	}
	wlr_addon_set_finish(&tr->addons);
	free(tr);
}

struct wlr_color_transform *wlr_color_transform_ref(struct wlr_color_transform *tr) {
	tr->ref_count += 1;
	return tr;
}

void wlr_color_transform_unref(struct wlr_color_transform *tr) {
	if (!tr) {
		return;
	}
	assert(tr->ref_count > 0);
	tr->ref_count -= 1;
	if (tr->ref_count == 0) {
		color_transform_destroy(tr);
	}
}

struct wlr_color_transform_lut3d *wlr_color_transform_lut3d_from_base(
		struct wlr_color_transform *tr) {
	assert(tr->type == COLOR_TRANSFORM_LUT_3D);
	struct wlr_color_transform_lut3d *lut3d = wl_container_of(tr, lut3d, base);
	return lut3d;
}
