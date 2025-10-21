#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../mouse.h"
typedef struct NodeStack {
  struct NodeStack *next;
  Action action;
} NodeStack;

NodeStack *createNodeStack(int data);
int insertBeforeHead(NodeStack **head, int data);

int deleteHead(NodeStack **head);

int StackEmpty(NodeStack **stack);

void push(NodeStack **stack, int data);

void pop(NodeStack **stack);

int peek(NodeStack **stack);

void printStack(NodeStack **stack);
