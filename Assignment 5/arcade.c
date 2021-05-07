// Author: Matthew McKeever
// Course: COP 3502H
// Date: March 22, 2021

#include <stdio.h>
#include <stdlib.h>

// #define macros
#define MAX_LEN 20
#define numInput(x) scanf("%d", &x)
#define PersonInput(x, y) scanf("%d%s", &x, y)

// Renaming structs
typedef struct Stack Stack;
typedef struct Node Node;
typedef struct Queue Queue;
typedef struct Person Person;

// Struct definitions
struct Queue {
  Stack *front;
  Stack *back;
};

struct Stack {
  Node *head;
  int size;
};

struct Node {
  Person *current;
  Person *max;
  Node *next;
  int id;
};

struct Person {
  int tokens;
  char name[MAX_LEN + 1];
};

// Queue Function Prototypes
Queue *createQueue();
void enqueue(Queue *);
void dequeue(Queue *);
void top(Queue *);
void flip(Queue *);
void destroyQueue(Queue *);

// Stack Function Prototypes
void push(Stack *, Node *);
void pop(Stack *);
void destroyStack(Stack *);


//Node Function Prototypes
Node *createNode();
Person *createPerson();
void destroyNode(Node *);

int main() {
  int n;

  // Create queue
  Queue *queue = createQueue();

  // Obtains input and acts accordingly until 0 is inputted
  do {
    numInput(n);
    if (n == 1) {
      enqueue(queue); // Adds Node to queue
    } else if (n == 2) {
      dequeue(queue); // Removes Node from queue
    } else if (n == 3) {
      top(queue); // Prints current MVP
    }
  } while (n != 0);

  // Frees remaining structures
  destroyQueue(queue);

  return 0;
}

// Destroys queue
void destroyQueue(Queue *que) {
  if (que->front->head == NULL) {
    free(que->front);
  } else {
    destroyStack(que->front);
  }
  if (que->back->head == NULL) {
    free(que->back);
  } else {
    destroyStack(que->back);
  }
  free(que);
}

// Destroys stack
void destroyStack(Stack *stack) {
  Node *tmp;
  do {
    tmp = stack->head->next;
    destroyNode(stack->head);
    stack->head = tmp;
  } while (tmp != NULL);
  free(stack);
}

// Destroys node
void destroyNode(Node *node) {
  free(node->current);
  free(node);
}

// Creates queue
Queue *createQueue() {
  Queue *tmp = malloc(sizeof(Queue));
  tmp->front = calloc(1, sizeof(Stack));
  tmp->back = calloc(1, sizeof(Stack));

  if (tmp->front == NULL || tmp->back == NULL)
    exit(3);

  tmp->front->size = tmp->back->size = 0;

  return tmp;
}

// Adds a Node to the back stack
void enqueue(Queue *que) {
  Node *tmp = createNode();
  push(que->back, tmp);
}

// Removes top node from front stack
void dequeue(Queue *que) {
  if (que->back->head == NULL && que->front->head == NULL) {
    return;
  }
  if (que->front->head == NULL && que->back->head != NULL) {
    flip(que);
  }
  Node *tmp = que->front->head;
  que->front->head = tmp->next;
  destroyNode(tmp);
}

// Obtains and prints MVP
void top(Queue *que) {
  if (que->front->head == NULL && que->back->head == NULL) {
    return;
  }
  if (que->front->head == NULL && que->back->head != NULL) {
    printf("%s\n", que->back->head->max->name);
  } else if (que->back->head == NULL && que->front->head != NULL) {
    printf("%s\n", que->front->head->max->name);
  } else {
    printf("%s\n", ((que->front->head->max->tokens > que->back->head->max->tokens) ?
                        que->front->head->max->name : que->back->head->max->name));
  }
}

// Stacks do a "barrel roll" / moves Nodes from back to front
void flip(Queue *que) {
  Node *tmp = que->back->head;
  do {
    que->back->head = tmp->next;
    push(que->front, tmp);
    tmp = que->back->head;
  } while (tmp != NULL);
}

// Appends Node to stack
void push(Stack *stack, Node *tmp) {
  if (tmp->id == 0)
    tmp->id = (stack->size)++;

  // Lengthens linked list in the stack struct
  tmp->next = stack->head;
  stack->head = tmp;

  if (tmp->next == NULL) {
    tmp->max = tmp->current;
    return;
  }

  // Assigns current MVP
  if (tmp->current->tokens > tmp->next->max->tokens) {
    tmp->max = tmp->current;
  } else if (tmp->current->tokens < tmp->next->max->tokens) {
    tmp->max = tmp->next->max;
  } else if (tmp->id > tmp->next->id) {
    tmp->max = tmp->current;
  } else {
    tmp->max = tmp->next->max;
  }
}

// Creates Node
Node *createNode() {
  Node *tmp = malloc(sizeof(Node));
  tmp->max = NULL;
  tmp->next = NULL;
  tmp->id = 0;
  tmp->current = createPerson();

  if (tmp == NULL)
    exit(2);

  return tmp;
}

// Creates Person
Person *createPerson() {
  Person *tmp = malloc(sizeof(Person));
  PersonInput(tmp->tokens, tmp->name);

  if (tmp == NULL)
    exit(1);

  return tmp;
}
