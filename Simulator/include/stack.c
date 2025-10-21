#include "stack.h"
void printStack(NodeStack **stack) {
  NodeStack *temp = *stack;

  while (temp != NULL) {
    char *printStatement;
    switch (temp->action) {
    case RIGHT:
      printStatement = "RIGHT";
      break;
    case LEFT:
      printStatement = "LEFT";
      break;
    case FORWARD:
      printStatement = "FORWARD";
      break;
    default:
      printStatement = "UNKNOWN";
      break;
    }
    printf("%s-> ", printStatement);
    temp = temp->next;
  }
  printf("\n");
}

NodeStack *createNodeStack(int data) {
  NodeStack *newNode = (NodeStack *)malloc(sizeof(NodeStack));
  if (newNode == NULL)
    return NULL;
  newNode->action = data;
  newNode->next = NULL;
  return newNode;
};

int insertBeforeHead(NodeStack **head, int data) {
  NodeStack *newNode = createNodeStack(data);
  if (!newNode)
    return -1;

  if (*head == NULL) {
    *head = newNode;
    return 0;
  }

  newNode->next = *head;
  *head = newNode;
  return 0;
}

int deleteHead(NodeStack **head) {
  NodeStack *temp = *head;
  *head = (*head)->next;
  free(temp);
  return 0;
}

int StackEmpty(NodeStack **stack) { return *stack == NULL; }

void push(NodeStack **stack, int data) {
  if (insertBeforeHead(stack, data)) {
    printf("Stack Overflow!\n");
  }
}

// Function to pop an element from  the stack
void pop(NodeStack **stack) {
  // checking underflow condition
  if (StackEmpty(stack)) {
    printf("Stack Underflow\n");
  }
  // deleting the head.
  deleteHead(stack);
};

int peek(NodeStack **stack) {
  // check for empty stack
  if (!StackEmpty(stack))
    return (*stack)->action;
  else
    return -1;
}
