#include "off.h"

bool isOFF(FILE* file)
{
    char firstElement[50];
    fscanf(file, "%s", firstElement);
    return (strcmp("OFF", firstElement) == 0);
}

Model getModel(FILE* file)
{
    int NVerts;
    int NFaces;
    int NEdges;

    while (getc(file) != '\n');
    fscanf(file, "%d %d %d",
           &NVerts,
           &NFaces,
           &NEdges);

    int numVertices = NVerts;
    int numFaces = NFaces;

    Point3D* verts = malloc(sizeof(Point3D)*numVertices);
    printf("Loading vertices... \n");
    for (int i = 0; i < numVertices; i++)
    {
        Point3D vals;
        fscanf(file, "%f %f %f", &(vals.x), &(vals.y), &(vals.z));
        verts[i] = vals;
    }

    printf("Loading faces... \n");
    Face* faces = malloc(sizeof(Face)*numFaces);
    for (int i = 0; i < numFaces; i++)
    {
        Face vals;
        fscanf(file, "%*d %d %d %d", &(vals.i), &(vals.j), &(vals.k));
        faces[i] = vals;
    }

    printf("Building model... \n");
    Model model;
    model.NVerts = NVerts;
    model.NFaces = NFaces;
    model.NEdges = NEdges;
    model.vertices = verts;
    model.faces = faces;

    
    scaleModel(model);
    translateModel(model);
    return model;
}

Normal getNormalFromFace(Model model,Face face){
    Point3D i = model.vertices[face.i];
    Point3D j = model.vertices[face.j];
    Point3D k = model.vertices[face.k];

    Point3D U2 = getVector(j, i);
    Point3D V2 = getVector(k, i);
    return getCrossProduct(U2, V2);
}

Normal getNormalFromTriangle(Point3D i, Point3D j, Point3D k) {
    Point3D U2 = getVector(j, i);
    Point3D V2 = getVector(k, i);
    return getCrossProduct(U2, V2);
}

float getArea(Model model, Face face) {
    Normal f = getNormalFromFace(model, face);
    return (f.i+f.j+f.k)*0.5;
}

Model readOFFFile(const char* fileName)
{

    FILE* file = openFile(fileName);
    Model model;
    if (isOFF(file))
    {
        model = getModel(file);
    }
    else
    {
        printf("Incorrect data format for OFF file.\n");
    }
    return model;
}

void printVertices(Model model)
{
    for(int i = 0; i < model.NVerts; i++)
    {
        printf(
            "| x = %.2f | y = %.2f | z = %.2f |\n",
            model.vertices[i].x,
            model.vertices[i].y,
            model.vertices[i].z
        );
    }
    printf("\n");
}

void printFaces(Model model)
{
    for(int i = 0; i < model.NFaces; i++)
    {
        printf(
            "| x = %d | y = %d | z = %d |\n",
            model.faces[i].i,
            model.faces[i].j,
            model.faces[i].k
        );
    }
    printf("\n");
}

void printNormals(Model model){
    for (int i = 0; i < model.NFaces; i++)
    {
        Normal normal = getNormalFromFace(model, model.faces[i]);
        printf("%.8f | %.8f | %.8f\n", normal.i, normal.j, normal.k);
    }
}

float getPointX(Model model, int vertex){
    return model.vertices[vertex].x;
}

float getPointY(Model model, int vertex){
    return model.vertices[vertex].y;
}

float getPointZ(Model model, int vertex){
    return model.vertices[vertex].z;
}

Point3D getCenterOfMass(Model model) {
    Point3D c;
    float x = 0;
    float y = 0;
    float z = 0;
    for (int i = 0; i < model.NVerts; i++) {
        x += model.vertices[i].x;
        y += model.vertices[i].y;
        z += model.vertices[i].z;
    }
    c.x = x / model.NVerts;
    c.y = y / model.NVerts;
    c.z = z / model.NVerts;
    return c;
}

float getRadiusOfModel(Model model) {
    float rad1 = 0.0;
    float rad2 = 0.0;
    float x, y, z;
    Point3D c = getCenterOfMass(model);

    for (int i = 0; i < model.NVerts; ++i) {
        x = model.vertices[i].x - c.x;
        y = model.vertices[i].y - c.y;
        z = model.vertices[i].z - c.z;

        rad2 = sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));

        if (rad1 < rad2)
            rad1 = rad2;
    }
    return rad1;
}

void translateModel(Model model) {
    Point3D c = getCenterOfMass(model);
    for (int i = 0; i < model.NVerts; i++) {
        model.vertices[i].x = model.vertices[i].x - c.x;
        model.vertices[i].y = model.vertices[i].y - c.y;
        model.vertices[i].z = model.vertices[i].z - c.z;
    }
}

void scaleModel(Model model) {
    float ratio = 1.00 / getRadiusOfModel(model);
    for (int i = 0; i < model.NVerts; i++) {
        model.vertices[i].x = model.vertices[i].x * ratio;
        model.vertices[i].y = model.vertices[i].y * ratio;
        model.vertices[i].z = model.vertices[i].z * ratio;
    }
}
