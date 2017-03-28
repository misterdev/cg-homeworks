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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
//	#include <GL/glut.h>
#endif

#define MAX_CV 64

/* prototypes */
void removeFirstPoint ();
void removeLastPoint ();
void addNewPoint (float x, float y);

/* global variables */
float CV[MAX_CV][3];
int numCV = 0;

// Window size in pixels
int WindowHeight;
int WindowWidth;

/* function definitions */

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

// Left button presses place a control point.
void mouse (int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float xPos = ((float) x) / ((float) (WindowWidth - 1));
        float yPos = ((float) y) / ((float) (WindowHeight - 1));
        yPos = 1.0f - yPos;			   // Flip value since y position is from top row.
        addNewPoint (xPos, yPos);
        glutPostRedisplay ();
    }
}

// Add a new point to the end of the list.
// Remove the first point in the list if too many points.
void removeLastPoint () {
    if (numCV > 0)
        numCV--;
}


// Add a new point to the end of the list.
// Remove the first point in the list if too many points.
void addNewPoint (float x, float y) {
    if (numCV >= MAX_CV)
        removeFirstPoint ();

    CV[numCV][0] = x;
    CV[numCV][1] = y;
    CV[numCV][2] = 0;
    numCV++;
}

void drawBezier () {
    int i;
    glEnable(GL_MAP1_VERTEX_3);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, numCV, CV);
    glBegin(GL_LINE_STRIP);
    for ( i = 0; i <= 100; i++)
    {
        glEvalCoord1d( (GLdouble) (i / 100.0));
    }
    glEnd();
    glDisable(GL_MAP1_VERTEX_3);
}

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
    glColor3f (0.0f, 0.0f, 0.0f);	   // Draw points in black
    glBegin (GL_POINTS);
    for (i = 0; i < numCV; i++) {
        glVertex3f (CV[i][0], CV[i][1], CV[i][2]);
    }

    glEnd ();

    if (numCV > 1) drawBezier();

    glFlush ();
}

void initRendering () {
    glClearColor (1.0f, 1.0f, 1.0f, 1.0f);

    // Make big points and wide lines.  (This may be commented out if desired.)
    glPointSize (8);
    glLineWidth (1);

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

    glutMainLoop ();
    return 0;						   // This line is never reached
}
