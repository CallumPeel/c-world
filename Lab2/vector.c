#include "vector.h"

Point3D multiplyByVector(Point3D point, float scalar)
{
    Point3D result;
    result.x = point.x * scalar;
    result.y = point.y * scalar;
    result.z = point.z * scalar;
    return result;
}

float dotProduct(Point3D u, Point3D v)
{
    return ((u.x * v.x) + (u.y * v.y) + (u.z * v.z));
}

Point3D getVector(Point3D p2, Point3D p1)
{
    Point3D U;
    U.x = (p2.x - p1.x);
    U.y = (p2.y - p1.y);
    U.z = (p2.z - p1.z);
    return U;
}

Normal getCrossProduct(Point3D U, Point3D V)
{
    Normal normal;
    normal.i = ((U.y * V.z) - (U.z * V.y));
    normal.j = ((U.z * V.x) - (U.x * V.z));
    normal.k = ((U.x * V.y) - (U.y * V.x));
    return normal;
}

Normal getNormalFromPoints3D(Point3D p1, Point3D p2, Point3D p3){
    Point3D U2 = getVector(p2, p1);
    Point3D V2 = getVector(p3, p1);
    return getCrossProduct(U2, V2);
}

float getMag(Normal normal) {
    float a, b, c;
    a = normal.i;
    b = normal.j;
    c = normal.k;

    if (normal.i != 0) a = (normal.i * normal.i);
    if (normal.j != 0) b = (normal.j * normal.j);
    if (normal.k != 0) c = (normal.k * normal.k);
    return sqrt(a + b + c);
}

Normal getUnitNormal(Point3D a, Point3D b, Point3D c) {
    Point3D E1 = getVector(b, a);
    Point3D E2 = getVector(c, a);
    Normal unitNormal = getCrossProduct(E1, E2);
    float mag = getMag(unitNormal);
    if (unitNormal.i != 0) unitNormal.i /= mag;
    if (unitNormal.j != 0) unitNormal.j /= mag;
    if (unitNormal.k != 0) unitNormal.k /= mag;
    return unitNormal;
}

float radians(float deg) {
    return deg * 3.14159f / 180.0f;
}