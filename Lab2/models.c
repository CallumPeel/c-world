#include "models.h"

void drawBone(Model** models,int modelNumber) {
	glPushMatrix();
	printf("%.2f\n", models[modelNumber]->position.y);
	glTranslatef(models[modelNumber]->position.x, models[modelNumber]->position.y, models[modelNumber]->position.z);
	drawModel(models[modelNumber]);
	drawBoundingBox(*models[modelNumber]);
	glPopMatrix();
}

void drawSun(float x, float y, float z, float* sunRot) {
	glPushMatrix();
	// Translate world. Keeps sun relative to viewer.
	glTranslated(x, y, z);
	// Rotate world.
	glRotated(*sunRot, 1, 0, 0);
	*sunRot = *sunRot + 0.1;
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
}

void drawFloor(Model* floor) {
	scaleModelXYZ(floor, 100.0f, 1.0, 100.0f);

	glClearColor(0.0, 0.5, 0.5, 0.0);
	glColor3f(1.0, 0.0, 0.0);
}

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