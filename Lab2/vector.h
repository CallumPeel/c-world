#pragma once
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

typedef struct GLfloat {
    GLfloat x;
    GLfloat y;
    GLfloat z;
 }Point3D;

typedef GLfloat Vector3D;

typedef struct Normal {
    float i, j, k;
} Normal;

Point3D multiplyByVector(Point3D point, float scalar);

float dotProduct(Point3D u, Point3D v);

Point3D getVector(Point3D p2, Point3D p1);

Normal getCrossProduct(Point3D U, Point3D V);

Normal getNormalFromPoints3D(Point3D p1, Point3D p2, Point3D p3);

Normal getUnitNormal(Point3D a, Point3D b, Point3D c);

typedef GLfloat point3[3];

float radians(float deg);