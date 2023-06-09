#include <GL/glut.h>
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include "AirHockeytrialno1000.h"
#include "RGBPixmap.h"
#include "Table.h"

#define FPS 30

//Global variables
RGBpixmap MalletR;
RGBpixmap MalletB;
RGBpixmap Galaxy;

double Rz = 1;

UINT prevFrameTime = -1;
float currXPosition = 0;

int initialTime = time(NULL), finalTime, frameCount = 0;

int puckX = window_width / 2;
int puckY = window_height / 2;
int puckRadius = 20;

float puckSpeedX = 5.0f;
float puckSpeedY = 5.0f;

float R = 0.0f;

// Button dimensions
const int buttonWidth = 100;
const int buttonHeight = 50;

int malletWidth = 24 * 5;
int malletHeight = 24 * 5;

string score1STR = " ", score2STR = " ";
int score1 = 0, score2 = 0;

bool MouseClicked = false;
bool MouseMoved = false;

int  X = window_width / 2 - 60;
int  Y = 20;

double TX = window_width / 2 - 60;
double TY = window_height - 140;

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

void OnSpecialKeyPress(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT://		Left function key
        TX -= 25;
        break;
    case GLUT_KEY_RIGHT://		Right function key
        TX += 25;
        break;
    case GLUT_KEY_UP://		Up function key
        TY += 25;
        break;
    case GLUT_KEY_DOWN://		Down function key
        TY -= 25;
        break;
    };
}

void DrawButton(int buttonX, int buttonY, string buttonText) {
    glColor3f(0.5, 0.0, 0.5);  // Set button color
    glBegin(GL_QUADS);
    glVertex2i(buttonX, buttonY);
    glVertex2i(buttonX + buttonWidth, buttonY);
    glVertex2i(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2i(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.0, 1.0, 0.5);  // Set text color
    glRasterPos2i(buttonX + 30, buttonY + 20);
    for (int i = 0; i < buttonText.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buttonText[i]);
    }
}

void DrawGoal(int scoreX, int scoreY1, int scoreY2) {

    glColor3f(1.0, 1.0, 1.0);  // Set text color
    glPushMatrix();

    glRasterPos2i(scoreX, scoreY1);
    for (char c : score2STR)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glRasterPos2i(scoreX, scoreY2);
    for (char c : score1STR)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glPopMatrix();
}

void DrawEnd(int buttonX, int buttonY, string buttonText, string win) {

    glRasterPos2i(window_width / 2 - 100, window_height - 150);

    for (char c : win)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glColor3f(0.5, 0.5, 0.5);  // Set button color
    glBegin(GL_QUADS);
    glVertex2i(buttonX, buttonY);
    glVertex2i(buttonX + 200, buttonY);
    glVertex2i(buttonX + 200, buttonY + buttonHeight);
    glVertex2i(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(1.0, 1.0, 0.0);  // Set text color
    glPushMatrix();
    glRasterPos2i(buttonX + 20, buttonY + 20);
    for (char c : buttonText)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glPopMatrix();
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
void Update(int value) {

    //UINT currTime = GetTickCount64();
    ////the first update?
    //if (prevFrameTime == -1) {
    //    prevFrameTime = currTime;
    //    return;
    //}
    //float dt = (currTime - prevFrameTime) / 1000.0;


    //Rz -= 30 * dt;

    //prevFrameTime = currTime;
    //int random = (rand() % (ub - lb + 1)) + lb;


    // Update puck position
    puckX += puckSpeedX;
    puckY += puckSpeedY;

    PuckCollision();

    MalletRCollision();
    MalletBCollision();

    glutPostRedisplay();
    glutTimerFunc(16, Update, 0);
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

void MalletBCollision()
{
    // Collision detection with walls
    if (TX > 425)
        TX = 425;
    else if (TX < 50) {
        TX = 50;
    }
    if (TY < (window_height / 2))
        TY = (window_height / 2);
    else if (TY > 690) {
        TY = 690;
    }
}

//Get distance between any two points
double GetDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

void PuckCollision()
{
    boolean wallCollision = false;

    
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

    if (puckY + puckRadius > window_height - 10 && !(puckX >= 250 && puckX <= 350)) {
        wallCollision = true;
        if (puckSpeedY > 0) {
            puckSpeedY *= -1;
        }
    }
    else if (puckY - puckRadius < 10 && !(puckX >= 250 && puckX <= 350)) {
        wallCollision = true;
        if (puckSpeedY < 0) {
            puckSpeedY *= -1;
        }
    }

    if (puckY + puckRadius < 5) {
        puckX = window_width / 2;
        puckY = window_height / 2;

        PuckCollision();
        score2++;
    }
    else if (puckY - puckRadius > window_height - 5) {
        puckX = window_width / 2;
        puckY = window_height / 2;

        PuckCollision();
        score1++;
    }

    // Getting Distance between Center of Puck and Center of Mallet to get the Normal Point of Collision
    // Getting angle between two vectors
    // Getting dot product of 2 vectors to get angle betwen them 

    // Puck and Red Mallet
    PuckMalletCollision(puckX, puckY, X, Y);
    // Puck and Blue Mallet
    PuckMalletCollision(puckX, puckY, TX, TY);

}

void PuckMalletCollision(int puckX, int puckY, int X, int Y)
{
    // Calculate the distance between the center of the puck and the mallet
    float distance = GetDistance(puckX, puckY, X + malletWidth / 2.0, Y + malletHeight / 2.0);
    // Check if the puck collides with the mallet
    if ((distance <= puckRadius + malletWidth / 2.0 - 10))
    {
        sndPlaySound(TEXT("Beer_bottles_hit_together.wav"), SND_ASYNC);


        //R = 1.0f;

        // Calculate the angle between the puck and the mallet
        float angle = atan2(puckY - (Y + malletHeight / 2.0), puckX - (X + malletWidth / 2.0));

        // Calculate the new puck velocity after collision
        float newSpeedX = cos(angle) * sqrt(pow(puckSpeedX, 2) + pow(puckSpeedY, 2));
        float newSpeedY = sin(angle) * sqrt(pow(puckSpeedX, 2) + pow(puckSpeedY, 2));

        // Reverse the puck's direction
        puckSpeedX = newSpeedX;
        puckSpeedY = newSpeedY;
    }
   /* else
        R = 0.0f;*/
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

void puckReset()
{
    /*puckX = window_width / 2;
    puckY = window_height / 2;*/

    score1STR = "0";
    score2STR = "0";

    X = window_width / 2 - 60;
    Y = 20;

    TX = window_width / 2 - 60;
    TY = window_height - 140;

   // puckSpeedX = 0.0f;
   // puckSpeedY = 0.0f;
}

void GameScreen() {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);

    SetTransformations();

    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glPixelZoom(1, 1);
    glRasterPos2f(0, 0);
    Galaxy.draw();
    glPopMatrix();

    DrawTable();

    int scoreX = 525, scoreY1 = 525, scoreY2 = 175;
    score1STR = std::to_string(score1);
    score2STR = std::to_string(score2);

    DrawGoal(scoreX, scoreY1, scoreY2);

    //glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glPixelZoom(5, 5);
    glRasterPos2f(X, Y);
    MalletR.draw();
    glPopMatrix();

    glPushMatrix();
    glPixelZoom(5, 5);
    glRasterPos2f(TX, TY);
    MalletB.draw();
    glPopMatrix();

    // Draw puck
    DrawPuck();

    // Swap the buffers
    glFlush();
    glutSwapBuffers();

    if (score1 == 6 || score2 == 6) {
       // puckReset();

        screenSwitch = 2;

        sndPlaySound(TEXT("cartoon_success.wav"), SND_ASYNC);
    }
}

void StartScreen() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);

    SetTransformations();

    string buttonStart = "Start";
    string buttonExit = "Exit";
    // Button position
    const int StartButtonX = 250;
    const int StartButtonY = 350;
    DrawButton(StartButtonX, StartButtonY, buttonStart);
    const int ExitButtonX = 250;
    const int ExitButtonY = 250;
    DrawButton(ExitButtonX, ExitButtonY, buttonExit);


    // Swap the buffers
    glFlush();
    glutSwapBuffers();
}

void EndScreen(float R, float B, string win) {
    glClearColor(R, 0.0, B, 0.0);
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);

    SetTransformations();

    //string win = "PlAYER 1 WINS!";
    string buttonReplay = "Play Again?";
    string buttonExit = "Exit";
    // Button position
    const int ReplayButtonX = 200;
    const int ReplayButtonY = 400;
    DrawEnd(ReplayButtonX, ReplayButtonY, buttonReplay, win);
    const int ExitButtonX = 200;
    const int ExitButtonY = 250;
    DrawEnd(ExitButtonX, ExitButtonY, buttonExit, win);


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
    if (X + 60 >= 250 && X + 60 <= 350 && Y + 60 >= 250 && Y + 60 <= 300 && screenSwitch == 0) {
        cout << "Mouse position " << X << " , " << Y << endl;
        exit(0);
    }

    if (screenSwitch == 2) {
        if (X + 60 >= 200 && X + 60 <= 400 && Y + 60 >= 400 && Y + 60 <= 450) {
            cout << "Mouse position " << X << " , " << Y << endl;
            screenSwitch = 1;
        }
        if (X + 60 >= 250 && X + 60 <= 400 && Y + 60 >= 250 && Y + 60 <= 300) {
            cout << "Mouse position " << X << " , " << Y << endl;
            exit(0);
        }
    }

    if (screenSwitch == 0) {
        StartScreen();
    }
    if (screenSwitch == 1) {
        GameScreen();
    }
    if (screenSwitch == 2 && score1 == 6) {
        EndScreen(1.0, 0.0, "PlAYER 1 WINS!");
    }
    if (screenSwitch == 2 && score2 == 6) {
        EndScreen(0.0, 1.0, "PlAYER 2 WINS!");
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

    glutSpecialFunc(OnSpecialKeyPress);

    glutTimerFunc(0, Update, 0);

    glutDisplayFunc(OnDisplay);
    glutIdleFunc(OnDisplay);

    glutMainLoop();
}

int main(int argc, char* argv[]) {
    InitGraphics(argc, argv);
    return 0;
}
