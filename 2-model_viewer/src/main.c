/*  CG LAB2
Gestione interattiva di una scena 3D mediante controllo
da mouse e da tastiera. I modelli geometrici in scena
sono primitive GLU e mesh poligonali in formato *.m

 * INPUT: file .m contenente la mesh a triangoli:
 *        Vertex  NUM  x y z
 *                .......
 *        Normal  NUM  x y z
 *                .......
 *        Face    NUM f1 f2 f3
 *                .......
 * OUPUT: visualizzazione della mesh in una finestra OpenGL
 *
 *
 * NB: i vertici di ogni faccia sono in verso orario
 */

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include "main.h"
#include "utils.h"
#include "v3d.h"


#define MAX_V 10000 // max number of vertices allowed in the mesh model
#define MAX_P 3     // max number of figures allowed in the program
#define M_PI 3.141592653589793

float tbRotationMatrix[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, 1.0f };
int tbUpdateMatrix = 0;
float pVCS[MAX_P+1][3] = { {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} };

int 	listname;
int  	wireframe; // controls the visualization of primitives via glPolygonMode
int     orpro;     // controls the type of projection via gluPerspective and glOrtho
int     cull;      // toggles backface culling via glEnable( GL_CULL_FACE ) and glDisable( GL_CULL_FACE );
int     mater;     // controls the material associated with the model via glMaterial
int     shading;   // controls the shading model via glShadeModel
int     cameraMotion; // controls the motion

GLfloat fovy;                       // angolo del punto di vista
GLfloat angle[3];                   // rotazione degli assi WCS

GLfloat camC[3];        // centro del sistema
GLfloat camE[3];        // punto di vista
GLfloat camU[3];        // vettore su della camera
GLfloat lightPos[4];    // posizione della luce

GLfloat brass_ambient[] = {0.33, 0.22, 0.03, 1.0}, brass_diffuse[] = {0.78, 0.57, 0.11, 1.0}, brass_specular[] = {0.99, 0.91, 0.81, 1.0}, brass_shininess[] = {27.8};
GLfloat red_plastic_ambient[] = {0.0, 0.0, 0.0}, red_plastic_diffuse[] = {0.5, 0.0, 0.0}, red_plastic_specular[] = {0.7, 0.6, 0.6}, red_plastic_shininess[] = {32.0};
GLfloat emerald_ambient[] = {0.0215, 0.1745, 0.0215}, emerald_diffuse[] = {0.07568, 0.61424, 0.07568}, emerald_specular[] = {0.633, 0.727811, 0.633}, emerald_shininess[] = {76.8};
GLfloat slate_ambient[] = {0.02, 0.02, 0.02}, slate_diffuse[] = {0.02, 0.01, 0.01}, slate_specular[] = {0.4, 0.4, 0.4}, slate_shininess[] = {.78125};


GLfloat aspect = 1.0;   // rapporto larghezza-altezza della viewport

GLUquadricObj* myReusableQuadric = 0;

char*   pnames[MAX_P] = {"../data/cow.m", "../data/toy2.m", "../data/cbun.m"};   // names of the files to use
int     pSelected   = MAX_P;
// traslazioni rispetto agli assi
GLfloat pTranslations[MAX_P+1][3] = { { 1, 1, 0 }, { 2, 0.5, 2 }, { 0, -0.5, 2 } , { 0, 0, 0 } };
GLfloat pScales[MAX_P+1]          = { 0.25, 0.25, 10.5, 1 };
GLfloat pRotations[MAX_P+1][3]    = { { 0, 0, 0 }, { 90, 0, 0 }, { 0, 90, 0  }, { 0, 0, 0 } };

/* Trackball variables */
float tbAngle = 0.0;
float tbAxis[3];

int tbDragging = 0;
float tbW[3];
float tbV[3];

float timer = 0.0f; // Camera Motion Timer

enum Modes {
    MODE_INVALID,
    MODE_CHANGE_EYE_POS,
    MODE_CHANGE_REFERENCE_POS,
    MODE_CHANGE_UP_POS,
    MODE_CHANGE_LIGHT_POS,
    MODE_CHANGE_ZOOM, //5
    MODE_ROTATE_OBJECT,
    MODE_FLOAT_AROUND,

    MODE_CHANGE_CULLING,
    MODE_CHANGE_WIREFRAME,
    MODE_CHANGE_SHADING, //10
    MODE_CHANGE_PROJECTION,
    MODE_CHANGE_MATERIAL,

    MODE_TRANSLATE_OBJECT,
    MODE_TRANSLATE_WCS,
    MODE_ROTATE_WCS, // 15
    MODE_TRANSLATE_OCS,
    MODE_ROTATE_OCS,
    MODE_TRANSLATE_VCS,
    MODE_ROTATE_VCS,

    MODE_PRINT_SYSTEM_STATUS, //20
    MODE_RESET,
    MODE_QUIT
};

enum Modes mode = MODE_CHANGE_REFERENCE_POS; // global variable that stores the current mode

int WindowWidth = 700;
int WindowHeight = 700;

void drawAxis( float scale, int drawLetters ) {
    glDisable( GL_LIGHTING );
    glPushMatrix();
    glScalef( scale, scale, scale );
    glBegin( GL_LINES );

    glColor4d( 1.0, 0.0, 0.0, 1.0 );
    if( drawLetters ) {
        glVertex3f( .8f, 0.05f, 0.0 );  glVertex3f( 1.0, 0.25f, 0.0 ); /* Letter X */
        glVertex3f( 0.8f, .25f, 0.0 );  glVertex3f( 1.0, 0.05f, 0.0 );
    }
    glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 1.0, 0.0, 0.0 ); /* X axis      */


    glColor4d( 0.0, 1.0, 0.0, 1.0 );
    if( drawLetters ) {
        glVertex3f( 0.10f, 0.8f, 0.0 );   glVertex3f( 0.10f, 0.90f, 0.0 ); /* Letter Y */
        glVertex3f( 0.10f, 0.90f, 0.0 );  glVertex3f( 0.05, 1.0, 0.0 );
        glVertex3f( 0.10f, 0.90f, 0.0 );  glVertex3f( 0.15, 1.0, 0.0 );
    }
    glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 1.0, 0.0 ); /* Y axis      */


    glColor4d( 0.0, 0.0, 1.0, 1.0 );
    if( drawLetters ) {
        glVertex3f( 0.05f, 0, 0.8f );  glVertex3f( 0.20f, 0, 0.8f ); /* Letter Z*/
        glVertex3f( 0.20f, 0, 0.8f );  glVertex3f( 0.05, 0, 1.0 );
        glVertex3f( 0.05f, 0, 1.0 );   glVertex3f( 0.20, 0, 1.0 );
    }
    glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 0.0, 1.0 ); /* Z axis    */

    glEnd();
    glPopMatrix();

    // sequenza matrici proiezione - vista - tm - t
    // siccome sono effettuate all'inverso devo dargliele al contrario
    // prima di disegnare l'oggetto devo aver pronta la matrice
    // OCS matrice di 16 elementi che serve per traformare l'oggetto nel proprio pianno di riferimento
    glEnable( GL_LIGHTING );
}

void computePointOnTrackball( int x, int y, float p[3] ) {
    float zTemp;
    //map to [-1;1]
    p[0] = (2.0f * x - WindowWidth)  / WindowWidth;
    p[1] = (WindowHeight - 2.0f * y) / WindowHeight;

    zTemp = 1.0f - (p[0] * p[0]) - (p[1] * p[1]);
    p[2] = (zTemp > 0.0f ) ? sqrt(zTemp) : 0.0;

//    printf( "p = (%.2f, %.2f, %.2f)\n", p[0], p[1], p[2] );
}

void zoom( int direction ) {
    // c(t) = P0 + tv
    // P0 = camE
    // t = 1
    // X calcolare v = P0 - P1
    float v[3];
    float tv[3];
    float origin[3] = {0, 0, 0};
    float t;

    t = 0.1f * (float) direction;

    v3dSub(origin, camE, v);

    v3dMul(v, t, tv);
    v3dSum(camE, tv, camE);
}

void mouse( int button, int state, int x, int y ) {
    if( button==GLUT_LEFT_BUTTON && state  == GLUT_DOWN ) {
        computePointOnTrackball( x, y, tbV );
    }
    if( button==GLUT_LEFT_BUTTON && state==GLUT_UP ) {
        if(tbDragging) {
            tbDragging = 0;
            tbUpdateMatrix = 1;
            glutPostRedisplay();
        }
    }
}

void motion( int x, int y ) {
    float dx, dy, dz;

    tbDragging = 1;

    computePointOnTrackball(x, y, tbW);
    if(tbDragging) {
        dx = tbW[0] - tbV[0];
        dy = tbW[1] - tbV[1];
        dz = tbW[2] - tbV[2];
        if (dx || dy || dz) {
            tbAngle = sqrt(dx * dx + dy * dy + dz * dz) * (180.0 / M_PI);
            tbAxis[0] = tbV[1]*tbW[2] - tbV[2]*tbW[1];
            tbAxis[1] = tbV[2]*tbW[0] - tbV[0]*tbW[2];
            tbAxis[2] = tbV[0]*tbW[1] - tbV[1]*tbW[0];
        }
    }

    glutPostRedisplay();
}

void passiveMotion( int x, int y ) {

}

void special( int key, int x, int y ) {

}

void keyboard (unsigned char key, int x, int y) {
    float* pos = NULL;
    float step;

    printf("MODE: ");
    printf("%d \n", mode);

    if ( mode == MODE_CHANGE_EYE_POS ) {
        pos = camE;
        step = 0.5;
    } else if( mode == MODE_CHANGE_REFERENCE_POS ) {
        pos = camC;
        step = 0.1;
    } else if( mode == MODE_CHANGE_UP_POS ) {
        pos = camU;
        step = 1.0;
    } else if( mode == MODE_CHANGE_LIGHT_POS ) {
        pos = lightPos;
        step = 1.0;
    } else if( mode == MODE_TRANSLATE_OBJECT ) {
        pos = pTranslations[pSelected];
        step = 0.5;
    } else if( mode == MODE_ROTATE_OBJECT ) {
        pos = pRotations[pSelected];
        step = 1.0;
    }


    if( pos != NULL )
    {
        printf(" %c", key);
        printf("\n");

        if( key == 'x' )
            pos[0] += step;
        else if( key == 'X' )
            pos[0] -= step;
        else if( key == 'y' )
            pos[1] += step;
        else if( key == 'Y' )
            pos[1] -= step;
        else if( key == 'z' )
            pos[2] += step;
        else if( key == 'Z' )
            pos[2] -= step;
        else if( key == 'v' ) {
            zoom(-1);
        }
        else if( key == 'V') {
            zoom(1);
        }
        else if( key == '0') {
            pSelected = MAX_P;  // the world object
        }
        else if( key == '1') {
            pSelected = 0;      // the 1st object
        }
        else if( key == '2') {
            pSelected = 1;      // the 2nd object
        }
        else if( key == '3') {
            pSelected = 2;      // the 3rd object
        }

        glutPostRedisplay();
    }

    if( key ==  27 ) //esc
        exit(1);

}

void drawGluSlantCylinderWithCaps( double height, double radiusBase, double radiusTop, int slices, int stacks ) {
    // First draw the cylinder
    gluCylinder( myReusableQuadric, radiusBase, radiusTop, height, slices, stacks );

    // Draw the top disk cap
    glPushMatrix();
    glTranslated(0.0, 0.0, height);
    gluDisk( myReusableQuadric, 0.0, radiusTop, slices, stacks );
    glPopMatrix();

    // Draw the bottom disk cap
    glPushMatrix();
    glRotated(180.0, 1.0, 0.0, 0.0);
    gluDisk( myReusableQuadric, 0.0, radiusBase, slices, stacks );
    glPopMatrix();

}

void reset() {
//    angle[0] = 0.0;
//    angle[1] = 0.0;
//    angle[2] = 0.0;
    pTranslations[MAX_P][0] = 0.0;
    pTranslations[MAX_P][1] = 0.0;
    pTranslations[MAX_P][2] = 0.0;

    camE[0] = 8.8;
    camE[1] = 4.9;
    camE[2] = 9.0;

    camC[0] = 0.0;
    camC[1] = 0.0;
    camC[2] = 0.0;

    camU[0] = 0.0;
    camU[1] = 1.0;
    camU[2] = 0.0;

    lightPos[0] =  5.0;
    lightPos[1] =  5.0;
    lightPos[2] =  5.0;
    lightPos[3] =  1.0;

    fovy = 20;
    wireframe = 0;
    cull = 0;
    mater = 1;
    orpro = 1;
    shading = 0;
    cameraMotion = -1; // nobody

    glutPositionWindow(10,10);
    glutReshapeWindow(700,700);
    glutPostRedisplay();

}

void readFile (const char* FILENAME, int faces[MAX_V][3], int nface, float vertices[MAX_V][3], float fnormals[MAX_V][3], float vnormals[MAX_V][3], int listname) {
    FILE * idf;
    int nvert, noused;
    char s[10];
    float* vert;
    float a,b,c;
    int i, nrighe;
    float *fnormal, *vnormal;
    int* face;
//    faces / nface / fnormals / vnormals / vertices

    //apertura del file *.m
    printf("Apertura del file...\n");

#ifdef WIN32
    if ((idf = fopen( FILENAME, "r")) == NULL)
#else
    if ((idf = fopen( FILENAME, "r")) == NULL )
#endif
    {
        perror("file non trovato/i\n");
        exit (1);
    }

    nface=0;
    nvert=0;

    int vnormalcounts[MAX_V] = {0};
////////
    while( !feof( idf ) )
    {
        fscanf(idf,"%s %d %f %f %f",s, &noused, &a,&b,&c);
        switch( s[0] )
        {
            case 'V':
                vert = vertices[nvert];
                nvert++;

                vert[0] = a;
                vert[1] = b;
                vert[2] = c;
                break;
            case 'N':
                printf(" Vertex normals not used! \n");
                break;
            case 'F':
                face = faces[nface];
                fnormal = fnormals[nface];

                nface++;
                face[0] = (int)a - 1;
                face[1] = (int)b - 1;
                face[2] = (int)c - 1;

                float temp1[3];
                float temp2[3];

                v3dSub(vertices[face[1]], vertices[face[0]], temp1);
                v3dSub(vertices[face[2]], vertices[face[0]], temp2);
                v3dCross(temp2, temp1, fnormal);
                // calcolo normali alle facce = cross product v1-v0 v2-v0 -- memorizzo in fnormal
                // calcolo normale al vertice -- sommo su un vertice tutte le normali delle facce che incidono in quel vertice

                for(int k = 0; k < 3; k++) { // itero sui vertici della faccia
                    vnormal = vnormals[face[k]];
                    v3dSum(vnormal, fnormal, vnormal);
                    vnormalcounts[face[k]]++;
                }

                break;
            case 'E':
                printf(" Edge not used! \n");
                break;
            default:
                printf(" ERROR: Reading file \n");
                break;
        }
    }

    nrighe = nvert + nface;
    nface--;
    printf("Chiusura del file...Vertices %d Faces %d -> %d\n",nvert, nface,nrighe);
    fclose(idf);

    // Finito di leggere il file, faccio la media
    for ( i = 0 ; i < nvert ; i++ ) {
        v3dMul(vnormals[i], 1.0f / (float) vnormalcounts[i], vnormals[i]);
        vnormalcounts[i] = 0;
    }

    // crea la display list mesh
    printf("Creazione display list .. \n");
    glNewList(listname,GL_COMPILE);

    int ids[3], ii;
    for( i = 0; i < nface; i++ ) {
        ids[2] = faces[i][0];
        ids[1] = faces[i][1];
        ids[0] = faces[i][2];

        glNormal3f( fnormals[i][0], fnormals[i][1], fnormals[i][2] );

        //disegna triangoli coi vertici specificati
        glBegin( GL_TRIANGLES );
        glColor3f(1,0,0);
        for( ii = 2; ii >= 0; ii--)
        {
            glNormal3f( vnormals[ids[ii]][0], vnormals[ids[ii]][1], vnormals[ids[ii]][2] );
            glVertex3f( vertices[ids[ii]][0], vertices[ids[ii]][1], vertices[ids[ii]][2] );
        }
        glEnd();
    }
    glEndList();
}

const int NCAM_CPS = 5;

float camCPS[NCAM_CPS][3] = {
        { 0.0f, 7.0f, 0.000000f },
        { -17.0f, -3.0f, 0.000000f },
        { 16.0f, -26.0f, 0.000000f },
        { 18.0f, 15.0f, 0.000000f },
        { 0.0f, 7.0f, 0.000000f }
};
float camTimer = 0.0f;

void floatAround () {
    float speed = 0.001f;
    int i;
    float temp[NCAM_CPS][3];

    for (i = 0; i < NCAM_CPS; i++) {
        assignToPoint(temp[i], camCPS[i]);
    }

    for (i = 1; i < NCAM_CPS; i++) {
        for (int j = 0; j < NCAM_CPS - i; j++) {
            lerp(temp[j], temp[j + 1], camTimer, temp[j]);
        }
    }

    camE[0] = temp[0][0];
    camE[2] = temp[0][1];

    if ( camTimer + speed <= 1.0f ) {
        camTimer += speed;
    } else {
        camTimer = 0.0f;
    }

    glutPostRedisplay();
}


void init() {
    int 	faces   [MAX_P][MAX_V][3];    /* faces */
    float 	vertices[MAX_P][MAX_V][3]; /* vertices */
    float 	vnormals[MAX_P][MAX_V][3]; /* vertex normals */
    float   fnormals[MAX_P][MAX_V][3]; /* face normals */
    int     nfaces  [MAX_P];

    int p, i, j;


    listname=glGenLists(MAX_P);  //ID della lista e' listname

    for ( p=0; p < MAX_P ; p++ ) {
        for( i=0 ; i<MAX_V ; i++ ) {
            for( j=0 ; j<3 ; j++ ) {
                fnormals[p][i][j] = 0.0f;
                vnormals[p][i][j] = 0.0f;
            }
        }
    }

    for ( p=0 ; p < MAX_P ; p++ ) {
        readFile(pnames[p], faces[p], nfaces[p], vertices[p], fnormals[p], vnormals[p], listname + p);
    }


    // TODO: crea la display list quadrica
    myReusableQuadric = gluNewQuadric();
    gluQuadricNormals( myReusableQuadric, GL_TRUE );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glClearColor( 1.0, 1.0, 1.0, 1.0 );

//    glEnable (GL_POINT_SMOOTH);
//    glEnable (GL_LINE_SMOOTH);
//    glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
//    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);	// Antialias the lines
//    glEnable (GL_BLEND);
//    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void print_sys_status() {
    system("clear");
    printf("\nSystem status\n\n");
    printf("---------------------------------------------------\n");
    printf("Eye point coordinates :\n");
    printf("x = %.2f, y = %.2f, z = %.2f\n",camE[0], camE[1], camE[2] );
    printf("---------------------------------------------------\n");
    printf("Reference point coordinates :\n");
    printf("x = %.2f, y = %.2f, z = %.2f\n",camC[0], camC[1], camC[2] );
    printf("---------------------------------------------------\n");
    printf("Up vector coordinates :\n");
    printf("x = %.2f, y = %.2f, z = %.2f\n",camU[0], camU[1], camU[2] );
    printf("---------------------------------------------------\n");
    printf("Light position coordinates :\n");
    printf("x = %.2f, y = %.2f, z = %.2f\n",lightPos[0], lightPos[1], lightPos[2] );
    printf("---------------------------------------------------\n");
    printf("Axis rotation (in degree) :\n");
    printf("x = %.2f, y = %.2f, z = %.2f\n",angle[0], angle[1], angle[2] );
    printf("---------------------------------------------------\n");
    printf("Field of view angle (in degree) = %.2f\n",fovy);
    printf("---------------------------------------------------\n");
    if(wireframe)
        printf("Wireframe = YES\n");
    else
        printf("Wireframe = NO\n");
    printf("---------------------------------------------------\n");
}

void menu(int sel) {
    //enum Modes oldMode = mode;
    //mode = sel;

    if( sel == MODE_CHANGE_EYE_POS || sel == MODE_CHANGE_REFERENCE_POS || sel == MODE_CHANGE_UP_POS || sel == MODE_CHANGE_LIGHT_POS || sel == MODE_ROTATE_OBJECT || sel == MODE_TRANSLATE_OBJECT )
        mode = sel;

    if(sel == MODE_CHANGE_CULLING) {
        cull = !cull;
        (cull ? glEnable : glDisable)(GL_CULL_FACE);
    }
    if(sel == MODE_CHANGE_WIREFRAME) {
        wireframe = !wireframe;
    }
    if(sel == MODE_CHANGE_PROJECTION) {
        orpro = !orpro;
    }
    if(sel == MODE_CHANGE_SHADING) {
        shading = !shading;
    }
    if (sel == MODE_FLOAT_AROUND) {
        cameraMotion = (cameraMotion >= 0 ? -1 : 0 );
        glutIdleFunc(cameraMotion ? floatAround : NULL);
    }
    if( sel == MODE_CHANGE_MATERIAL ) {
        pVCS[0][0] = 90.0;
        mater = (mater+1)%4;
    }
    if(sel == MODE_RESET) {
        reset();
    }
    if(sel == MODE_QUIT) {
        exit(0);
    }
    if (sel == MODE_PRINT_SYSTEM_STATUS) {
        print_sys_status();
    }

    createMenu();
    glutPostRedisplay();
}

void createMenu () {
    char *prompt;
    glutCreateMenu(menu);
    glutAddMenuEntry("Menu",-1); //-1 significa che non si vuole gestire questa riga
    glutAddMenuEntry("Press 0(WCS) to select the world or 1-3(OCS) to select an object", -1);
    glutAddMenuEntry("Press v,V to  Zoom", MODE_CHANGE_ZOOM);
    glutAddMenuEntry("-----------------------------------------------------",-1);

    prompt = ( mode == MODE_CHANGE_EYE_POS ) ? "(X)  Change Eye Point (x,y,z,X,Y,Z)" : "(  )  Change Eye Point (x,y,z,X,Y,Z)";
    glutAddMenuEntry(prompt, MODE_CHANGE_EYE_POS);

    prompt = ( mode == MODE_CHANGE_REFERENCE_POS ) ? "(X)  Change Reference Point (x,y,z,X,Y,Z)" : "(  )  Change Reference Point (x,y,z,X,Y,Z)";
    glutAddMenuEntry(prompt, MODE_CHANGE_REFERENCE_POS);

    prompt = ( mode == MODE_CHANGE_UP_POS ) ? "(X)  Change Up Vector (x,y,z,X,Y,Z)" : "(  )  Change Up Vector (x,y,z,X,Y,Z)";
    glutAddMenuEntry(prompt, MODE_CHANGE_UP_POS);

    prompt = ( mode == MODE_CHANGE_LIGHT_POS ) ? "(X)  Change Light Position (x,y,z,X,Y,Z)" : "(  )  Change Light Position (x,y,z,X,Y,Z)";
    glutAddMenuEntry(prompt, MODE_CHANGE_LIGHT_POS);

    prompt = ( mode == MODE_TRANSLATE_OBJECT ) ? "(X)  Translate Objects (x,y,z,X,Y,Z)" : "(  )  Translate Objects (x,y,z,X,Y,Z)";
    glutAddMenuEntry(prompt, MODE_TRANSLATE_OBJECT);
    prompt = ( mode == MODE_ROTATE_OBJECT ) ? "(X)  Rotate Objects (x,y,z,X,Y,Z)" : "(  )  Rotate Objects (x,y,z,X,Y,Z)";
    glutAddMenuEntry(prompt, MODE_ROTATE_OBJECT);
    glutAddMenuEntry("",-1);

    prompt = wireframe ? "(X) Wireframe" : "(  ) Wireframe";
    glutAddMenuEntry(prompt, MODE_CHANGE_WIREFRAME);

    prompt = shading ? "(X) Shading" : "(  ) Shading";
    glutAddMenuEntry(prompt, MODE_CHANGE_SHADING);

    prompt = cull ? "(X) Culling" : "(  ) Culling";
    glutAddMenuEntry(prompt, MODE_CHANGE_CULLING);
    glutAddMenuEntry("",-1);

    prompt = ( orpro ) ? "(X) Perspective" : "(  ) Perspective";
    glutAddMenuEntry(prompt, MODE_CHANGE_PROJECTION);

    prompt = ( !orpro ) ? "(X) Ortographic" : "(  ) Ortographic";
    glutAddMenuEntry(prompt, MODE_CHANGE_PROJECTION);
    glutAddMenuEntry("",-1);

    prompt = ( cameraMotion >= 0 ) ? "(  ) Camera Motion" : "(X) Camera Motion";
    glutAddMenuEntry(prompt, MODE_FLOAT_AROUND);
    prompt = "      Change Material";
    glutAddMenuEntry(prompt, MODE_CHANGE_MATERIAL);

    glutAddMenuEntry("",-1);
    glutAddMenuEntry("   Print system status", MODE_PRINT_SYSTEM_STATUS);
    glutAddMenuEntry("   Reset", MODE_RESET);
    glutAddMenuEntry("   Quit", MODE_QUIT);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display() {
    if (mater==0) { // ottone
        glLightfv(GL_LIGHT0, GL_AMBIENT, brass_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, brass_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, brass_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, brass_shininess);
    }
    if (mater==1) { // plastica rossa
        glLightfv(GL_LIGHT0, GL_AMBIENT, red_plastic_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red_plastic_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, red_plastic_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, red_plastic_shininess);
    }
    if (mater==2) { // smeraldo
        glLightfv(GL_LIGHT0, GL_AMBIENT, emerald_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, emerald_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, emerald_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, emerald_shininess);
    }
    if (mater==3) { // slate
        glLightfv(GL_LIGHT0, GL_AMBIENT, slate_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, slate_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, slate_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, slate_shininess);
    }

    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    if ( wireframe )
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // per riempire la facce

    if ( shading )
        glShadeModel (GL_SMOOTH);
    else
        glShadeModel (GL_FLAT);

    glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );


    glMatrixMode( GL_PROJECTION );

    glLoadIdentity ();

    if (orpro)
        gluPerspective( fovy, aspect, 1, 100);
    else
        glOrtho(-2.5f, 2.5f, -2.5f, 2.5f, -100.0f, 100.0f);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // matrice TV di vista
    gluLookAt( camE[0], camE[1], camE[2], camC[0], camC[1], camC[2], camU[0], camU[1], camU[2] );

    glLineWidth(1);

    glMultMatrixf(tbRotationMatrix);

    glRotatef( pRotations[MAX_P][0], 1.0, 0.0, 0.0);
    glRotatef( pRotations[MAX_P][1], 0.0, 1.0, 0.0);
    glRotatef( pRotations[MAX_P][2], 0.0, 0.0, 1.0);

    glRotatef(tbAngle, tbAxis[0], tbAxis[1], tbAxis[2]);

    glTranslated(pTranslations[MAX_P][0], pTranslations[MAX_P][1], pTranslations[MAX_P][2]);

    if (tbUpdateMatrix) {
        glPushMatrix();
        glLoadMatrixf(tbRotationMatrix);
        glRotatef(tbAngle, tbAxis[0], tbAxis[1], tbAxis[2]);
        glGetFloatv(GL_MODELVIEW_MATRIX, tbRotationMatrix);
        glPopMatrix();

        tbUpdateMatrix = 0;
        tbAngle = 0.0;
        tbAxis[0] = 0.0;
        tbAxis[1] = 0.0;
        tbAxis[1] = 0.0;
    }

    drawAxis( 2.0, 2 );

    // for each polygon
    for ( int p=0 ; p < MAX_P ; p++ ) {
        glPushMatrix();
        glTranslated(pTranslations[p][0], pTranslations[p][1], pTranslations[p][2]);


        drawAxis( 0.5, 0 );
        glScalef( pScales[p], pScales[p], pScales[p] );
        glRotatef( pRotations[p][0], 1.0, 0.0, 0.0);
        glRotatef( pRotations[p][1], 0.0, 1.0, 0.0);
        glRotatef( pRotations[p][2], 0.0, 0.0, 1.0);

        glPushMatrix();
        glRotatef( pVCS[p][0], 1.0, 0.0, 0.0);
        glRotatef( pVCS[p][1], 0.0, 1.0, 0.0);
        glRotatef( pVCS[p][2], 0.0, 0.0, 1.0);


        glCallList(listname+p);

        glPopMatrix();

        glPopMatrix();
    }



    //Draw some quadrics
//    gluSphere(myReusableQuadric, 1.0, 12, 12);

//    glTranslated(0.0, 0.0, 1.5);
//    gluCylinder(myReusableQuadric, 0.5, 0.2, 0.5, 12, 12);

    //gluDisk(myReusableQuadric, 0.5, 1.0, 10, 10);
    //gluPartialDisk( myReusableQuadric, 0.5, 1.0, 10, 10, 0.0, 45.0);
    //drawGluSlantCylinderWithCaps( 2.0, 1.0, 0.4, 8, 8 );

//  glutWireIcosahedron();
//	glutWireDodecahedron();
//  glutWireTeapot(1.0);
//	glutWireTorus(0.5, 1.0, 10, 10);
//	glutWireCone(1.0, 1.0, 10, 10);

    glutSwapBuffers();
}

int main (int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize( WindowWidth, WindowHeight );
    glutInitWindowPosition (0, 0);
    glutCreateWindow("Model Viewer ");

    // Set culling face GL_BACK, GL_FRONT_AND_BACK
    glCullFace(GL_FRONT);

    glutDisplayFunc(display);
    glutSpecialFunc(special);		// frecce up,down,left,right
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse); // pressione e rilascio
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutIdleFunc(floatAround);
    reset();
    init();

    createMenu();
    glutMainLoop();

    return -1;
}
