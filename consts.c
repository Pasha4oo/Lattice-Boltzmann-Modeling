#include "consts.h"

const int Nx = 800;
const int Ny = 400;
const int WINDOW_WIDTH = 1100;
const int WINDOW_HEIGHT = 710;

const int Nt = 30000;
const int NL = 9;

const int cxs[] = { 0, 0, 1, 1, 1, 0, -1, -1, -1 };
const int cys[] = { 0, 1, 1, 0, -1, -1, -1, 0, 1 };
const double weights[] = { 4.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0,
					1.0 / 9.0, 1.0 / 36.0, 1.0 / 9.0,
					1.0 / 36.0, 1.0 / 9.0, 1.0 / 36.0 };