#include "off.h"
#include "models.h"

#define TIMERMSECS 16
#define MY_GRAVITY -0.98f

int winx;
int winy;

void animate(int oldTime);

void init(void);

void reshape(int w, int h);

bool isCollidingForTwo(Model* model1, Model* model2);

void gravity(float deltaTime);

void scene(void);

void keys(unsigned char key, int x, int y);

float radians(float deg);

void mouseMove(int x, int y);
