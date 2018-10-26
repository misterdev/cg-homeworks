#include "../../common/common.h"
#include "RgbImage.h"

GLuint program;
GLuint tex;

const char* cubicFilenameArray[6] = {
        "../textures/posx2.bmp",
        "../textures/negx2.bmp",
        "../textures/posy2.bmp",
        "../textures/negy2.bmp",
        "../textures/posz2.bmp",
        "../textures/negz2.bmp",
};

void init()
{
    GLubyte red[3] = {255, 0, 0};
    GLubyte green[3] = {0, 255, 0};
    GLubyte blue[3] = {0, 0, 255};
    GLubyte cyan[3] = {0, 255, 255};
    GLubyte magenta[3] = {255, 0, 255};
    GLubyte yellow[3] = {255, 255, 0};

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_CUBE_MAP);

    //TODO RgbImageInitFile(&right, "texture/right_1") //r,l,t,b,f,b
    RgbImage image;

    glActiveTexture(GL_TEXTURE0); //?

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    for(GLuint i = 0; i < 6; i++) {
        RgbImageInitFile(&image, cubicFilenameArray[i]);
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, image.ImagePtr
        );
    }

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);

//        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X ,0,3,1,1,0,GL_RGB,GL_UNSIGNED_BYTE, red);
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X ,0,3,1,1,0,GL_RGB,GL_UNSIGNED_BYTE, green);
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y ,0,3,1,1,0,GL_RGB,GL_UNSIGNED_BYTE, blue);
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ,0,3,1,1,0,GL_RGB,GL_UNSIGNED_BYTE, cyan);
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z ,0,3,1,1,0,GL_RGB,GL_UNSIGNED_BYTE, magenta);
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z ,0,3,1,1,0,GL_RGB,GL_UNSIGNED_BYTE, yellow);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST); 

    glClearColor(1.0,1.0,1.0,1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);

    #ifndef __APPLE__
        glewInit();
    #endif
    program = initShader("../src/v.glsl", "../src/f.glsl");

    GLuint texMapLocation;
    texMapLocation = glGetUniformLocation(program, "texMap");
    glUniform1i(texMapLocation,0);
}

void draw(void)
{
    double t = (double)glutGet(GLUT_ELAPSED_TIME);
    double k = 0.05 * 360.0/1000.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
    glPushMatrix();
        //glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5.0f);
        glRotatef(k*t, 1.0, 0.0, 0.0);
        glRotatef(k*t, 0.0, 1.0, 0.0);
        glutSolidTeapot(1.0);
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("colorcube environment");

    glutDisplayFunc(draw);
    glutKeyboardFunc(commonKeyboard);

    init();

    glutMainLoop();
    return 0;
}
