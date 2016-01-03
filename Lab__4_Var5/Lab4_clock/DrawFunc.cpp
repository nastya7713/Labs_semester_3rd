#pragma once
#include "DrawFunc.h"


void DrawArrow(int depth,int width, int X, int Y)
{
	long double M_PI = acos(-1.0);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, depth);  

	glRotatef(Y, 1.0f, 0.0f, 0.0f);
	glRotatef(X, 0.0f, 1.0f, 0.0f);
	glLineWidth(width-1);
	glBegin(GL_LINES);
	
	for (int i = 0; i < 12; ++i)
	{
		//using that sin and cos are the periodic creates a partion
		float x = sin(2 * M_PI / 12 * i); 
		float y = cos(2 * M_PI / 12 * i);
		glVertex3f(0.8 * x, 0.8 * y, 0.29f);
		glVertex3f(0.74 * x, 0.74 * y, 0.29f);
	}
	
}
void DrawTime(tm* lt, int width, int depth, int X, int Y)
{
	long double M_PI = acos(-1.0);
	int h = lt->tm_hour;//initialize h with hours
	int m = lt->tm_min;//initialize m with minutes
	int s = lt->tm_sec;//initialize s wit seconds
	
	//hour arrow
	float x = sin(2 * M_PI * (h * 60 + m) / 12 / 60);
	float y = cos(2 * M_PI * (h * 60 + m) / 12 / 60);
	
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, depth);

	glRotatef(Y, 1.0f, 0.0f, 0.0f);
	glRotatef(X, 0.0f, 1.0f, 0.0f);
	
	glLineWidth(width);
	glBegin(GL_LINES);

	//draws a line of required colour between two vertexes
	glColor3f(0.0F, 0.0F, 0.0F);
	glVertex3f(0.0f, 0.0f, 0.29f);
	glVertex3f(0.4 * x, 0.4 * y, 0.29f);
	
	//minute arrow
	x = sin(2 * M_PI * m / 60);
	y = cos(2 * M_PI * m / 60);
	
	//draws a line of required colour between two vertexes
	glColor3f(0.0F, 0.0F, 0.0F);
	glVertex3f(0.0f, 0.0f, 0.29f);
	glVertex3f(0.7 * x, 0.7 * y, 0.29f);
	
	glColor3f(0.0F, 0.0F, 0.0F);
	
	//second arrow
	x = sin(2 * M_PI * s / 60);
	y = cos(2 * M_PI * s / 60);
	
	//draws a line of required colour between two vertexes
	glColor3f(0.0F, 0.0F, 0.0F);
	glVertex3f(0.0f, 0.0f, 0.29f);
	glVertex3f(0.76* x, 0.76 * y, 0.29f);

	glEnd();
}