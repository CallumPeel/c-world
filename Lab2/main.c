#include <stdio.h>
#include <gl/freeglut.h>
#include "Lab5.h"

Model model;

int main(int argc, char** argv)
{
	const char* fileName = "bone_normalized_aligned.off";
	model = readOFFFile(fileName);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(110, 0);
	glutCreateWindow("Hello OpenGL");
	glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc(keys);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glutDisplayFunc(mydisplay);
	init();
	glutMainLoop();
	return 0;
}