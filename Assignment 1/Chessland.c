// Author: Matthew McKeever
// Course: COP 3502H
// Date: January 25, 2021

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Macro for default capacity
#define DEFAULT_CAP 10

// Typedef declarations for my structs
typedef struct Piece Piece;
typedef struct List List;
typedef struct Board Board;

// My beautiful structs that run this program
struct Piece {
  int rank, file, id;
};

struct List {
  int identifier;
  Piece **arr;
  int size, cap;
};

struct Board {
  List **arr;
  int size, cap;
};

// Function prototypes
Board *createBoard();
List *createList(int);
Piece *createPiece(int, int, int);
void destroyBoard(Board *);
void destroyList(List *);
void destroyPieces(Piece **, int);
void addPieceToList(List *, Piece *);
void addListToBoard(Board *, List *);
void expandList(List *);
void expandBoard(Board *);
List *findList(Board *, int);
void findTargetsInList(List *, int, int *, int *, int);
void printOpponents(int *, int);

int main() {
  // Create rank, file, counter, and sum variables
  int n, rank, file, counter = 0;
  int forwardSum, backwardSum;
  scanf("%d", &n);

  // Create boards and various arrays
  Board *rankBoard = createBoard();
  Board *fileBoard = createBoard();
  Board *forwardDiagBoard = createBoard();
  Board *backwardDiagBoard = createBoard();
  Piece **array = malloc(n * sizeof(Piece *));
  int *storage = calloc(8, sizeof(int));

  // Obtain inforamtion for each piece
  for (int i = 0; i < n; i++) {
    // Scans stdin for rank and file and creates new piece
    scanf("%d %d", &rank, &file);
    Piece *new = createPiece(rank, file, i + 1);
    array[i] = new;

    // forwardSum = id for forwardDiag, backwardSum = id for backwardDiag
    forwardSum = rank + file;
    backwardSum = rank - file;

    // Adds piece to each board
    addPieceToList(findList(rankBoard, new->rank), new);
    addPieceToList(findList(fileBoard, new->file), new);
    addPieceToList(findList(forwardDiagBoard, forwardSum), new);
    addPieceToList(findList(backwardDiagBoard, backwardSum), new);
  }

  printf("\n");

  // Finds targets for each piece
  for (int i = 0; i < n; i++) {
    counter = 0;

    forwardSum = array[i]->rank + array[i]->file;
    backwardSum = array[i]->rank - array[i]->file;

    // Finds targets for each piece in each board
    findTargetsInList(findList(rankBoard, array[i]->rank), array[i]->file, storage, &counter, 1);
    findTargetsInList(findList(fileBoard, array[i]->file), array[i]->rank, storage, &counter, 2);
    findTargetsInList(findList(forwardDiagBoard, forwardSum), backwardSum, storage, &counter, 3);
    findTargetsInList(findList(backwardDiagBoard, backwardSum), forwardSum, storage, &counter, 4);

    // Prints oppponents
    printOpponents(storage, counter);
  }
  // Frees everything
  free(storage);
  destroyPieces(array, n);
  destroyBoard(rankBoard);
  destroyBoard(fileBoard);
  destroyBoard(forwardDiagBoard);
  destroyBoard(backwardDiagBoard);

  return 0;
}

// Creates board pointer
Board *createBoard() {
  Board *tmp = malloc(sizeof(Board));

  // Assigns pertinent info to board and creates arrayList
  tmp->size = 0;
  tmp->cap = DEFAULT_CAP;
  tmp->arr = calloc(DEFAULT_CAP, sizeof(List *));

  return tmp;
}

// Creates piece and returns pointer
Piece *createPiece(int rank, int file, int id) {
  Piece *tmp = malloc(sizeof(Piece));

  // Assigns pertinent info to piece
  tmp->rank = rank;
  tmp->file = file;
  tmp->id = id;

  return tmp;
}

// Finds list with given identifier and returns it
List *findList(Board *board, int id) {
  List *tmp;

  // Searches entire board for a specific list
  for (int i = 0; i < board->size && board->arr[i] != NULL; i++) {
    if (board->arr[i]->identifier == id) {
      return board->arr[i];
    }
  }
  // Creates new list if not found and adds it to the board
  tmp = createList(id);
  addListToBoard(board, tmp);

  return tmp;
}

// Creates list with given identifier
List *createList(int id) {
  List *tmp = malloc(sizeof(List));

  // Assigns pertinent info to list and creates pieceList
  tmp->identifier = id;
  tmp->size = 0;
  tmp->cap = DEFAULT_CAP;
  tmp->arr = calloc(DEFAULT_CAP, sizeof(Piece *));

  return tmp;
}

// Adds given list to board
void addListToBoard(Board *board, List *list) {
  // Checks if board arrayList is full
  if (board->size == board->cap) {
    expandBoard(board);
  }
  board->arr[board->size] = list;
  board->size++;
}

// Expands board arrayList by a factor of 2 and increases capacity
void expandBoard(Board *board) {
  int newCap = 2 * board->cap;
  board->arr = realloc(board->arr, newCap * sizeof(List *));
  board->cap = newCap;
}

// Adds piece to appropriate list
void addPieceToList(List *list, Piece *new) {
  // Checks if array in list is full
  if (list->size == list->cap) {
    expandList(list);
  }
  list->arr[list->size] = new;
  list->size++;
}

// Expands list and assigns a new capacity
void expandList(List *list) {
  int newCap = 2 * list->cap;
  list->arr = realloc(list->arr, newCap * sizeof(Piece *));
  list->cap = newCap;
}

//
void findTargetsInList(List *list, int sum, int *storage, int *count, int type) {
  int behindPiece = 0, frontPiece = 0, minBehindDist = INT_MIN, minForDist = INT_MAX;
  int difference = 0;

  // Implements linear search to find attackable neighbors
  for (int i = 0; i < list->size; i++) {
    if (type == 1) { // For rank
      difference = list->arr[i]->file - sum;
    }
    else if (type == 2) { // For file
      difference = list->arr[i]->rank - sum;
    }
    else if (type == 3) { // For forwards diagonal
      difference = (list->arr[i]->rank - list->arr[i]->file) - sum;
    }
    else if (type == 4) { // For backwards diagonal
      difference = (list->arr[i]->rank + list->arr[i]->file) - sum;
    }

    // Determines if there's a closer piece forwards or backwards
    if (difference > 0 && difference < minForDist) {
      minForDist = difference;
      frontPiece = list->arr[i]->id;
    }
    else if (difference < 0 && difference > minBehindDist) {
      minBehindDist = difference;
      behindPiece = list->arr[i]->id;
    }
  }
  // Checks if either a close forward or backwards piece was found
  // and adds to an integer array to be used later in printOpponents
  if (frontPiece != 0) {
    storage[*count] = frontPiece;
    (*count)++;
  }
  if (behindPiece != 0) {
    storage[*count] = behindPiece;
    (*count)++;
  }
}

// Prints all oppononents for ith piece
void printOpponents(int *storage, int count) {
  int i;

  if (count == 0) {
    printf("0\n");
    return;
  }

  printf("%d ", count);
  for (i = 0; i < (count - 1); i++) {
    printf("%d ", storage[i]);
  }
  printf("%d\n", storage[i]);
}

// Free entire board and subsequent lists inside
void destroyBoard(Board *board) {
  for (int i = 0; i < board->size; i++) {
    destroyList(board->arr[i]);
  }
  free(board->arr);
  free(board);
}

// Frees list's array and list
void destroyList(List *list) {
  free(list->arr);
  free(list);
}

// Free each piece pointer
void destroyPieces(Piece **arr, int n) {
  for (int i = 0; i < n; i++) {
    free(arr[i]);
  }
  free(arr);
}
