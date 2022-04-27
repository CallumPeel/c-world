#pragma once
#include "off.h"

void drawOrigin();

void animate();

void init(void);

bool isColliding();

void scene(void);

void keys(unsigned char key, int x, int y);

float radians(float deg);

void mouseMove(int x, int y);
