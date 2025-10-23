#include "solver.h"
#include "include/queue.h"
#include "mouse.h"

// This function redirects function calls from mouse.c to the desired maze
// solving algorithm

Cell decMaze[16][16]; // y, x
int lastPosition[2] = {-1, -1};
int lastHeading = NORTH;
NodeStack *finalPath = {};

Action solver(Mouse *mouse) {
  getBestPath(mouse, decMaze);
  return floodFill(mouse);
}

// Simple algorithm; mouse goes straight until encountering a wall, then
// preferentially turns left

Action obstacleAvoider(Mouse *mouse) {
  if (getFrontReading(mouse) == 0)
    return FORWARD;
  else if (getLeftReading(mouse) == 0)
    return LEFT;
  else if (getRightReading(mouse) == 0)
    return RIGHT;
  else
    return LEFT;
}

// Left wall following algorithm
int turnedLeft = 0;
Action leftWallFollower(Mouse *mouse) {
  if (turnedLeft) {
    turnedLeft = 0;
    return FORWARD;
  } else if (getLeftReading(mouse) == 0) {
    turnedLeft = 1;
    return LEFT;
  } else if (getFrontReading(mouse) == 0)
    return FORWARD;
  else
    return RIGHT;
}

void printMaze(Cell maze[16][16], int size) {
  for (int x = size - 1; x >= 0; x--) {
    for (int y = 0; y < size; y++)
      printf("%3d", maze[x][y].distance);
    printf("\n");
  }
  printf("\n");
}

int goal[2] = {-1, -1};

enum State { GOING, BACKING, NOTHING };
enum State turn = GOING;

Action floodFill(Mouse *mouse) {
  int newCell = lastPosition[0] != mouse->x || lastPosition[1] != mouse->y;
  int newHeading = lastHeading != mouse->heading;
  int size = mouse->maze->size;
  int x = mouse->x;
  int y = mouse->y;
  switch (turn) {
  case GOING:

    goal[0] = mouse->maze->finish_pos[0];
    goal[1] = mouse->maze->finish_pos[1];
    break;
  case BACKING:
    goal[0] = 0;
    goal[1] = 0;
    break;
  case NOTHING:
    break;
  }
  if ((newCell || newHeading)) {
    switch (mouse->heading) {
    case NORTH:
      if (getFrontReading(mouse)) {
        decMaze[y][x].N = 1;
        if (y + 1 < size)
          decMaze[y + 1][x].S = 1;
      }
      if (getLeftReading(mouse)) {
        decMaze[y][x].W = 1;
        if (x - 1 >= 0)
          decMaze[y][x - 1].E = 1;
      }
      if (getRightReading(mouse)) {
        decMaze[y][x].E = 1;
        if (x + 1 < size)
          decMaze[y][x + 1].W = 1;
      }
      break;

    case SOUTH:
      if (getFrontReading(mouse)) {
        decMaze[y][x].S = 1;
        if (y - 1 >= 0)
          decMaze[y - 1][x].N = 1;
      }
      if (getLeftReading(mouse)) {
        decMaze[y][x].E = 1;
        if (x + 1 < size)
          decMaze[y][x + 1].W = 1;
      }
      if (getRightReading(mouse)) {
        decMaze[y][x].W = 1;
        if (x - 1 >= 0)
          decMaze[y][x - 1].E = 1;
      }
      break;

    case EAST:
      if (getFrontReading(mouse)) {
        decMaze[y][x].E = 1;
        if (x + 1 < size)
          decMaze[y][x + 1].W = 1;
      }
      if (getLeftReading(mouse)) {
        decMaze[y][x].N = 1;
        if (y + 1 < size)
          decMaze[y + 1][x].S = 1;
      }
      if (getRightReading(mouse)) {
        decMaze[y][x].S = 1;
        if (y - 1 >= 0)
          decMaze[y - 1][x].N = 1;
      }
      break;

    case WEST:
      if (getFrontReading(mouse)) {
        decMaze[y][x].W = 1;
        if (x - 1 >= 0)
          decMaze[y][x - 1].E = 1;
      }
      if (getLeftReading(mouse)) {
        decMaze[y][x].S = 1;
        if (y - 1 >= 0)
          decMaze[y - 1][x].N = 1;
      }
      if (getRightReading(mouse)) {
        decMaze[y][x].N = 1;
        if (y + 1 < size)
          decMaze[y + 1][x].S = 1;
      }
      break;
    }

    floodFillAlg(size, decMaze, goal);
    // printMaze(decMaze, size)

    if (mouse->x == mouse->maze->finish_pos[0] &&
        mouse->y == mouse->maze->finish_pos[1]) {
      turn = BACKING;
    } else if (x == goal[0] && y == goal[1]) {
      turn = NOTHING;
    }

    // Determine best move based on current heading and flood fill values
    int currentDistance = decMaze[y][x].distance;
    Action bestAction = IDLE;
    int bestDistance = currentDistance;

    // Check available directions and map them to mouse actions
    switch (mouse->heading) {
    case NORTH:
      if (!decMaze[y][x].N && y + 1 < size &&
          decMaze[y + 1][x].distance < bestDistance) {
        bestAction = FORWARD;
        bestDistance = decMaze[y + 1][x].distance;
      }
      if (!decMaze[y][x].W && x - 1 >= 0 &&
          decMaze[y][x - 1].distance < bestDistance) {
        bestAction = LEFT;
        bestDistance = decMaze[y][x - 1].distance;
      }
      if (!decMaze[y][x].E && x + 1 < size &&
          decMaze[y][x + 1].distance < bestDistance) {
        bestAction = RIGHT;
        bestDistance = decMaze[y][x + 1].distance;
      }
      break;

    case SOUTH:
      if (!decMaze[y][x].S && y - 1 >= 0 &&
          decMaze[y - 1][x].distance < bestDistance) {
        bestAction = FORWARD;
        bestDistance = decMaze[y - 1][x].distance;
      }
      if (!decMaze[y][x].E && x + 1 < size &&
          decMaze[y][x + 1].distance < bestDistance) {
        bestAction = LEFT;
        bestDistance = decMaze[y][x + 1].distance;
      }
      if (!decMaze[y][x].W && x - 1 >= 0 &&
          decMaze[y][x - 1].distance < bestDistance) {
        bestAction = RIGHT;
        bestDistance = decMaze[y][x - 1].distance;
      }
      break;

    case EAST:
      if (!decMaze[y][x].E && x + 1 < size &&
          decMaze[y][x + 1].distance < bestDistance) {
        bestAction = FORWARD;
        bestDistance = decMaze[y][x + 1].distance;
      }
      if (!decMaze[y][x].N && y + 1 < size &&
          decMaze[y + 1][x].distance < bestDistance) {
        bestAction = LEFT;
        bestDistance = decMaze[y + 1][x].distance;
      }
      if (!decMaze[y][x].S && y - 1 >= 0 &&
          decMaze[y - 1][x].distance < bestDistance) {
        bestAction = RIGHT;
        bestDistance = decMaze[y - 1][x].distance;
      }
      break;

    case WEST:
      if (!decMaze[y][x].W && x - 1 >= 0 &&
          decMaze[y][x - 1].distance < bestDistance) {
        bestAction = FORWARD;
        bestDistance = decMaze[y][x - 1].distance;
      }
      if (!decMaze[y][x].S && y - 1 >= 0 &&
          decMaze[y - 1][x].distance < bestDistance) {
        bestAction = LEFT;
        bestDistance = decMaze[y - 1][x].distance;
      }
      if (!decMaze[y][x].N && y + 1 < size &&
          decMaze[y + 1][x].distance < bestDistance) {
        bestAction = RIGHT;
        bestDistance = decMaze[y + 1][x].distance;
      }
      break;
    }

    if (bestAction == IDLE && turn != NOTHING) {
      bestAction = LEFT;
    }

    lastHeading = mouse->heading;
    lastPosition[0] = mouse->x;
    lastPosition[1] = mouse->y;

    switch (bestAction) {
    case RIGHT:
      push(&finalPath, LEFT);
      break;
    case LEFT:
      push(&finalPath, RIGHT);
      break;
    case FORWARD:
      push(&finalPath, FORWARD);
      break;
    default:
      break;
    }
    return bestAction;
  }
  return IDLE;
}

void floodFillAlg(int MazeSize, Cell maze[16][16], int startPoint[2]) {
  for (int y = 0; y < MazeSize; ++y) {
    for (int x = 0; x < MazeSize; ++x) {
      decMaze[y][x].distance = -1;
      decMaze[y][x].visited = 0;
    }
  }

  Queue *q = initQueue();

  enqueue(q, startPoint[0], startPoint[1]);

  decMaze[startPoint[1]][startPoint[0]].distance = 0;
  decMaze[startPoint[1]][startPoint[0]].visited = 1;

  while (!isEmpty(q)) {
    Node *c = dequeue(q);
    int currentDistance = decMaze[c->y][c->x].distance;

    // Norte
    if (c->y + 1 < MazeSize && !decMaze[c->y][c->x].N &&
        !decMaze[c->y + 1][c->x].visited) {
      enqueue(q, c->x, c->y + 1);
      decMaze[c->y + 1][c->x].distance = currentDistance + 1;
      decMaze[c->y + 1][c->x].visited = 1;
    }

    // Sur
    if (c->y - 1 >= 0 && !decMaze[c->y][c->x].S &&
        !decMaze[c->y - 1][c->x].visited) {
      enqueue(q, c->x, c->y - 1);
      decMaze[c->y - 1][c->x].distance = currentDistance + 1;
      decMaze[c->y - 1][c->x].visited = 1;
    }

    // Este
    if (c->x + 1 < MazeSize && !decMaze[c->y][c->x].E &&
        !decMaze[c->y][c->x + 1].visited) {
      enqueue(q, c->x + 1, c->y);
      decMaze[c->y][c->x + 1].distance = currentDistance + 1;
      decMaze[c->y][c->x + 1].visited = 1;
    }

    // Oeste
    if (c->x - 1 >= 0 && !decMaze[c->y][c->x].W &&
        !decMaze[c->y][c->x - 1].visited) {
      enqueue(q, c->x - 1, c->y);
      decMaze[c->y][c->x - 1].distance = currentDistance + 1;
      decMaze[c->y][c->x - 1].visited = 1;
    }
    free(c);
  }
};

NodeStack *getBestPath(Mouse *mouse, Cell maze[16][16]) {
  int size = mouse->maze->size;
  int currentValue =
      decMaze[mouse->maze->finish_pos[1]][mouse->maze->finish_pos[0]].distance;
  floodFillAlg(size, maze, mouse->maze->finish_pos);
  printMaze(maze, size);
  Cell coordinates[size * 8];
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (decMaze[i][j].E == decMaze[i][j].N == decMaze[i][j].S)
        if (decMaze[i][j].distance == currentValue - 1)
          currentValue = decMaze[i][j].distance;
    }
  };
  return NULL;
}

Action pathSolved(NodeStack **stack) {
  if (StackEmpty(stack))
    return IDLE;
  pop(stack);
  return (*stack)->action;
};
