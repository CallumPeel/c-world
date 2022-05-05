#include "scene.h"

int numOfModels;
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

bool isCollidingForTwo(Model* modela, Model* modelb) {
	bool xco = (modela->boundingBox.minX + modela->position.x <= modelb->boundingBox.maxX + modelb->position.x && modela->boundingBox.maxX + modela->position.x >= modelb->boundingBox.minX + modelb->position.x);
	bool yco = (modela->boundingBox.minY + modela->position.y <= modelb->boundingBox.maxY + modelb->position.y && modela->boundingBox.maxY + modela->position.y >= modelb->boundingBox.minY + modelb->position.y);
	bool zco = (modela->boundingBox.minZ + modela->position.z <= modelb->boundingBox.maxZ + modelb->position.z && modela->boundingBox.maxZ + modela->position.z >= modelb->boundingBox.minZ + modelb->position.z);
	return (xco && yco && zco);
		
}

void gravity() {
	for (int i = 1; i < numOfModels; i++) {
		models[i]->velocity.y -= myGravity;
		bool iscol = isCollidingForTwo(models[i], models[0]);
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

void setObjects() {
	const char* fileName = "bone.off";
	numOfModels = 3;
	*models = malloc(sizeof(Model)*numOfModels);
	models[0] = readOFFFile("floor.off");
	models[1] = readOFFFile(fileName);
	models[1]->position.x += -2;
	models[1]->position.y += 2;
	models[1]->position.z += -10;
	models[1]->velocity.x = 0.1;
	models[2] = readOFFFile(fileName);
	models[2]->position.x += 2;
	models[2]->position.y += 2;
	models[2]->position.z += -10;
}

void setCamera() {
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

void init(void) {
	setCamera();
	setObjects();
}

void setScene() {
	drawOrigin();
	drawFloor(models[0]);

	glPushMatrix();
		glTranslated(0, -1, 0);
		glColor3f(0.6f, 0.6f, 0.6f);
		drawModel(models[0]);
	glPopMatrix();
	
	drawSun(viewer[0], viewer[1], viewer[2], &sunRot);

	glColor3f(0.0, 0.0, 1.0);
	drawBone(models, 1);

	glColor3f(1.0, 0.0, 0.0);
	drawBone(models, 2);
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
		models[1]->velocity.y += 0.05f;
		break;
	case '[':
		models[1]->velocity.y += 0.05f;
		models[1]->velocity.x += 0.05f;
		break;
	case 'o':
		models[1]->velocity.y += 0.05f;
		models[1]->velocity.x -= 0.05f;
		break;
	}
	if ((key == 'q') || (key == 'Q'))
		exit(0);
	glutPostRedisplay();
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
