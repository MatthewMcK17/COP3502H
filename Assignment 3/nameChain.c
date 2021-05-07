// Author: Matthew McKeever
// Date: 02/19/2021
// Class: COP3502H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// My beautiful macros
#define in32(n) scanf("%d", &n)
#define MAX_LEN 20

// Word struct that stores each word and its respctive length
typedef struct word {
  char ar[MAX_LEN + 1];
  int len;
} word;

// Function prototypes
void getStrings(word **, int);
int prettyPhrase(word **, int, int, int *, int *);
void printPrettyPhrase(word **, int *, int);
void freeBuf(word **, int);
void copyAr(int *, int *, int);
int isValid(word *, word *);
int isUsed(int *, int, int);

int main() {
  // Declares variables and obtains # of words from stdin
  int n, *indexes, *good;
  word **buf;
  in32(n);

  // Initliazes all of my variables and gets words from input
  buf = calloc(n, sizeof(word *));
  indexes = calloc(n, sizeof(int));
  good = calloc(n, sizeof(int));
  getStrings(buf, n);

  // Prints # of correct permutations (name chains) and prints a prettyPhrase
  printf("%d\n", prettyPhrase(buf, 0, n, indexes, good));
  printPrettyPhrase(buf, good, n);

  // Frees memory
  free(indexes);
  freeBuf(buf, n);

  return 0;
}

// Copies good indices to print later
void copyAr(int *old, int *new, int n) {
  for (int i = 0; i < n; i++) {
    new[i] = old[i];
  }
}

// Determines if new word is a valid chain word
int isValid(word *name, word *new) {
  if (name->ar[name->len - 1] == new->ar[0]) {
    return 1;
  }
  return 0;
}

// Determines if a word is already in the chain
int isUsed(int *indexes, int n, int new) {
  for (int i = 0; i < n; i++) {
    if (indexes[i] == new) {
      return 1;
    }
  }
  return 0;
}

// Meat and potatoes of my program
// Optimized brute force permutator
int prettyPhrase(word **buf, int cur, int n, int *indexes, int *good) {
  if (cur == n) {
    copyAr(indexes, good, n);
    return 1;
  }
  int ans = 0;
  for (int i = 0; i < n; i++) {
    if (cur == 0) {
      indexes[cur] = i;
      ans += prettyPhrase(buf, cur + 1, n, indexes, good);
    } else if ((isUsed(indexes, cur, i) ^ 1) && isValid(buf[indexes[cur - 1]], buf[i])) {
      indexes[cur] = i;
      ans += prettyPhrase(buf, cur + 1, n, indexes, good);
    }
  }
  return ans;
}

// Gets each word from input
void getStrings(word **buf, int n) {
  for (int i = 0; i < n; i++) {
    buf[i] = calloc(MAX_LEN, sizeof(word));
    scanf("%s", buf[i]->ar);
    buf[i]->len = strlen(buf[i]->ar);
  }
}

// Prints a valid pretty phrase
void printPrettyPhrase(word **buf, int *indexes, int n) {
  int i = 0;
  if (n == 1) {
    printf("%s\n", buf[0]->ar);
    return;
  } else if (indexes[0] == indexes[1]) {
    return;
  }
  for (i; i < (n - 1); i++) {
    printf("%s ", buf[indexes[i]]->ar);
  }
  printf("%s\n", buf[indexes[i]]->ar);
}

// Frees word structs
void freeBuf(word **buf, int n) {
  for (int i = 0; i < n; i++) {
    free(buf[i]);
  }
  free(buf);
}
