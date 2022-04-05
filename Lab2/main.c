#include <stdio.h>
#include <gl/freeglut.h>
#include "scene.h"

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// GL depth test
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Collision Detection");
	glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc(keys);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutDisplayFunc(scene);
	init();
	glutMainLoop();
	return 0;
}

