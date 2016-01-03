#include "DrawFunc.h"

char windowTitle[] = "Clock";
float RotateX = 0;
float RotateY = 0;
float depth = -5.5;
float width = 4.5;

#define ESC 27
#define ENTER 13

GLuint texture[1];

int LoadTexture()//the fuction to load a texture
{
	texture[0] = SOIL_load_OGL_texture("shine_wh2.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	
	if (texture[0] == 0)
		std::printf("SOIL loading error: '%s'\n", SOIL_last_result());

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	return true;
}
/* Initialize OpenGL Graphics */
void initGL() {

	if (!LoadTexture())								
	{
		return;									// if texture didn't load 
	}

	glEnable(GL_TEXTURE_2D);
	glClearColor(0.8f, 0.7f, 0.6f, 1.0f); // sets background color 
	glClearDepth(1.0f);                   // sets background depth to farthest
	glEnable(GL_DEPTH_TEST);   
	glDepthFunc(GL_LEQUAL);   
	glShadeModel(GL_SMOOTH);  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);     

	// Render 
	glLoadIdentity();   
	
	glTranslatef(0.0f, 0.0f, depth);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glRotatef(RotateY, 1.0f, 0.0f, 0.0f);
	glRotatef(RotateX, 0.0f, 1.0f, 0.0f);
	
	//implements the drawing of clock base using textures
	glBegin(GL_QUADS);                
	
	{
	// Top face (y = 1.0f)
	glColor3f(0.0f, 0.0f, 0.0f);     
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, 1.0f, -0.3f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, 1.0f, -0.3f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, 1.0f, 0.3f);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, 0.3f);

	// Bottom face (y = -1.0f)
	glColor3f(0.0f, 0.0f, 0.0f);    
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 0.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.3f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -0.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -0.3f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 1.0f, 1.0f);    
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.3f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 0.3f);

	// Back face (z = -1.0f)
	glColor3f(0.1f, 0.1f, 0.0f);     
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -0.3f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -0.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -0.3f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -0.3f);

	// Left face (x = -1.0f)
	glColor3f(0.1f, 0.1f, 0.0f);     
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -0.3f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -0.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.3f);

	// Right face (x = 1.0f)
	glColor3f(0.1f, 0.1f, 0.0f);    
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -0.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.3f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 0.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -0.3f);
	}
	glEnd();

	DrawArrow(depth, width, RotateX,RotateY);//draws partion of time
	time_t t = time(0);
	tm *lt = localtime(&t);

	DrawTime(lt, width, RotateX, RotateY, depth);//draws arrows
	
	glutSwapBuffers();  
	
}

void reshapeFunc(GLsizei width, GLsizei height)//options required for reshaping
{                
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();            
	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 0.1f, 80.0f);
}

void SpecialKeys(int key, int x, int y)//keys needed to rotate clock
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
						 RotateX += 20;
						 break;

	case GLUT_KEY_LEFT:
						  RotateX -= 20;
						  break;
	
	case GLUT_KEY_UP:
						RotateY += 20;
						break;
	
	case GLUT_KEY_DOWN:
						RotateY -= 20;
						break;

	}
}
GLvoid IdleFunc(GLvoid)
{
	display();
}
void keyboardPress(unsigned char key, int x, int y)//keys needed to close window and perform resizing of clock
{
	switch (key)
	{
	case ESC: case ENTER:
	{
			exit(0);
			break;
	}
	
	case('+') : case('='): 
	{
					depth += 2;
					width += 2;
					break;
	}
	case('-') : case('_') :
	{
					depth -= 2;
					width -= 2;
					break;
	}
	default:
		break;
	}
	glutPostRedisplay(); //marks the current window as needing to be redisplayed
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);//initializes glut      
	glutInitDisplayMode(GLUT_DOUBLE);// sets the initial display mode.
	glutInitWindowSize(640, 480);//sets window size using given parametrs
	glutInitWindowPosition(50, 50);//sets window position
	glutCreateWindow(windowTitle);//creates window with given title
	glutDisplayFunc(display);//sets the display callback for the current window  
	glutReshapeFunc(reshapeFunc);//sets the reshape callback for the current window  
	glutIdleFunc(IdleFunc);//sets the global idle callback
	glutKeyboardFunc(keyboardPress);//keyboard keys processing
	glutSpecialFunc(SpecialKeys);//processing other keys
	initGL();                 
	glutMainLoop();                 
	return 0;
}

