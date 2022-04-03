#include <stdio.h>
#include <gl/freeglut.h>
#include "Lab5.h"

int main(int argc, char** argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Hello OpenGL");
	glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc(keys);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glutDisplayFunc(mydisplay);
	init();
	glutMainLoop();
	return 0;
}