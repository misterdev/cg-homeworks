
#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

/*
 * Copies a point
 * @param var - destination point
 * @param value - original point
 */
void assignToPoint(float* var, float* value) {
    var[0] = value[0];
    var[1] = value[1];
    var[2] = value[2];
}

/*
 * Linear interpolation
 * @param p1 - first point
 * @param p2 - second point
 * @param t - evaluates the curve in t (range 0-1)
 * @param ret - returned point
 */
void lerp( float* p1, float* p2, float t, float* ret) {
    ret[0] = ( (1-t) * p1[0] ) + ( t * p2[0]);
    ret[1] = ( (1-t) * p1[1] ) + ( t * p2[1]);
    ret[2] = ( (1-t) * p1[2] ) + ( t * p2[2]);
}