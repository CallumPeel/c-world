#include <stdio.h>
#include <gl/freeglut.h>
#include "scene.h"
#define WIN_WIDTH 500
#define WIN_HEIGHT 500

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(
		(glutGet(GLUT_SCREEN_WIDTH) / 2) - (WIN_WIDTH / 2),
		(glutGet(GLUT_SCREEN_HEIGHT) / 2) - (WIN_HEIGHT / 2)
	);
	glutCreateWindow("Callum's Happy Fun Times");
	glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc(keys);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(250, 250);

	init();
	glutTimerFunc(TIMERMSECS, animate, 1);
	glutReshapeFunc(reshape);
	glutDisplayFunc(scene);

	glutMainLoop();
	return 0;
}

