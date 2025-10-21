#ifndef SOLVER_H
#define SOLVER_H

#include "mouse.h"

Action solver(Mouse *mouse);
Action obstacleAvoider(Mouse *mouse);
Action pathSolved(Action path[], size_t size);
Action leftWallFollower(Mouse *mouse);
Action floodFill(Mouse *mouse);

#endif
