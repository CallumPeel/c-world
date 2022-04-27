#include "off.h"

// The time in milliseconds between timer ticks
#define TIMERMSECS 200
float startTime, prevTime;
static float days = 0.f, hrs = 0.f;
static float timeScale = 3600.f;  // 1s in reallife corresponds to 1hr in simulation


Model* model1;
Model* model2;
Model* sceneFloor;
float deltaAngle = 0.0f;
float xAngle = 0;
float yAngle = 0;
float xloc = 0;
float zloc = 0;
float sunRot = 0;
float myGravity = 0.00005f;

static float viewer[] = {
	0.0, 0.0, 1.0,  // location
	0.0, 0.0, 0,	// look at point
	0.0, 1.0, 0.0
};

enum {
	ORTHO2D, PERSPECTIVE, FRUSTUM, ORTHO
} mode = ORTHO;

void drawOrigin() {
	point3 origin[4] =
	{
		{0,-1,0},
		{1,-1,0},
		{0,0,0},
		{0,-1,1}
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

void gravity(Model* model) {
	model->velocity.y -= myGravity;
}

void animate() {

	printf("%.2f\n", model1->velocity.y);
	glutTimerFunc(TIMERMSECS, animate, 0);


	// 1. Get the elapsed time in seconds
	float currTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	float timeSincePrevFrame = currTime - prevTime;  // time elapsed since previous frame

	// 2. Update the hrs
	//    The division by 3600 is to convert time into hrs
	hrs = hrs + (timeSincePrevFrame / 3600) * timeScale;
	while (hrs > 24)
		hrs = hrs - 24;

	// 3. Update the days
	//    The division by 3600 is to convert time into days
	days = days + (timeSincePrevFrame / (3600 * 24)) * timeScale;
	while (days > 365)
		days = days - 365;
	gravity(model1);
	// 3. Make sure you save the current time to use it in the next call to this function
	prevTime = currTime;
	// if model hits ground and still has velocity, flip velocity and apply gravity
	if (model1->boundingBox.minY <= -1 && model1->velocity.y > 0) {
		model1->velocity.y * -1;
	}
	// if model hits ground and has no velocity then stop gravity?
	if (model1->boundingBox.minY <= -1 && model1->velocity.y <= 0) {
		model1->velocity.y = 0
	}

	glutPostRedisplay();
}

void init(void) {
	const char* fileName = "bone.off";
	model1 = readOFFFile(fileName);
	model2 = readOFFFile(fileName);
	sceneFloor = readOFFFile("floor.off");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	translateModel(model1, -0.8, 0, -10);
	translateModel(model2, 0.8, 0, -10);
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
	// Get the current time in seconds
	prevTime = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
	glutTimerFunc(TIMERMSECS, animate, 0);
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
	glScaled(100.0, 1.0, 100.0);
	glTranslated(0, -1, 0);
	glColor3f(0.6, 0.6, 0.6);
	drawModel(sceneFloor);
	glPopMatrix();

	// draw sun here
	glPushMatrix();
	// Translate world. Keeps sun relative to viewer.
	glTranslated(viewer[0], viewer[1], viewer[2]);
	// Rotate world.
	glRotated(sunRot,1,0,0);
	sunRot = sunRot + 0.1;
	int numOfCirclePoints = 100;
	float radiusOfSun = 3;
	float twoPi = 3.14159 * 2;
	glColor3f(1, 0.8, 0.0);
	glLineWidth(2.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < numOfCirclePoints; i++) {
		glVertex3f(
			// Take center of circle
			// 2pi / number of points gives theta
			// "i" marks which point in the circle
			// cos gives x value in the sin function
			// sin gives y value in the sin function
			radiusOfSun * cos(i * twoPi / numOfCirclePoints),
			radiusOfSun * sin(i * twoPi / numOfCirclePoints),
			-100
		);
	}
	glEnd();
	glPopMatrix();

	glColor3f(0.0, 0.9, 0.0);
	if (!isColliding()) {
		glColor3f(0.0, 0.0, 1.0);
		drawModel(model1);
		glColor3f(1.0, 0.0, 0.0);
		drawModel(model2);
	}
	else {
		glColor3f(0.0, 0.0, 0.0);
		drawModel(model1);
		glColor3f(0.0, 0.0, 0.0);
		drawModel(model2);
	}
	drawBoundingBox(*model1);
	drawBoundingBox(*model2);
	glPopMatrix();
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	float distX = viewer[0] - viewer[3];
	float distZ = viewer[2] - viewer[5];

	Point3D cameraLoc = { viewer[0], viewer[1], viewer[2] };
	Point3D cameraUpLoc = { viewer[0]+viewer[6], viewer[1]+viewer[7], viewer[2]+viewer[8] };
	Point3D lookatPoint = { viewer[3], viewer[4], viewer[5] };

	Point3D upVector = getVector(cameraLoc, cameraUpLoc);
	Point3D lookatVector = getVector(cameraLoc, lookatPoint);
	Normal sideDirection = getUnitNormal(cameraLoc, upVector, lookatVector);

	float dx = (viewer[3] - viewer[0]) * 400;
	float dz = (viewer[5] - viewer[2]) * 400;

	switch (key) {
		// Strafe left
	case 'a':
	case 'A':
		viewer[0] += dz;
		viewer[2] += -dx;
		viewer[3] += dz;
		viewer[5] += -dx;

		break;
		// Strafe right
	case 'd':
	case 'D':
		viewer[0] += -dz;
		viewer[2] += dx;
		viewer[3] += -dz;
		viewer[5] += dx;
		break;
		// forward
	case 'w':
	case 'W':
		viewer[0] -= distX * 250;
		viewer[2] -= distZ * 250;
		viewer[3] -= distX * 250;
		viewer[5] -= distZ * 250;
		break;
		// back
	case 's':
	case 'S':
		viewer[0] += distX * 250;
		viewer[2] += distZ * 250;
		viewer[3] += distX * 250;
		viewer[5] += distZ * 250;
		break;

		// model movements
			// Left
	case 'g':
		translateModel(model1, -0.03, 0, 0);
		break;
		// Right
	case 'h':
		translateModel(model1, 0.03, 0, 0);
		break;
		// Up
	case 't':
		translateModel(model1, 0, 0.03, 0);
		break;
		// Down
	case 'y':
		translateModel(model1, 0, -0.03, 0);
		break;
	case 'b':
		translateModel(model1, 0, 0, 0.03);
		break;
	case 'n':
		translateModel(model1, 0, 0, -0.03);
		break;
	case 'p':
		model1->velocity.y += 0.1;
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
	float speed = 0.002;
	x -= 250;
	xAngle += x;
	zloc = viewer[2] + ((sin(radians(-xAngle))) * speed);
	xloc = viewer[0] + ((-cos(radians(-xAngle))) * speed);

	viewer[3] = xloc;
	viewer[5] = zloc;

	glutWarpPointer(250, 250);
	glutPostRedisplay();
}
