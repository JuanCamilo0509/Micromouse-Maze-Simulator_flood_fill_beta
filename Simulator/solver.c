#include "solver.h"
#include "mouse.h"

// This function redirects function calls from mouse.c to the desired maze
// solving algorithm

Action solver(Mouse *mouse) {

  Action mazeSolvedPath[4] = {RIGHT, FORWARD, FORWARD, LEFT};
  return pathSolved(mazeSolvedPath, 4);
}

// Simple algorithm; mouse goes straight until encountering a wall, then
// preferentially turns left

int counter = -1;
Action pathSolved(Action directions[], size_t size) {
  counter++;
  if (counter >= size)
    return IDLE;
  return directions[counter];
}

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

typedef struct Node {
  int x, y, distance;
  struct Node *next;
} Node;

typedef struct Cell {
  int distance;
  int N, S, W, E;
  int visited;
} Cell;

typedef struct {
  Node *front, *rear;
} Queue;

Node *createNode(int x, int y) {
  Node *newCell = (Node *)malloc(sizeof(Node));
  if (newCell == NULL)
    return NULL;
  newCell->x = x;
  newCell->y = y;
  newCell->next = NULL;
  return newCell;
};

Queue *initQueue() {
  Queue *newQueue = (Queue *)malloc(sizeof(Queue));
  newQueue->front = newQueue->rear = NULL;
  return newQueue;
}

int isEmpty(Queue *q) { return q->front == NULL; }

void enqueue(Queue *q, int x, int y) {
  Node *newCell = createNode(x, y);
  if (!newCell) {
    printf("Queue overflow");
    return;
  }
  if (q->rear == NULL) {
    q->front = q->rear = newCell;
    return;
  }
  q->rear->next = newCell;
  q->rear = newCell;
}

Node *dequeue(Queue *q) {
  if (isEmpty(q)) {
    printf("Queue underflow\n");
    return NULL;
  }
  Node *temp = q->front;
  q->front = q->front->next;
  if (q->front == NULL)
    q->rear = NULL;
  return temp;
}

void traverse(Queue *q) {
  Node *current = q->front;
  while (current != NULL) {
    printf("(%d, %d) ", current->x, current->y);
    current = current->next;
  }
  printf("NULL\n");
}

void printMaze(Cell maze[16][16], int size) {
  for (int x = size - 1; x >= 0; x--) {
    for (int y = 0; y < size; y++)
      printf("%3d", maze[x][y].distance);
    printf("\n");
  }
  printf("\n");
}

Cell decMaze[16][16]; // y, x
int lastPosition[2] = {-1, -1};
int lastHeading = NORTH;
Action finalPath = {};

Action floodFill(Mouse *mouse) {
  int newCell = lastPosition[0] != mouse->x || lastPosition[1] != mouse->y;
  int newHeading = lastHeading != mouse->heading;
  int x = mouse->x;
  int y = mouse->y;
  int hasReach =
      x == mouse->maze->finish_pos[0] && mouse->maze->finish_pos[1] == y;

  if ((newCell || newHeading) && !hasReach) {
    int size = mouse->maze->size;

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

    Queue *q = initQueue();
    for (int y = 0; y < size; ++y) {
      for (int x = 0; x < size; ++x) {
        decMaze[y][x].distance = -1;
        decMaze[y][x].visited = 0;
      }
    }

    enqueue(q, mouse->maze->finish_pos[0], mouse->maze->finish_pos[1]);

    decMaze[mouse->maze->finish_pos[1]][mouse->maze->finish_pos[0]].distance =
        0;
    decMaze[mouse->maze->finish_pos[1]][mouse->maze->finish_pos[0]].visited = 1;
    while (!isEmpty(q)) {
      Node *c = dequeue(q);
      int currentDistance = decMaze[c->y][c->x].distance;

      // Norte
      if (c->y + 1 < size && !decMaze[c->y][c->x].N &&
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
      if (c->x + 1 < mouse->maze->size && !decMaze[c->y][c->x].E &&
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
    printMaze(decMaze, mouse->maze->size);

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

    if (bestAction == IDLE) {
      return LEFT;
    }

    lastHeading = mouse->heading;
    lastPosition[0] = mouse->x;
    lastPosition[1] = mouse->y;

    switch (bestAction) {
    case RIGHT:
      printf("RIGHT\n");
      break;
    case LEFT:
      printf("LEFT\n");
      break;
    case FORWARD:
      printf("FORWARD\n");
      break;
    default:
      printf("IDLE\n");
      break;
    }
    return bestAction;
  }
  return IDLE;
}
