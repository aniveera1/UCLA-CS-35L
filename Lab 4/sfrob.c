#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Reads frobnicated lines from STDIN, sorts them, and
 prints back to STDOUT */

/* Compares two frobnicated lines */
int frobcmp (char const *a, char const *b) {
  int currentA = 0;
  int currentB = 0;
  while (!!(strncmp(&a[currentA], " ", 1)) && !!(strncmp(&b[currentB], " ", 1))) {
    if ((a[currentA] ^ 42) > (b[currentB] ^ 42)) {
      return 1;
    }
    if ((a[currentA] ^ 42) < (b[currentB] ^ 42)) {
      return -1;
    }
    currentA++;
    currentB++;
  }
  if (!!(strncmp(&a[currentA], " ", 1)))
    return 1;
  if (!!(strncmp(&b[currentB], " ", 1)))
    return -1;
  return 0;
}

/* Checks for errors in processing STDIN or printing to STDOUT */
void checkInput() {
  if (ferror(stdin) != 0) {
    fprintf(stderr, "ERROR: Unable to process STDIN\n");
    exit(1);
  }
}

void checkOutput() {
  if (ferror(stdin) != 0) {
    fprintf(stderr, "ERROR: Unable to print to STDOUT\n");
    exit(1);
  }
}

/* Container function used for qsort */
int frobContainer (const void *a, const void *b) {
  return frobcmp(*(char**)a, *(char**)b);
}

int main(void) {
  char* currentWord;
  char** words;
    
  
  currentWord = (char*)malloc(sizeof(char));
  words = (char**)malloc(sizeof(char*));
  char current = getchar();
  checkInput();
  char next = getchar();
  checkInput();
     
  int letterCount = 0;
  int wordCount = 0;
  while (current != EOF) {
    currentWord[letterCount] = current;

    char* temp = (char*)realloc(currentWord, (letterCount+2)*sizeof(char));
    if (temp == NULL) {
      free(currentWord);
      fprintf(stderr, "ERROR: Memory allocation");
      exit(1);
    }
    letterCount++;
    currentWord = temp;
        

    if (current == ' ') {
      words[wordCount] = currentWord;
            
      char** tempWords = (char**)realloc(words, (wordCount+2)*sizeof(char*));
      if (tempWords == NULL) {
	free(words);
	fprintf(stderr, "ERROR: Memory allocation");
	exit(1);
      }
      wordCount++;
      words = tempWords;
      currentWord = NULL;
      currentWord = (char*)malloc(sizeof(char));
      letterCount = 0;
    }

    if (current == ' ' && next == EOF) {
      break;
    }
    else if (current == ' ' && next == ' ') {
      while (current == ' ') {
	current = getchar();
	checkInput();
      }
      next = getchar();
      checkInput();
      continue;
    }
    else if (next == EOF) {
      current = ' ';
      continue;
    }
    current = next;
    next = getchar();
    checkInput();
  }

  qsort(words, wordCount, sizeof(char*), frobContainer);
    
  size_t i;
  size_t j;
  for (i = 0; i < wordCount; i++) {
    for (j = 0;; j++) {
      putchar(words[i][j]);
      checkOutput();
      if (words[i][j] == ' ')
	break;
    }
  }
    
  long a;
  for (a = 0; a < wordCount; a++) {
    free(words[a]);
  }
  free(words);
    
  return 0;
}

