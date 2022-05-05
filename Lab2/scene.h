#include "off.h"
#include "models.h"

void animate();

void init(void);

bool isCollidingForTwo(Model* model1, Model* model2);

void gravity();

void scene(void);

void keys(unsigned char key, int x, int y);

float radians(float deg);

void mouseMove(int x, int y);
