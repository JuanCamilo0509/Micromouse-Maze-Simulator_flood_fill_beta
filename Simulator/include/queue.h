#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
typedef struct Node {
  int x, y, distance;
  struct Node *next;
} Node;

typedef struct {
  Node *front, *rear;
} Queue;

Node *createNode(int x, int y);
Queue *initQueue();
int isEmpty(Queue *q);
void enqueue(Queue *q, int x, int y);
Node *dequeue(Queue *q);
void traverse(Queue *q);
