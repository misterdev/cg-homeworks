/*
 * ConnectDots.c
 *
 *     This program draws straight lines connecting dots placed with mouse clicks.
 *
 *
 * Usage:
 *   Left click to place a control point.
 *		Maximum number of control points allowed is currently set at 64.
 *	 Press "f" to remove the first control point
 *	 Press "l" to remove the last control point.
 *	 Press escape to exit.
 */

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

typedef enum { OPENGL, DECASTELJAU, ADAPTIVE } draw_mode;
draw_mode DRAW_MODE = ADAPTIVE; // EDIT THIS TO CHANGE DRAW MODE

float CV[MAX_CV][DIM];          // Control points
int numCV = 0;                  // Number of control points
int WindowHeight, WindowWidth;  // Window size in pixels
int clickedPoint;               // Clicked point to drag



/**********************
 *     POINT UTILS
 **********************/

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

/*
 * Calculate the distance between points
 * @param p1 - point 1
 * @param p2 - point 2
 */
float point2pointDistance(float p1[DIM], float p2[DIM]) {
    return sqrtf(powf(p1[0] - p2[0], 2) + powf(p1[1] - p2[1], 2));
}

/*
 * Evaulate the distance of a point from a line
 * @param p0 - point
 * @param p1, p2 - extremity of segment
 */
float point2LineDistance(float p0[DIM], float p1[DIM], float p2[DIM]) {
    return fabsf(
            ( (p2[0] - p1[0]) * (p1[1] - p0[1]) ) // (x2 - x1) * (y1 - y0)
            - ( (p1[0] - p0[0] ) * (p2[1] - p1[1]) ) // - (x1 - x0) * (p2 - p1)
    ) / point2pointDistance(p1, p2);
}

/*
 * Adds a new point at the end of the list
 * removes the first point if there are too many points
 * @param x - x position of the new point
 * @param y - y position of the new point
 */
void addNewPoint (float x, float y) {
    if (numCV >= MAX_CV)
        removeFirstPoint ();

    CV[numCV][0] = x;
    CV[numCV][1] = y;
    CV[numCV][2] = 0;
    numCV++;
}

/*
 * Removes the first control point
 */
void removeFirstPoint () {
    int i;
    if (numCV > 0) {
        // Remove the first point, slide the rest down
        numCV--;
        for (i = 0; i < numCV; i++) {
            CV[i][0] = CV[i + 1][0];
            CV[i][1] = CV[i + 1][1];
        }
    }
}

/*
 * Remove last control point
 */
void removeLastPoint () {
    if (numCV > 0)
        numCV--;
}

/*
 * Get the nearest point in click range
 * @return index of the nearest point in range
 */
int getPointInRange ( float x, float y ) {
    int near;
    double lowerDistance;
    double r;
    double distance;

    near = -1;
    r = 0.05;
    lowerDistance = sqrt(pow (x - CV[0][0], 2.0 ) + pow (y - CV[0][1], 2.0 ) );
    if( lowerDistance <= r ) near = 0;

    for ( int i = 1 ; i < numCV ; i++ ) {
        distance = sqrt(pow ( x - CV[i][0], 2.0 ) + pow ( y - CV[i][1], 2.0 ) );
        if ( distance <= r && distance <= lowerDistance ) {
            near = i;
            lowerDistance = distance;
        }
    }

    return near;
}

/*
 * Moves a control point
 * @param i - index of the control point
 * @param x - x mouse's coordinate
 * @param y - y moouse's coordinate
 */
void movePoint ( int i, int x, int y ) {
    float xPos;
    float yPos;

    yPos = ((float) y) / ((float) (WindowHeight - 1));
    xPos = ((float) x) / ((float) (WindowWidth - 1));
    yPos = 1.0f - yPos;			   // Flip value since y position is from top row.

    CV[i][0] = xPos;
    CV[i][1] = yPos;
    glutPostRedisplay ();
}



/**********************
 *     ALGORITHMS
 **********************/

/*
 * Draws a bezier curve
 */
void drawBezier () {
    int i;


    if(DRAW_MODE == ADAPTIVE) {
        // DRAW MODE : adaptive
        glBegin(GL_LINE_STRIP);
        adaptiveSubdivision(CV, 0.00005f);
        glEnd();
    } else {
        glEnable(GL_MAP1_VERTEX_3);
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, numCV, &CV[0][0]);

        glBegin(GL_LINE_STRIP);
        for ( i = 0; i <= 100; i++) {
            if(DRAW_MODE == DECASTELJAU) {
                // DRAW MODE : decasteljau
                deCasteljau( (float) (i / 100.0));
            } else {
                // DRAW MODE : opengl
                glEvalCoord1d( (GLdouble) (i / 100.0));
            }
        }
        glEnd();
        glDisable(GL_MAP1_VERTEX_3);
    }

}

/*
 * De Casteljau evaluator
 * @param t - evaluates the curve in t (range 0-1)
 */
void deCasteljau (float t) {
    float temp[numCV][DIM];
    int i;

    for (i = 0; i < numCV; i++) {
        assignToPoint(temp[i], CV[i]);
    }

    for (i = 1; i < numCV; i++) {
        for (int j = 0; j < numCV - i; j++) {
            lerp(temp[j], temp[j + 1], t, temp[j]);
        }
    }

    glVertex3f (temp[0][0], temp[0][1], temp[0][2]);
}


/*
 * Adaptive subdivision algorithm
 * @param cps - curve control points
 * @param tolerance - degree of smoothness
 */
void adaptiveSubdivision(float cps[MAX_CV][DIM], float tolerance) {
    int i, j;
    float temp[numCV][DIM];
    float curve1[numCV][DIM];
    float curve2[numCV][DIM];
    bool canApproxLine = true;

    // Calculates the distance of every CP from the line between
    // the first CP and the last CP
    for (i = 1; i < numCV - 1; i++) {
        if(point2LineDistance(cps[i], cps[0], cps[numCV - 1]) > tolerance) {
            canApproxLine = false;
        }
    }

    // Draw the line if it can be approximated
    if(canApproxLine == true) {
        glVertex3f(cps[0][0], cps[0][1], cps[0][2]);
        glVertex3f(cps[numCV - 1][0], cps[numCV - 1][1], cps[numCV - 1][2]);
    } else {

        for (i = 0; i < numCV; i++) {
            assignToPoint(temp[i], cps[i]);
        }

        assignToPoint(curve1[0], temp[0]);
        assignToPoint(curve2[numCV - 1], temp[numCV - 1]);

        // Otherwise evaluate the point in 0.5
        for (i = 1; i < numCV; i++) {
            for (j = 0; j < numCV - i; j++) {
                lerp(temp[j], temp[j+1], 0.5f, temp[j]);
            }
            assignToPoint(curve1[i], temp[0]);
            assignToPoint(curve2[numCV - i - 1], temp[numCV - i - 1]);
        }

        // Recursive call on the 2 sub curves
        adaptiveSubdivision(curve1, tolerance);
        adaptiveSubdivision(curve2, tolerance);
    }

}



/**********************
 *      HANDLERS
 **********************/

/*
 * Handles keyboard events
 * @param key - key pressed code
 */
void keyboard (unsigned char key, int x, int y) {
    switch (key) {
        case 'f':
            removeFirstPoint ();
            glutPostRedisplay ();
            break;
        case 'l':
            removeLastPoint ();
            glutPostRedisplay ();
            break;
        case 27:						   // Escape key
            exit (0);
            break;
    }
}

/*
 * Handles mouse click events
 * @param button - pressed mouse button code
 * @param state - pressed mouse button state
 * @param x - mouse x position
 * @param y - mouse y position
 */
void mouse (int button, int state, int x, int y) {
    float xPos;
    float yPos;

    yPos = ((float) y) / ((float) (WindowHeight - 1));
    xPos = ((float) x) / ((float) (WindowWidth - 1));
    yPos = 1.0f - yPos;			   // Flip value since y position is from top row.

    // [NEW] check if point in range
    if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
        clickedPoint = getPointInRange( xPos , yPos );
    } else if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP ) {
        if ( clickedPoint >= 0 ) {
            movePoint ( clickedPoint, x, y );
            clickedPoint = -1;
        } else {
            addNewPoint(xPos, yPos);
            glutPostRedisplay();
        }
    }
}

/*
 * Handles mouse move events
 * @param x - mouse x position
 * @param y - mouse y position
 */
void mousemove (int x, int y) {
    if ( clickedPoint >= 0 ) {
        movePoint ( clickedPoint, x, y );
    }
}



/**********************
 *   MAIN FUNCTIONS
 **********************/

/*
 * Display function
 */
void display (void) {
    int i;
    glClear (GL_COLOR_BUFFER_BIT);

    // Draw the line segments
    glColor3f (1.0f, 0.0f, 0.8f);	   // Reddish/purple lines
    glEnable (GL_LINE_STIPPLE);
    glLineStipple (1, 0x00FF);
    if (numCV > 1) {
        glBegin (GL_LINE_STRIP);
        for (i = 0; i < numCV; i++) {
            glVertex3f (CV[i][0], CV[i][1], CV[i][2]);
        }
        glEnd ();
    }
    glDisable (GL_LINE_STIPPLE);

    // Draw the interpolated points second.
    glColor3f (0.01f, 0.1f, 1.0f);	   // Draw points in blue
    glBegin (GL_POINTS);
    for (i = 0; i < numCV; i++) {
        glVertex3f (CV[i][0], CV[i][1], CV[i][2]);
    }

    glEnd ();

    if(numCV > 1) drawBezier();

    glFlush ();
}

/*
 * Init function
 */
void initRendering () {
    glClearColor (1.0f, 1.0f, 1.0f, 1.0f);

    // Make big points and wide lines.  (This may be commented out if desired.)
    glPointSize (8);
    glLineWidth (0.7);

    // The following commands should induce OpenGL to create round points and
    //  antialias points and lines.  (This is implementation dependent unfortunately, and
    //  may slow down rendering considerably.)
    //  You may comment these out if you wish.
    glEnable (GL_POINT_SMOOTH);
    glEnable (GL_LINE_SMOOTH);
    glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);	// Antialias the lines
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*
 * Resize function
 */
void reshape (int w, int h) {
    WindowHeight = (h > 1) ? h : 2;
    WindowWidth = (w > 1) ? w : 2;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (0.0f, 1.0f, 0.0f, 1.0f);	// Always view [0,1]x[0,1].
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}

int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);

    initRendering ();

    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glutMouseFunc (mouse);
    glutMotionFunc(mousemove); // detects mouse move while dragging
//    glutPassiveMotionFunc(mousemove); // detects mouse move
    glutMainLoop ();
    return 0;						   // This line is never reached
}
