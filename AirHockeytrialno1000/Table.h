#pragma once
#include <GL/glut.h>
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <iostream>
#include "RGBPixmap.h"

int window_width = 600;
int window_height = 700;

const float PI = 3.14159265359f;

void drawEmptyCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments); // get the current angle
        float x = r * cosf(theta); // calculate the x component
        float y = r * sinf(theta); // calculate the y component
        glVertex2f(x + cx, y + cy); // output vertex
    }
    glEnd();
}

void drawFilledCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Center of circle
    for (int i = 0; i <= num_segments; i++) {
        float angle = i / (float)num_segments * 2.0f * PI; // Angle of the segment
        float x = r * cosf(angle);
        float y = r * sinf(angle);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void DrawTable() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2i(50, 100);  // bottom-left corner
    glVertex2i(550, 100); // bottom-right corner
    glVertex2i(550, 700); // top-right corner
    glVertex2i(50, 700);  // top-left corner
    glEnd();

    // Draw the lines with different colors
    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.5f, 0.5f);
    glVertex2i(50, 0);  // bottom-left corner
    glColor3f(1.0f, 1.0f, 0.5f);
    glVertex2i(50, window_height);  // top-left corner

    glColor3f(0.5f, 0.5f, 1.0f);
    glVertex2i(550, 0); // bottom-right corner
    glColor3f(0.5f, 1.0f, 1.0f);
    glVertex2i(550, window_height); // top-right corner

    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2i(550, window_height / 2); // center of right line
    glVertex2i(50, window_height / 2); // center of left line

    glColor3f(0.5f, 1.0f, 0.5f);
    glVertex2i(550, window_height); // center of top-right line
    glVertex2i(50, window_height); // center of top-left line

    glVertex2i(550, 0); // center of bottom-right line
    glVertex2i(50, 0); // center of bottom-left line

    //Grey Borders
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);
    drawEmptyCircle(window_width / 2, window_height, 50, 40);
    drawEmptyCircle(window_width / 2, window_height / 2, 50, 40);
    drawEmptyCircle(window_width / 2, 0, 50, 40);
    glPopMatrix();
    //

    glColor3f(0.5f, 0.5f, 0.5f);
    glPointSize(7.5f);
    glBegin(GL_POINTS);
    glVertex2i(window_width / 2, window_height / 2); // center of table
    glEnd();
}