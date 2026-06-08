#include "buttons_callback.h"

void cb_wall_circle(void* data) { ((Walls*)data)->wall_type = WALL_CIRCLE; }
void cb_wall_free(void* data) { ((Walls*)data)->wall_type = WALL_FREE; }
void cb_wall_line(void* data) { ((Walls*)data)->wall_type = WALL_LINE; }
void cb_wall_eraser(void* data) { ((Walls*)data)->wall_type = WALL_ERASER; }
void cb_wall_none(void* data) { ((Walls*)data)->wall_type = WALL_NONE; }

void cb_clear_walls(void* data) { init_walls(&((Walls*)data)->walls); }
void cb_load_bmp(void* data) { load_walls_bmp((Walls*)data); }
void cb_load_wall(void* data) { load_walls_pwal((Walls*)data); }
void cb_save_wall(void* data) { save_walls_pwal((const Walls*)data); }

void cb_pauser(void* data) { switch_pauser((Pauser*)data); }
void cb_restart_liquid(void* data) { init_lbm((LBM*)data); ((LBM*)data)->max_v = 1e-12; }

void cb_area_clogged(void* data) {
    AreaTypeData* d = (AreaTypeData*)data;
    d->set->area_type = AREA_CLOGGED;
    set_clogged_walls(d->ws);
    enable_light(d->light, 0);
}
void cb_area_cyclic(void* data) {
    AreaTypeData* d = (AreaTypeData*)data;
    if (d->set->area_type == AREA_CLOGGED) {
        remove_clogged_walls(d->ws);
    }
    d->set->area_type = AREA_CYCLIC;
    enable_light(d->light, 1);
}
void cb_area_outgoing(void* data) {
    AreaTypeData* d = (AreaTypeData*)data;
    if (d->set->area_type == AREA_CLOGGED) {
        remove_clogged_walls(d->ws);
    }
    d->set->area_type = AREA_OUTGOING;
    enable_light(d->light, 2);
}

void cb_load_all(void* data) { LoadAllData* d = (LoadAllData*)data; load_all_psim(d->ws, d->lbm, d->arr, d->set, d->light); }
void cb_save_all(void* data) { LoadAllData* d = (LoadAllData*)data; save_all_psim(d->ws, d->lbm, d->arr, d->set, d->light); }

void cb_export_csv(void* data) { export_velocity_csv((LBM*)data); }
void cb_fixate(void* data) { Settings* s = (Settings*)data; s->fixate = !s->fixate; }