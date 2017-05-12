
// MIO
// program = initShader("v.glsl", "f.glsl");
// timeParam = glGetUniformLocation(program, "time"); // time param = nome nel programma || time = nome nello shader
// amplitude = glGetUniformLocation(program, "A");
// frequency = glGetUniformLocation(program, "omega");

// glUniform1f(amplitude, 0.1);
// gluniform1f(frequency, 0.001);

/* sets up flat mesh */
/* sets up elapsed time parameter for use by shaders */
/* vertex shader varies height of mesh sinusoidally */
/* uses a pass through fragment shader */

#include "../../common/common.h"

#define N 64

GLuint program;
GLint timeParam;
GLint amplitude;
GLint frequency;

static void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.75,0.75,-0.75,0.75,-5.5,5.5);

    #ifndef __APPLE__
        glewInit();
    #endif
    program = initShader("../src/v.glsl", "../src/f.glsl");

    /* ----------- ADD ------------------ 
    glGetUniformLocation of time, amplitude and frequency
// qui li chiamo in un modo e li passo al vertex shader v.glsl
// la tabella la riempo durante l'animazione
    set the values via glUniform1f
    */
}


void mesh()
{
    int i,j;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2.0, 2.0, 2.0, 0.5, 0.0, 0.5, 0.0, 1.0, 0.0);
    for(i=0; i<N; i++)
    {
        for(j=0; j<N;j++)
        {
            glColor3ub(50,109,164);
            glBegin(GL_QUADS);
                glVertex3f((float)i/N, 0.0, (float)j/N);
                glVertex3f((float)i/N, 0.0, (float)(j+1)/N);
                glVertex3f((float)(i+1)/N, 0.0, (float)(j+1)/N);
                glVertex3f((float)(i+1)/N, 0.0, (float)(j)/N);
            glEnd();

            glColor3f(0.2, 0.2, 0.2);
            glBegin(GL_LINE_LOOP);
                glVertex3f((float)i/N, 0.0, (float)j/N);
                glVertex3f((float)i/N, 0.0, (float)(j+1)/N);
                glVertex3f((float)(i+1)/N, 0.0, (float)(j+1)/N);
                glVertex3f((float)(i+1)/N, 0.0, (float)(j)/N);
            glEnd();
        }
    }
}

static void draw(void)
{
    /* ----------- ADD ------------------ 
        send elapsed time to shaders
glUniform1f(timeParam, glutGet(GLUT_ELAPSED_TIME))
        to get the elpased time, use glutGet(GLUT_ELAPSED_TIME)
    */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mesh();

    glutSwapBuffers();
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    /* ----------- ADD ------------------ 
        add functionality for left and right button
        left    -- randomly change aplitude through values [0.05; 0.1; 0.2].
        right   -- randomly change frequency through values [0.0005; 0.001; 0.002].

    */
    // TODO modifica freq e amplitude
}

int main(int argc, char** argv)
{
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Simple GLSL example");
    glutDisplayFunc(draw);
    glutKeyboardFunc(commonKeyboard);
    glutMouseFunc(mouse);

    init();

    glutMainLoop();
}
