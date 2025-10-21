#include "queue.h"
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
