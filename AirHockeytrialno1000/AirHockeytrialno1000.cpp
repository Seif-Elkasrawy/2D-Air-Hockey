#include <GL/glut.h>
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <iostream>
#include "AirHockeytrialno1000.h"
#include "RGBPixmap.h"
#include "Table.h"


//Global variables
RGBpixmap MalletR;
RGBpixmap MalletB;

double Rz = 1;

UINT prevFrameTime = -1;
float currXPosition = 0;

int puckX = window_width / 2;
int puckY = window_height / 2;
int puckRadius = 20;

float puckSpeedX = 2.0f;
float puckSpeedY = 2.0f;

float R = 0.0f;

// Button dimensions
const int buttonWidth = 100;
const int buttonHeight = 50;

// Button position
const int buttonX = 250;
const int buttonY = 350;

int malletWidth = 24 * 5;
int malletHeight = 24 * 5;

bool MouseClicked = false;
bool MouseMoved = false;

int  X = window_width / 2 - 60;
int  Y = 20;

int screenSwitch = 0;

int lb = 0;
int ub = 1;

// (x, y) is the mouse-click location.
// button: GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, GLUT_MIDDLE_BUTTON
// state: GLUT_UP, GLUT_DOWN
//For Movement
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        cout << "Mouse Clicked  " << x << " , " << 700 - y << endl;
        MouseClicked = true;
        X = x - 60;
        Y = 700 - y - 60;
    }
}
// where (x, y) is the mouse location in Window's coordinates
void MouseX_Y(int x, int y) {
    if (MouseClicked == true) {
        X = x - 60;
        Y = 700 - y - 60;
    }
}

void DrawButton() {
    glColor3f(0.5, 0.0, 0.5);  // Set button color
    glBegin(GL_QUADS);
    glVertex2i(buttonX, buttonY);
    glVertex2i(buttonX + buttonWidth, buttonY);
    glVertex2i(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2i(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.0, 1.0, 0.5);  // Set text color
    glRasterPos2i(buttonX + 30, buttonY + 20);
    std::string buttonText = "Start";
    for (int i = 0; i < buttonText.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buttonText[i]);
    }
}

void Draw_Coordinates()
{
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    // X-axis
    glVertex3f(-100, 0, 0);
    glVertex3f(100, 0, 0);
    //Y-axis
    glVertex3f(0, -100, 0);
    glVertex3f(0, 100, 0);
    glEnd();
}

// Update function
void update(int value) {

    //int random = (rand() % (ub - lb + 1)) + lb;

    // Update puck position
    puckX += puckSpeedX;
    puckY += puckSpeedY;

    PuckCollision();

    MalletRCollision();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void MalletRCollision()
{
    // Collision detection with walls
    if (X > 425)
        X = 425;
    else if (X < 50) {
        X = 50;
    }
    if (Y > (window_height / 2 - 100))
        Y = (window_height / 2 - 100);
    else if (Y < 10) {
        Y = 10;
    }
}

double GetDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

void PuckCollision()
{
    boolean wallCollision = false;

    //to get normal of point of collision get center of puck and center of mallet
    // get angle between two vectors
    // get dot product of 2 vectors to get angle betwen them 
    // Collision detection with walls
    if (puckX + puckRadius > window_width - 50) {
        wallCollision = true;
        if (puckSpeedX > 0) {
            puckSpeedX *= -1;
        }
    }
    else if (puckX - puckRadius < 50) {
        wallCollision = true;
        if (puckSpeedX < 0) {
            puckSpeedX *= -1;
        }
    }

    if (puckY + puckRadius > window_height - 10) {
        wallCollision = true;
        if (puckSpeedY > 0) {
            puckSpeedY *= -1;
        }
    }
    else if (puckY - puckRadius < 10) {
        wallCollision = true;
        if (puckSpeedY < 0) {
            puckSpeedY *= -1;
        }
    }

    // Calculate the distance between the center of the puck and the mallet
    float distance = GetDistance(puckX, puckY, X + malletWidth / 2.0, Y + malletHeight / 2.0);
    cout << "distance = " << distance << endl;
    // Check if the puck collides with the mallet
    if ((distance <= puckRadius + malletWidth / 2.0 - 10))
    {
        R = 1.0f;

        // Calculate the angle between the puck and the mallet
        float angle = atan2(puckY - (Y + malletHeight / 2.0), puckX - (X + malletWidth / 2.0));

        // Calculate the new puck velocity after collision
        float newSpeedX = cos(angle) * sqrt(pow(puckSpeedX, 2) + pow(puckSpeedY, 2));
        float newSpeedY = sin(angle) * sqrt(pow(puckSpeedX, 2) + pow(puckSpeedY, 2));

        // Reverse the puck's direction
        puckSpeedX = newSpeedX;
        puckSpeedY = newSpeedY;
    }
    else
        R = 0.0f;

}

void SetTransformations() {
    //set up the logical coordinate system of the window: [-100, 100] x [-100, 100]

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, window_width, 0.0, window_height, -1, 1);
}

void DrawPuck()
{
    glBegin(GL_TRIANGLE_FAN);

    glColor3f(R, 0.5f, 1.0f);
    glVertex2f(puckX, puckY);
    for (int i = 0; i <= 360; i++) {
        float angle = 2.0f * PI * i / 360;
        float x = puckX + puckRadius * cos(angle);
        float y = puckY + puckRadius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void GameScreen() {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);

    SetTransformations();

    DrawTable();

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glPixelZoom(5, 5);
    glRasterPos2f(X, Y);
    MalletRCollision();
    MalletR.draw();
    glPopMatrix();

    glPushMatrix();
    glPixelZoom(5, 5);
    glRasterPos2f(window_width / 2 - 60, window_height - 140);
    MalletB.draw();
    glPopMatrix();

    // Draw puck
    DrawPuck();

    // Swap the buffers
    glFlush();
    glutSwapBuffers();
}

void StartScreen() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);

    SetTransformations();

    DrawButton();

    // Swap the buffers
    glFlush();
    glutSwapBuffers();
}

void OnDisplay() {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);

    SetTransformations();

    if (X + 60 >= 250 && X + 60 <= 350 && Y + 60 >= 350 && Y + 60 <= 400) {
        cout << "Mouse position " << X << " , " << Y << endl;
        screenSwitch = 1;
    }

    if (screenSwitch == 0) {
        StartScreen();
    }
    if (screenSwitch == 1) {
        GameScreen();
    }
}


/**
Creates the main window, registers event handlers, and
initializes OpenGL stuff.
*/
void InitGraphics(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //Create an 800x600 window with its top-left corner at pixel (100, 100)
    glutInitWindowPosition(0, 0); //pass (-1, -1) for Window-Manager defaults
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("2D Air Hockey");

    MalletR.readBMPFile("MalletR2.bmp");
    MalletB.readBMPFile("MalletB.bmp");

    glutMouseFunc(mouse);
    glutMotionFunc(MouseX_Y);

    glutTimerFunc(0, update, 0);

    glutDisplayFunc(OnDisplay);
    glutIdleFunc(OnDisplay);

    glutMainLoop();
}

int main(int argc, char* argv[]) {
    InitGraphics(argc, argv);
    return 0;
}
