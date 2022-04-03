#include "off.h"

// for mouse things
float deltaAngle = 0.0f;

static float viewer[] = {
	0.0, 0.0, 1.0, // initial camera location
	0.0, 0.0, 0.0, // initial look at point
	0.0, 1.0, 0.0  // initial  upvector
};

enum {
	ORTHO2D, PERSPECTIVE, FRUSTUM, ORTHO
} mode = ORTHO;

void drawOrigin() {
	point3 origin[4] =
	{
		{0,0,0},  // 0
		{2.5,0,0},  // x
		{0,2.5,0},  // y
		{0,0,2.5}   // z
	};
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(origin[0]);
	glVertex3fv(origin[1]);
	glEnd();
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(origin[0]);
	glVertex3fv(origin[2]);
	glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(origin[0]);
	glVertex3fv(origin[3]);
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(5.0);

}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0); /* draw on white background */
	glColor3f(1.0, 0.0, 0.0);         /* draw in red */
	glLineWidth(2.0);                 /* draw using lines 2 pixels wide */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	mode = PERSPECTIVE;

	if (mode == ORTHO) {
		// This is orthographic projection
		GLdouble left = -1;
		GLdouble right = 1;
		GLdouble bottom = -1;
		GLdouble top = 1;
		GLdouble nearVal = 0.1;
		GLdouble farVal = 100;     // near and far clipping planes
		glOrtho(left, right,
			bottom, top,
			nearVal, farVal);

	}
	else if (mode == PERSPECTIVE) {
		//
		GLdouble fov = 45;		// degrees
		GLdouble aspect = 1;		// aspect ratio aspect = height/width
		GLdouble nearVal = 0.1;
		GLdouble farVal = 100;     // near and far clipping planes
		gluPerspective(fov, aspect, nearVal, farVal);

	}
	else if (mode == FRUSTUM) {
		GLdouble left = -1;
		GLdouble right = 1;
		GLdouble bottom = -1;
		GLdouble top = 1;
		GLdouble nearVal = .05;
		GLdouble farVal = 100;     // near and far clipping planes
		glFrustum(left, right,
			bottom, top,
			nearVal, farVal);
	}
}

void mydisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		viewer[0], viewer[1], viewer[2],      // eye location
		viewer[3], viewer[4], viewer[5],      // looking at
		viewer[6], viewer[7], viewer[8]       // up vector
	);
	glutSolidTeapot(0.5);
	drawOrigin();
	glutSwapBuffers();
	glFlush();
}

//gluLookAt(
//	camx,			camy,			camz,			// initial camera location
//	camx + camlx,	camy + camly,	camz + camlz,	// initial look at point
//	0.0f,			1.0f,			0.0f			// initial  upvector
void keys(unsigned char key, int x, int y)
{

	switch (key) {
	// Strafe left	
	case 'a':
		viewer[0] -= 0.2;
		viewer[3] -= 0.2;
		break;
	
	// Strafe left
	case 'd':
		viewer[0] += 0.2;
		viewer[3] += +0.2;
		break;
	case 'w':
		//THIS NEEDS FIXING
		viewer[0] += viewer[3];
		viewer[2] += viewer[5];
		break;
	case 's':
		viewer[0] -= viewer[3];
		viewer[2] -= viewer[5];
		break;
	case 'e':
		viewer[4] += 1;
		break;
	case 'c':
		viewer[4] -= 1;
		break;
	}

	if ((key == 'q') || (key == 'Q'))
		exit(0);

	glutPostRedisplay();
}

void mouseMove(int x, int y) {
	deltaAngle = x * 0.001f;
	viewer[3] = sin(deltaAngle);
	viewer[5] = -cos(deltaAngle);
	glutPostRedisplay();
}
