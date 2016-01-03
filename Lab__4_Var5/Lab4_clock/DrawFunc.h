#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> 
#include <gl\SOIL.h>
#include <gl\gl.h>			
#include <gl\glu.h>	
#include <glut.h>  
#include <iostream>
#include <ctime>

void DrawArrow(int depth,int width, int X, int Y);//fuction to draw arrows of clock
void DrawTime(tm* lt,int width, int depth, int X, int Y);//fuction required to draw arrows which are responsible to show exact time
