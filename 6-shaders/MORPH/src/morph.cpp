#include "../../common/common.h"

GLuint         program;
GLint          timeParam;
GLint          vertices_two_location;

const GLfloat vertices_one[3][2] = {{0.0, 0.0},{0.5,1.0},{1.0, 0.0}};
const GLfloat vertices_two[3][2] = {{0.0, 1.0},{0.5,0.0},{1.0, 1.0}};

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
    timeParam = glGetUniformLocation(program, "time");
    vertices_two_location = glGetAttribLocation(program, "vertices2");
	/* ----------- ADD ------------------ 
			handle additional variables if needed
	*/
}

static void draw(void)
{
    /* send elapsed time to shaders */
    glUniform1f(timeParam, glutGet(GLUT_ELAPSED_TIME));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glBegin(GL_LINE_LOOP);
		/* ----------- ADD ------------------ 
			instead of lines, draw a triangle
			assign color to the vertices
		*/
        glVertexAttrib2fv(vertices_two_location, &vertices_two[0][0]);
        glVertex2fv(vertices_one[0]);
        glVertexAttrib2fv(vertices_two_location, &vertices_two[1][0]);
        glVertex2fv(vertices_one[1]);
        glVertexAttrib2fv(vertices_two_location, &vertices_two[2][0]);
        glVertex2fv(vertices_one[2]);
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
