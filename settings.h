#pragma once

#include <raylib.h>

typedef struct Walls Walls;
typedef struct LBM LBM;
typedef struct Arrow Arrow;
typedef struct LightSystem LightSystem;

typedef enum AreaType {
	AREA_CLOGGED,
	AREA_CYCLIC,
	AREA_OUTGOING
} AreaType;

typedef struct Settings {
	AreaType area_type;
	float flow_speed;
	double tau;
	bool fixate;
} Settings;

extern const int TAU_BASE_LENGTH;
extern const int TAU_BASE_HIGHT;

void load_all_psim(Walls* ws, LBM* lbm, Arrow* arrow, Settings* settings, LightSystem* light_system);
void save_all_psim(Walls* ws, LBM* lbm, Arrow* arrow, Settings* settings, LightSystem* light_system);
void export_velocity_csv(LBM* lbm);