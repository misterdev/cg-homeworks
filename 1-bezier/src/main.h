#ifndef INC_1_BEZIER_MAIN_H
#define INC_1_BEZIER_MAIN_H

#define MAX_CV 64               // Max number of control points
#define DIM 3                   // Point dimension (x, y, z)

typedef enum { false, true } bool;

void removeFirstPoint ();
void removeLastPoint ();
void addNewPoint (float x, float y);

void assignToPoint(float* var, float* value);
void lerp( float* p1, float* p2, float t, float* ret);

void deCasteljau (float t);
void adaptiveSubdivision(float cps[MAX_CV][DIM], float tolerance);

#endif
