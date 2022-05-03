#include "off.h"

int numOfModels;
Model* model1;
Model* model2;
Model* sceneFloor;
Model* models[3];

float deltaAngle = 0.0f;
float xAngle = 0;
float yAngle = 0;
float xloc = 0;
float zloc = 0;
float sunRot = 0;
float myGravity = 0.002f;

Point3D windRes = { 0.99, 0, 0 };
Point3D windSpeed = { -0.0001, 0, 0 };

static float viewer[] = {
	0.0, 1.0, 1.0,  // location
	0.0, 1.0, 0.0,	// look at point
	0.0, 2.0, 0.0
};

enum {
	ORTHO2D, PERSPECTIVE, FRUSTUM, ORTHO
} mode = ORTHO;

void drawOrigin() {
	point3 origin[4] =
	{
		{0,0,0},
		{1,0,0},
		{0,1,0},
		{0,0,1}
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(origin[0]);
	glVertex3fv(origin[1]);
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(origin[0]);
	glVertex3fv(origin[2]);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(origin[0]);
	glVertex3fv(origin[3]);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(5.0f);

}

bool isCollidingForTwo(Model* modela, Model* modelb) {
	bool xco = (modela->boundingBox.minX + modela->position.x <= modelb->boundingBox.maxX + modelb->position.x && modela->boundingBox.maxX + modela->position.x >= modelb->boundingBox.minX + modelb->position.x);
	bool yco = (modela->boundingBox.minY + modela->position.y <= modelb->boundingBox.maxY + modelb->position.y && modela->boundingBox.maxY + modela->position.y >= modelb->boundingBox.minY + modelb->position.y);
	bool zco = (modela->boundingBox.minZ + modela->position.z <= modelb->boundingBox.maxZ + modelb->position.z && modela->boundingBox.maxZ + modela->position.z >= modelb->boundingBox.minZ + modelb->position.z);
	return (xco && yco && zco);
		
}

//bool isColliding(Model* model, int nModels) {
//	bool isColliding = false;
//	for (int i = 0; i < nModels; i++) {
//		if (
//			(model->boundingBox.minX < models[i]->boundingBox.maxX && model->boundingBox.maxX > models[i]->boundingBox.minX) &&
//			(model->boundingBox.minY < models[i]->boundingBox.maxY && model->boundingBox.maxY > models[i]->boundingBox.minY) &&
//			(model->boundingBox.minZ < models[i]->boundingBox.maxZ && model->boundingBox.maxZ > models[i]->boundingBox.minZ)
//			) {
//			isColliding = true;
//			break;
//		}
//	}
//	return isColliding;
//}

void gravity() {
	for (int i = 1; i < numOfModels; i++) {
		models[i]->velocity.y -= myGravity;
		bool iscol = isCollidingForTwo(models[i], sceneFloor);
		bool hasVelocity = models[i]->velocity.y <= 0;
		if (iscol && hasVelocity) {
			if (sqrt(models[i]->velocity.y * models[i]->velocity.y) > 0.01) {
			models[i]->velocity.y *= -0.7;
			}
			else {
				models[i]->velocity.y = 0;
			}
		}
	}
}

void wind() {
	for (int i = 1; i < numOfModels; i++) {
		if (models[i]->position.y > 0) {
			models[i]->velocity.x += windSpeed.x;
		}
	}
}

void windResistance() {
	for (int i = 1; i < numOfModels; i++) {
		if (models[i]->position.y > 0 && models[i]->velocity.x > 0) {
			models[i]->velocity.x *= windRes.x;
		}
		if (models[i]->velocity.y == 0) {
			models[i]->velocity.x = 0;
		}
	}
}

void animate() {
	gravity();
	wind();
	windResistance();
	for (int i = 1; i < numOfModels; i++) {
		models[i]->position.x += models[i]->velocity.x;
		models[i]->position.y += models[i]->velocity.y;
		models[i]->position.z += models[i]->velocity.z;
	}
}

void drawFloor() {
	scaleModelXYZ(sceneFloor, 100.0f, 1.0, 100.0f);

	glClearColor(0.0, 0.5, 0.5, 0.0);
	glColor3f(1.0, 0.0, 0.0);
}

void setObjects() {
	const char* fileName = "bone.off";
	numOfModels = 3;
	for (int i = 0; i < numOfModels; i++) {
		models[i] = malloc(sizeof(Model));
	}
	sceneFloor = readOFFFile("floor.off");
	model1 = readOFFFile(fileName);
	model2 = readOFFFile(fileName);

	models[0] = sceneFloor;

	models[1] = model1;
	models[1]->position.x += -2;
	models[1]->position.y += 2;
	models[1]->position.z += -10;

	models[2] = model2;
	models[2]->position.x += 2;
	models[2]->position.y += 2;
	models[2]->position.z += -10;

	models[1]->velocity.x = 0.1;
	drawFloor();
}

void init(void) {

	setObjects();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

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

void drawBlueBone() {
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	printf("%.2f\n", models[1]->position.y);
	glTranslatef(models[1]->position.x, models[1]->position.y, models[1]->position.z);
	drawModel(model1);
	drawBoundingBox(*model1);
	glPopMatrix();
}

void drawRedBone() {
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(models[2]->position.x, models[2]->position.y, models[2]->position.z);
	drawModel(model2);
	drawBoundingBox(*model2);
	glPopMatrix();
}

void setScene() {
	glPushMatrix();
		drawOrigin();
		glTranslated(0, -1, 0);
		glColor3f(0.6f, 0.6f, 0.6f);
		drawModel(sceneFloor);
	glPopMatrix();
	// draw sun here
	glPushMatrix();
		// Translate world. Keeps sun relative to viewer.
		glTranslated(viewer[0], viewer[1], viewer[2]);
		// Rotate world.
		glRotated(sunRot, 1, 0, 0);
		sunRot = sunRot + 0.1;
		int numOfCirclePoints = 100;
		float radiusOfSun = 3;
		float twoPi = 3.14159f * 2;
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
	drawBlueBone();
	drawRedBone();
}

void scene(void) {
	animate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		viewer[0], viewer[1], viewer[2],
		viewer[3], viewer[4], viewer[5],
		viewer[6], viewer[7], viewer[8]
	);
	setScene();
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

	case 'p':
		model1->velocity.y += 0.05f;
		break;
	case '[':
		model1->velocity.y += 0.05f;
		model1->velocity.x += 0.05f;
		break;
	case 'o':
		model1->velocity.y += 0.05f;
		model1->velocity.x -= 0.05f;
		break;
	}
	if ((key == 'q') || (key == 'Q'))
		exit(0);
	glutPostRedisplay();
}

float radians(float deg) {
	return deg * 3.14159f / 180.0f;
}

void mouseMove(int x, int y) {
	float speed = 0.002f;
	x -= 250;
	xAngle += x;
	zloc = viewer[2] + ((sin(radians(-xAngle))) * speed);
	xloc = viewer[0] + ((-cos(radians(-xAngle))) * speed);

	viewer[3] = xloc;
	viewer[5] = zloc;

	glutWarpPointer(250, 250);
	glutPostRedisplay();
}
