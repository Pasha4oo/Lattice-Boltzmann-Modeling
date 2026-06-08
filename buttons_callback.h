#pragma once

#include "walls.h"
#include "flow.h"
#include "settings.h"
#include "pauser.h"
#include "arrow.h"
#include "lights.h"

typedef struct { Walls* ws; Settings* set; LightSystem* light; } AreaTypeData;
typedef struct { Walls* ws; LBM* lbm; Arrow* arr; Settings* set; LightSystem* light; } LoadAllData;

void cb_wall_circle(void* data);
void cb_wall_free(void* data);
void cb_wall_line(void* data);
void cb_wall_eraser(void* data);
void cb_wall_none(void* data);

void cb_clear_walls(void* data);
void cb_load_bmp(void* data);
void cb_load_wall(void* data);
void cb_save_wall(void* data);

void cb_pauser(void* data);
void cb_restart_liquid(void* data);

void cb_area_clogged(void* data);
void cb_area_cyclic(void* data);
void cb_area_outgoing(void* data);

void cb_load_all(void* data);
void cb_save_all(void* data);

void cb_export_csv(void* data);
void cb_fixate(void* data);