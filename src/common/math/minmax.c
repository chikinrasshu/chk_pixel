#include "minmax.h"

int chk_min(int a, int b) { return (a < b) ? a : b; }
int chk_max(int a, int b) { return (a > b) ? a : b; }
int chk_abs(int v) { return (v >= 0) ? v : -v; }

float chk_minf(float a, float b) { return (a < b) ? a : b; }
float chk_maxf(float a, float b) { return (a > b) ? a : b; }
float chk_absf(float v) { return (v >= 0) ? v : -v; }

double chk_mind(double a, double b) { return (a < b) ? a : b; }
double chk_maxd(double a, double b) { return (a > b) ? a : b; }
double chk_absd(double v) { return (v >= 0) ? v : -v; }
