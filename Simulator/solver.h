#ifndef SOLVER_H
#define SOLVER_H

#include "mouse.h"
#include "./include/stack.h"

Action solver(Mouse *mouse);
Action obstacleAvoider(Mouse *mouse);
Action pathSolved(NodeStack** stack);
Action leftWallFollower(Mouse *mouse);
Action floodFill(Mouse *mouse);

#endif
