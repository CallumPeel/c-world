#pragma once
#include <string.h>
#include <stdlib.h>
#include "fileHandler.h"
#include "vector.h"

typedef struct modelInfo {
    int NVerts, NFaces, NEdges;
}modelInfo;

typedef struct Face{
    int i, j, k;
}Face;

typedef struct BoundingBox {
    GLfloat minX, maxX, minY, maxY, minZ, maxZ;
}BoundingBox;

typedef struct Model {
    int NVerts, NFaces, NEdges;
    Point3D* vertices;
    Face* faces;
    modelInfo* edges;
    BoundingBox boundingBox;
    Point3D centre;
    float radius;
    Point3D position;
    Point3D rotation;
    Point3D velocity;
}Model;

bool isOFF(FILE* file);

Model* getModel(FILE* file);

Normal getNormalFromFace(Model model,Face face);

Normal getNormalFromTriangle(Point3D i, Point3D j, Point3D k);

float getArea(Model model, Face face);

Model* readOFFFile(const char* fileName);

void printVertices(Model model);

void printFaces(Model model);

void printNormals(Model model);

float getPointX(Model model, int vertex);

float getPointY(Model model, int vertex);

float getPointZ(Model model, int vertex);

Point3D getCenterOfMass(Model model);

float getRadiusOfModel(Model model);

void translateModelToCenter(Model* model);

void scaleModel(Model* model);

void scaleModelXYZ(Model* model, float x, float y, float z);

void drawModel(Model* model);

//void translateModel(Model* model, float x, float y, float z);

BoundingBox getBoundingBox(Model model);

void drawBoundingBox(Model model);