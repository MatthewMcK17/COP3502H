// Author: Matthew McKeever
// Course: COP 3502H
// Date: April 5, 2021

#include <stdio.h>
#include <stdlib.h>

#define input(n) scanf("%d", &n)

// Aliasing data types
typedef struct Node Node;
typedef struct BST BST;

// Defining Node structure
struct Node {
  int id, winner;
  Node *left, *right;
};

// Defining binary search tree structure
typedef struct BST {
  Node *root;
} BST;

// Functional prototypes for BST
BST *createBST(int);
Node *insert(Node *, Node *);

// Functions that creates Nodes and players
Node *createNode(int);
int *createPlayers(int);

// Calculates excitement for tournament
void simulate(Node *root);

// Global variables for easy-access
unsigned long long excitement = 0;
int *players;

int main() {
  int n;
  input(n);

  // Meat and Potatoes (creates and solves tree)
  BST *bst = createBST(n - 1);
  players = createPlayers(n);
  simulate(bst->root);
  free(bst->root);
  free(bst);
  free(players);

  // Prints excitement
  printf("%llu\n", excitement);

  return 0;
}

// Implements post-order traversal to calculate total excitement
void simulate(Node *root) {
  if (root == NULL) {
    return;
  }

  // Recursive calls;
  simulate(root->left);
  simulate(root->right);

  // Assigns player1 & player2
  int player1 = (root->left == NULL) ? players[root->id - 1] : root->left->winner;
  int player2 = (root->right == NULL) ? players[root->id] : root->right->winner;

  // Caculates winner and match excitement
  root->winner = (player1 > player2) ? player1 : player2;
  excitement += ((player1 - player2) < 0) ? player2 - player1 : player1 - player2;

  free(root->left);
  free(root->right);
}

// Creates a list of players with their skill at index i
int *createPlayers(int n) {
  int val;
  int *tmp = malloc(sizeof(int) * n);
  for (int i = 0; i < n; i++) {
    input(val);
    tmp[i] = val;
  }
  return tmp;
}

// Creates tree with tables
BST *createBST(int n) {
  int val;
  BST *tmp = malloc(sizeof(BST));
  int *vals = malloc(sizeof(int) * n);
  tmp->root = NULL;

  // Obtains table #s and the inserts them in post-order
  for (int i = 0; i < n; i++) {
    input(val);
    vals[i] = val;
  }
  for (int i = n - 1; i >= 0; i--) {
    tmp->root = insert(tmp->root, createNode(vals[i]));
  }
  free(vals);
  return tmp;
}

// Inserts tables (Nodes) into trees
Node *insert(Node *root, Node *node) {
  if (root == NULL) {
    root = node;
    return root;
  }

  // Inserts table (Node) based on table id
  if (node->id < root->id) {
    root->left = insert(root->left, node);
  } else if (node->id > root->id) {
    root->right = insert(root->right, node);
  }
  return root;
}

// Creates table (Node) struct
Node *createNode(int val) {
  Node *tmp = malloc(sizeof(Node));
  tmp->id = val;
  tmp->winner = 0;
  tmp->right = tmp->left = NULL;

  return tmp;
}
