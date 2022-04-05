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
    return sqrt((normal.i * normal.i) + (normal.j * normal.j) + (normal.k * normal.k));
}

Normal getUnitNormal(Point3D U, Point3D V) {
    Normal unitNormal = getCrossProduct(U, V);
    float mag = getMag(unitNormal);
    unitNormal.i /= mag;
    unitNormal.j /= mag;
    unitNormal.k /= mag;
    return unitNormal;
}
