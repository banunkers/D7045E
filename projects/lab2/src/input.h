#include "types.h"

#ifndef INPUT_H
#define INPUT_H

PointSet readPointsFromFile();
PointSet randomPointSet(int numPoints);
int validatePointSet(PointSet &set);

#endif