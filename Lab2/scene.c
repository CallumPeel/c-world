#include "off.h"

Model* model1;
Model* model2;
float deltaAngle = 0.0f;
float xAngle = 0;
float yAngle = 0;

static float viewer[] = {
	0.0, 0.0, 1.0, // loc
	0.0, 0.0, 0, // eye
	0.0, 1.0, 0.0  // up vec
};

enum {
	ORTHO2D, PERSPECTIVE, FRUSTUM, ORTHO
} mode = ORTHO;

void drawOrigin() {
	point3 origin[4] =
	{
		{0,0,0},
		{0.2,0,0},
		{0,0.2,0},
		{0,0,0.2}
	};
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2.0);
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
	const char* fileName = "bone.off";
	model1 = readOFFFile(fileName);
	model2 = readOFFFile(fileName);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	translateModelX(model1, -0.8, 0, 0);
	translateModelX(model2, 0.8, 0, 0);
	glClearColor(0.0, 0.5, 0.5, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	mode = PERSPECTIVE;

	if (mode == ORTHO) {
		GLdouble left = -1;
		GLdouble right = 1;
		GLdouble bottom = -1;
		GLdouble top = 1;
		GLdouble nearVal = 0.1;
		GLdouble farVal = 100;
		glOrtho(left, right,
			bottom, top,
			nearVal, farVal);
	}
	else if (mode == PERSPECTIVE) {
		GLdouble fov = 45;
		GLdouble aspect = 1;
		GLdouble nearVal = 0.1;
		GLdouble farVal = 100;
		gluPerspective(fov, aspect, nearVal, farVal);
	}
	else if (mode == FRUSTUM) {
		GLdouble left = -1;
		GLdouble right = 1;
		GLdouble bottom = -1;
		GLdouble top = 1;
		GLdouble nearVal = .05;
		GLdouble farVal = 100;
		glFrustum(left, right,
			bottom, top,
			nearVal, farVal);
	}
}

bool isColliding() {
	return
		(model1->boundingBox.minX < model2->boundingBox.maxX&& model1->boundingBox.maxX > model2->boundingBox.minX) &&
		(model1->boundingBox.minY < model2->boundingBox.maxY&& model1->boundingBox.maxY > model2->boundingBox.minY) &&
		(model1->boundingBox.minZ < model2->boundingBox.maxZ&& model1->boundingBox.maxZ > model2->boundingBox.minZ);
}

void scene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		viewer[0], viewer[1], viewer[2],
		viewer[3], viewer[4], viewer[5],
		viewer[6], viewer[7], viewer[8]
	);
	glPushMatrix();
		drawOrigin(); 
		glColor3f(0.0, 0.9, 0.0);
		if (!isColliding()) {
			drawModel(*model1);
			drawModel(*model2);
		}
		else {
			glColor3f(1.0, 0.0, 0.0);
		}
		drawBoundingBox(*model1);
		drawBoundingBox(*model2);
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

void keys(unsigned char key, int x, int y)
{
	float speed = 0.5;
	switch (key) {
	// Strafe left	
	case 'a':
		viewer[0] -= speed;
		//viewer[3] -= speed;
		break;

	// Strafe right
	case 'd':
		viewer[0] += speed;
		//viewer[3] += speed;
		break;
	// forward
	case 'w':
		viewer[2] -= speed;
		break;
	// back
	case 's':
		viewer[2] += speed;
		break;
	// up
	case 'e':
		viewer[1] += 1;
		break;
	// down
	case 'c':
		viewer[1] -= 1;
		break;

	// model movements
		// Left
	case 'g':
		translateModelX(model1, -0.03, 0, 0);
		break;
		// Right
	case 'h':
		translateModelX(model1, 0.03, 0, 0);
		break;
		// Up
	case 't':
		translateModelX(model1, 0, 0.03, 0);
		break;
		// Down
	case 'y':
		translateModelX(model1, 0, -0.03, 0);
		break;
	case 'b':
		translateModelX(model1, 0, 0, 0.03);
		break;
		// Down
	case 'n':
		translateModelX(model1, 0, 0, -0.03);
		break;
}
	if ((key == 'q') || (key == 'Q'))
		exit(0);
	glutPostRedisplay();
}

float radians(float deg) {
	return deg * 3.14159 / 180.0;
}

void mouseMove(int x, int y) {
	x = (x - 250)*-0.15;
	xAngle += x;
	viewer[5] = (sin(radians(xAngle))*250);
	viewer[3] = (-cos(radians(xAngle))*250);
	glutWarpPointer(250, 250);
	glutPostRedisplay();
}