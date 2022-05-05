#pragma once
#include "off.h"

void drawOrigin();

void drawBone();

void animate();

void init(void);

bool isCollidingForTwo(Model* model1, Model* model2);

bool isColliding(Model* model, int nModels);

void gravity();

void scene(void);

void keys(unsigned char key, int x, int y);

float radians(float deg);

void mouseMove(int x, int y);
