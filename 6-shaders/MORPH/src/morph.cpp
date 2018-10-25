#include "../../common/common.h"

GLuint         program;
GLint          timeParam;
GLint          vertices2;
GLint          colors2;

const GLfloat vertices_one[4][2] = {{0.2, 0.2},{0.5,0.8},{0.8, 0.2},{0.5,0.8}};
const GLfloat vertices_two[4][2] = {{0.8, 0.2},{0.2,0.2},{0.2, 0.8},{0.8,0.8}};

const GLfloat colors_one[4][3] = {{0.9, 0.1, 0.1}, {0.1, 0.8, 0.1}, {0.1, 0.1, 0.8}, {0.1, 0.8, 0.8}};
const GLfloat colors_two[4][3] = {
        {0.129411764705882f, 0.588235294117647f, 0.952941176470588f},
        {0.298039215686275f, 0.686274509803922f, 0.313725490196078f},
        {0.129411764705882f, 0.588235294117647f, 0.952941176470588f},
        {0.686274509803922f, 0.313725490196078f, 0.298039215686275f}
};


static void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0.0,0.0,0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,1.0,0.0,1.0);

    glEnable(GL_DEPTH_TEST);

    #ifndef __APPLE__
        glewInit();
    #endif

    program = initShader("../src/v.glsl", "../src/f.glsl");

    // Setup uniform and attribute prameters
    timeParam   = glGetUniformLocation(program, "time");
    vertices2   = glGetAttribLocation(program, "vertices2");
    colors2     = glGetAttribLocation(program, "colors2");

}

static void draw(void)
{
    /* send elapsed time to shaders */
    glUniform1f(timeParam, glutGet(GLUT_ELAPSED_TIME));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_POLYGON);
    
    for (int i = 0; i < 4; i++) {
        glVertexAttrib2fv(vertices2, &vertices_two[i][0]);
        glVertex2fv(vertices_one[i]);

        glVertexAttrib3fv(colors2, &colors_two[i][0]);
        glColor3fv(colors_one[i]);
    }

    glEnd();

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Simple GLSL example");
    glutDisplayFunc(draw);
    glutKeyboardFunc(commonKeyboard);

    init();

    glutMainLoop();
    return 0;
}
