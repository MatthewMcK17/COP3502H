// Author: Matthew McKeever
// Course: COP 3502H
// Date: March 8, 2021

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros to improve program's versatility
#define NAME_SIZE 20
#define RUN 32

// Person struct that holds name, tokens, bills, and overall units
typedef struct Person {
  unsigned long long tokens, bills, units;
  char name[NAME_SIZE + 1];
} Person;

// Function prototypes
Person **createGroup(int);
void calculateUnits(Person **, int);
void printList(Person **, int);
void timSort(Person **, int);

int main() {
  int n;
  scanf("%d", &n);

  //Creates, sorts, , prints, and frees Person array
  Person **group = createGroup(n);
  calculateUnits(group, n);
  timSort(group, n);
  printList(group, n);

  return 0;
}

// A slightly modified version of Professor Meade's timSort algorithm
void timSort(Person **ar, int size) {
  // Base Case
  if (size <= RUN) {
    for (int i = 1; i < size; i++) {
      Person *tmp = ar[i];
      int j = i - 1;
      while (j >= 0 && ar[j]->units < tmp->units) {
        ar[j + 1] = ar[j];
        j--;
      }
      ar[j + 1] = tmp;
    }
    return;
  }
  
  // Implementation of merge sort
  int mid = size >> 1;
  timSort(ar, mid);
  timSort(ar + mid, size - mid);
  int front = 0, back = mid, i = 0;
  Person *tmp[size];
  for ( ; i < size; i++) {
    if (front == mid || (back < size && ar[back]->units > ar[front]->units)) {
      tmp[i] = ar[back++];
    } else {
      tmp[i] = ar[front++];
    }
  }
  memcpy(ar, tmp, sizeof(tmp));
}

// Prints sorted Person list, while simultaneously freeing
void printList(Person **group, int n) {
  for (int i = 0; i < n; i++) {
    printf("%s\n", group[i]->name);
    free(group[i]);
  }
  free(group);
}

// Creates Person struct array
Person **createGroup(int n) {
  Person **ret = calloc(n, sizeof(Person *));

  // Creates each person
  for (int i = 0; i < n; i++) {
    Person *tmp = malloc(sizeof(Person));
    scanf("%s%llu%llu", tmp->name, &tmp->tokens, &tmp->bills);
    ret[i] = tmp;
  }
  return ret;
}

// Calculates total units each Person has (per given multiplier)
void calculateUnits(Person **arr, int n) {
  int tokensMult, billsMult;
  scanf("%d%d", &billsMult, &tokensMult);

  // Assigns units for each person
  for (int i = 0; i < n; i++) {
    arr[i]->units = (arr[i]->tokens * tokensMult) + (arr[i]->bills * billsMult);
  }
}
