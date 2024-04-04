/*********
   CTIS164 - Project (HW1) Name: Planes on the sky
----------
STUDENT : Emirhan Yel 22103407
SECTION :01
HOMEWORK: 1
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:

// 1)We can move the plane with F1(down), F2(down-left), F3(down-right). 3 different direction to move
// 2)We can enlarge and shrink the plane with "e" and "s"
   3)We can change the colour of balloons on the screen with "SPACE" button.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include<time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


//Modes to move the shape to differen directions
#define STOP                0
#define DIRECTION           1
#define DIRECTION2          2
#define DIRECTION3          3



/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int mousex, mousey;               // shows where the mouse pointr is
int xshp, yshp;       // shape locations
int widthshp = 20, heightshp = 12; // inital size of the shape



int shapecnt = 0;             // counts the shapes on the screen
int r = 0;

int mode = STOP;          // if ACTION=it moves, if STOP:it stops
bool activeTimer = false; // move down timer
bool activeTimer2 = false; // move down-left timer
bool activeTimer3 = false; // move down-right timer




typedef struct {
    int cntt,
        sx, sy;
}shp_t;

shp_t myshape[10];


//BALLOON
double BalX1 = 420, BalY1 = -220; //initial position of first balloon
int Balradius=35; //radius of balloons
double BalX2 = -420, BalY2 = -220; //initial position of second balloon

int c1 = 0, c2 = 102, c3 = 0; // initial color of balloons

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void balloon() {

    // string of baloon1
    glLineWidth(3);
    glColor3f(192, 192, 192); 
    glBegin(GL_LINES);
    glVertex2f(BalX1, BalY1-35);
    glVertex2f(BalX1, BalY1-25-80);
    glEnd();


    

    // Baloon1 circle
    glLineWidth(1);
    glColor3ub(c1, c2, c3);
    circle(BalX1, BalY1, Balradius);



    // string of baloon2
    glLineWidth(3);
    glColor3f(192, 192, 192);
    glBegin(GL_LINES);
    glVertex2f(BalX2, BalY2 - 35);
    glVertex2f(BalX2, BalY2 - 25 - 80);
    glEnd();


    

    // Baloon2 circle
    glLineWidth(1);
    glColor3ub(c1, c2, c3);
    circle(BalX2, BalY2, Balradius);

}

void backgrnd() {

    //Name,Surname,Id Box
    glColor3ub(20, 20, 20);
    glRectf(-500, 350, -340, 260);
    glColor3ub(0, 110.0, 0);
    glRectf(-500, 350, -350, 270);
    glColor3ub(239.0, 176.0, 0);
    vprint(-470, 320, GLUT_BITMAP_9_BY_15, "Emirhan Yel");
    vprint(-470, 295, GLUT_BITMAP_9_BY_15, "22103407");


    //Columns and their parts

         // DRAW A RECTANGLE
    glColor3f(78.0 / 256, 27.0 / 256, 0);
    glRectf(-360, -350, -250, 0);

    // DRAW A RECTANGLE
    glColor3f(78.0 / 256, 27.0 / 256, 0);
    glRectf(250, -350, 360, 0);

    // DRAW A RECTANGLE
    glColor3f(20.0 / 256, 11.0 / 256, 67.0 / 256);
    glRectf(-250, -125, -120, -225);

    // DRAW A RECTANGLE
    glColor3f(20.0 / 256, 11.0 / 256, 67.0 / 256);
    glRectf(120, -125, 250, -225);


}

void menu() {
    glColor3ub(100, 30, 0);
    vprint(-5, 320, GLUT_BITMAP_9_BY_15, "F1: Move the plane (down)");
    vprint(-5, 305, GLUT_BITMAP_9_BY_15, "F2: Move the plane (down-left)");
    vprint(-5, 290, GLUT_BITMAP_9_BY_15, "F3: Move the plane (down-right)");
    vprint(-5, 275, GLUT_BITMAP_9_BY_15, "Click on the screen to create a shape (maximum 10 shape)");
   


    glColor3ub(100, 30, 0);
    vprint(-215, -285, GLUT_BITMAP_9_BY_15, "Press SPACE button to change the colour of balloon");
    vprint(-155, -305, GLUT_BITMAP_9_BY_15, "Press E button to enlarge the plane");
    vprint(-155, -320, GLUT_BITMAP_9_BY_15, "Press S button to shrink the plane");


}

void displayshape()
{

    for (int n = 1; n <= 10; n++) {

        xshp = myshape[n - 1].sx;
        yshp = myshape[n - 1].sy;


        if (!(yshp >= myshape[n].sy && yshp <= myshape[n].sy && xshp >= myshape[n].sx && xshp <= myshape[n].sx)) {


            //2 triangles
            glBegin(GL_TRIANGLES);
            glColor3ub(243.0 / 256, 242.0 / 256, 240.0 / 256);
            glVertex2f(xshp, yshp);
            glVertex2f(xshp - widthshp / 2, yshp - heightshp / 2);
            glVertex2f(xshp - widthshp / 2 - 40, yshp + 15);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3ub(243.0 / 256, 242.0 / 256, 240.0 / 256);
            glVertex2f(xshp, yshp);
            glVertex2f(xshp + widthshp / 2, yshp - heightshp / 2);
            glVertex2f(xshp + widthshp / 2 + 40, yshp + 15);
            glEnd();




            //1 connection triangle

            glBegin(GL_TRIANGLES);
            glColor3ub(243.0 / 256, 242.0 / 256, 240.0 / 256);
            glVertex2f(xshp, yshp);
            glVertex2f(xshp - widthshp / 2, yshp - heightshp / 2);
            glVertex2f(xshp + widthshp / 2, yshp - heightshp / 2);
            glEnd();

            //quad
            glBegin(GL_QUADS);
            glColor3ub(243, 242, 240);
            glVertex2f(xshp + widthshp / 2, yshp - heightshp / 2);
            glVertex2f(xshp - widthshp / 2, yshp - heightshp / 2);
            glVertex2f(xshp - widthshp / 2, yshp - heightshp / 2 - 20);
            glVertex2f(xshp + widthshp / 2, yshp - heightshp / 2 - 20);
            glEnd();

            //rectangle
            glColor3f(243.0 / 256, 242.0 / 256, 240.0 / 256);
            glRectf(xshp - widthshp / 2 - 25, yshp - heightshp / 2 - 20, xshp + widthshp / 2 + 25, yshp - heightshp / 2 - 20 - 13);




            //wing triangle
            glBegin(GL_TRIANGLES);
            glColor3ub(243.0 / 256, 242.0 / 256, 240.0 / 256);
            glVertex2f(xshp - widthshp / 2 - 25, yshp - heightshp / 2 - 20);
            glVertex2f(xshp - widthshp / 2 - 25, yshp - heightshp / 2 - 20 - 13);
            glVertex2f(xshp - widthshp / 2 - 25 - 20, yshp - heightshp / 2 - 20 + 10);


            glBegin(GL_TRIANGLES);
            glColor3ub(243.0 / 256, 242.0 / 256, 240.0 / 256);
            glVertex2f(xshp + widthshp / 2 + 25, yshp - heightshp / 2 - 20);
            glVertex2f(xshp + widthshp / 2 + 25, yshp - heightshp / 2 - 20 - 13);
            glVertex2f(xshp + widthshp / 2 + 25 + 20, yshp - heightshp / 2 - 20 + 10);
            glEnd();





            //head triangle
            glBegin(GL_TRIANGLES);
            glColor3ub(243.0 / 256, 242.0 / 256, 240.0 / 256);
            glVertex2f(xshp - widthshp / 2, yshp - heightshp / 2 - 20 - 13);
            glVertex2f(xshp + widthshp / 2, yshp - heightshp / 2 - 20 - 13);
            glVertex2f(xshp, yshp - heightshp / 2 - 20 - 13 - 40);
            glEnd();









        }
    }

}


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(51.0 / 256, 150.0 / 256, 255.0 / 256, .0 / 256);
    glClear(GL_COLOR_BUFFER_BIT);


    backgrnd();
    balloon();
    menu();
    displayshape();

    if (BalY1 + Balradius < winHeight / 3)
        BalY1+=0.1;

    if (BalY2 + Balradius < winHeight / 3)
        BalY2+=0.1; 

    for (int m = 0; m < 10; m++) {
        if (myshape[m].sy < -340)
            myshape[m].sy = 370;

        if (myshape[m].sx < -560)
            myshape[m].sx = 550;

        if (myshape[m].sx > 560)
            myshape[m].sx = -550;

    }


    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);


    if (key == 'e' || key == 'E') {

        if (widthshp < winWidth / 9)
        {
            widthshp += 8;
            heightshp += 4;

        }
    }


    if (key == 's' || key == 'S') {

        if (widthshp > winWidth / 20)
        {
            widthshp -= 8;
            heightshp -= 4;

        }
    }


    if (key == ' ') {
        c1 = rand() % 256;
        c2= rand() % 256;
        c3 = rand() % 256;
    }



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }


    if (key == GLUT_KEY_F1) {
        if (mode == STOP) {
            mode = DIRECTION;
            activeTimer = true;
        }
        else {
            mode = STOP;
            activeTimer = false;
        }
    }

    if (key == GLUT_KEY_F2) {
        if (mode == STOP) {
            mode = DIRECTION2;
            activeTimer2 = true;
        }
        else {
            mode = STOP;
            activeTimer2 = false;
        }
    }


    if (key == GLUT_KEY_F3) {
        if (mode == STOP) {
            mode = DIRECTION3;
            activeTimer3 = true;
        }
        else {
            mode = STOP;
            activeTimer3 = false;
        }
    }



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    xshp = x - winWidth / 2;
    yshp = winHeight / 2 - y;


    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && shapecnt < 10) {



        myshape[r].cntt = shapecnt;
        myshape[r].sx = xshp;
        myshape[r].sy = yshp;

        shapecnt++;
        r++;




    }







    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.
    

    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.

    mousex = x - winWidth / 2;
    mousey = winHeight / 2 - y;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.


    if (activeTimer) {
        // animate the current shape until it touches the edge of the window.


        for (int k = 0; k < 10; k++)
            myshape[k].sy -= 5;

        //myshape[l].sy -= 9;


    }

    if (activeTimer2)
        for (int l = 0; l < 10; l++) {
            myshape[l].sx -= 5;
            myshape[l].sy -= 5;
        }


    if (activeTimer3)
        for (int m = 0; m < 10; m++) {
            myshape[m].sx += 5;
            myshape[m].sy -= 5;
        }








    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Planes on the Sky");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}