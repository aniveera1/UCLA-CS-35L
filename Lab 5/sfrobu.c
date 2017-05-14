#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

/* Reads frobnicated lines from STDIN, sorts them,
 * and prints back to STDOUT using system calls
 */

bool fSelected = false;

/* Decode if flag is set */
unsigned char decode (char a) {
  a = a ^ 42;
  if (fSelected) {
    a = toupper((unsigned char)a);
  }
  return a;
}

/* Compares two frobnicated lines */
int frobcmp (char const *a, char const *b) {
  int currentA = 0;
  int currentB = 0;
  while (!!(strncmp(&a[currentA], " ", 1)) && !!(strncmp(&b[currentB], " ", 1))) {
    if (decode(a[currentA]) > decode(b[currentB])) {
      return 1;
    }
    if (decode(a[currentA]) < decode(b[currentB])) {
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

/* Container function used for qsort */
int frobContainer (const void *a, const void *b) {
  return frobcmp(*(char**)a, *(char**)b);
}

/* Checks for errors in processing STDIN or printing to STDOUT */
void checkCall(const long *error) {
  if (*error == -1) {
    char message[] = "ERROR: System call failed\n";
    write(STDERR_FILENO, &message, sizeof(message));
    exit(1);
  }
}

int main (int argc, char *argv[]) {
  /* Check for -f flag */
   if (argc == 3 || argc == 2)
    if (!(strncmp(argv[1], "-f", 2)))
      fSelected = true;
 
  /* Load File */
  struct stat fileDes;
  long error = 0;
  error = fstat(STDIN_FILENO, &fileDes);
  checkCall(&error);
    
  char **words;
  char *allWords = NULL;
  long wordsIterator = 0;
    
  /* File has been passed in */
  if (S_ISREG(fileDes.st_mode)) {
    /* Read all content */
    allWords = (char*)malloc(fileDes.st_size);
    error = read(STDIN_FILENO, allWords, fileDes.st_size);
    checkCall(&error);
        
    /* Parse through and count words */
    int count = 0;
    long i;
    for (i = 0; i < fileDes.st_size; i+=1) {
      /* Make last character a space */
      if (i == fileDes.st_size - 1)
	allWords[i] = ' ';

      /* In case word start immediately */
      if (i == 0 && allWords[i] != ' ')
	count++;

      /* Reached end of current word */
      if (allWords[i] == ' ') {
	/* Increment till next word */
	while (allWords[i] == ' ' && i < fileDes.st_size)
	  ++i;
                
	/* Reached new word */
	if (i < fileDes.st_size)
	  ++count;
      }
    }

    /* Make memory for all words */
    words = (char**)malloc(count * sizeof(char*));
        
    /* Record each word */
    bool processedWord = false;
    for (i = 0; i < fileDes.st_size; ++i) {
      /* Add word by storing first character
	 of word in array */
      if (!processedWord && allWords[i] != ' ') {
	words[wordsIterator] = &allWords[i];
	++wordsIterator;
	processedWord = true;
      }
      /* Reached end of current word */
      else if (processedWord && allWords[i] == ' ')
	processedWord = false;
    }
  }
  /* No file, allocate space for one word */
  else
    words = (char**)malloc(sizeof(char*));
    
  /* Continue to add words if file is increasing */
  char* word;
  word = (char*)malloc(1);
    
  char curr;
  ssize_t curErr = read(STDIN_FILENO, &curr, 1);
  checkCall(&curErr);
  char next;
  ssize_t nextErr = read(STDIN_FILENO, &next, 1);
  checkCall(&nextErr);

  long letterCount = 0;
  while (curErr > 0) {
    /* Add current char to word */
    word[letterCount] = curr;
    letterCount++;
    char* temp = (char*)realloc(word, letterCount + 1);
    
    /* Allocation error */
    if (temp == NULL) {
      free(word);
      char message[] = "ERROR: Memory allocation\n";
      write(STDERR_FILENO, &message, sizeof(message));
      exit(1);
    }
    word = temp;
        
    /* Reached end of word */
    if (curr == ' ') {
      /* Add to array */
      words[wordsIterator] = word;
      ++wordsIterator;
            
      char** tempWords = (char**)realloc(words, (wordsIterator+1) * sizeof(char*));
      /* Allocation error */
      if (tempWords == NULL) {
          free(words);
          char message[] = "ERROR: Memory allocation\n";
          write(STDERR_FILENO, &message, sizeof(message));
          exit(1);
      }
      words = tempWords;
            
      /* Prepare for next word */
      word = NULL;
      word = (char*)malloc(1);
      letterCount = 0;
    }
        
    /* Finished reading */
    if (nextErr == 0 && curr == ' ')
      break;
    /* Ignore extra spaces */
    else if (next == ' ' && curr == ' ') {
      while (curr == ' ') {
	curErr = read(STDIN_FILENO, &curr, 1);
	checkCall(&curErr);
      }
      nextErr = read(STDIN_FILENO, &next, 1);
      checkCall(&nextErr);
      continue;
    }
    /* Append a space to end */
    else if (nextErr == 0) {
      curr = ' ';
      continue;
    }
        
    /* Get next char */
    curr = next;
    nextErr = read(STDIN_FILENO, &next, 1);
    checkCall(&nextErr);
  }
    
   /* Sort words */
  qsort(words, wordsIterator, sizeof(char*), frobContainer);
  
  long i, j;
  /* Output words to STDOUT */
  for (i = 0; i < wordsIterator; ++i) {
    for (j = 0;; ++j) {
      /* Reached end of word */
      if (words[i][j] == ' ') {
          error = write(STDOUT_FILENO, &words[i][j], 1);
          checkCall(&error);
          break;
      }
        
      /* Output word char by char */
      error = write(STDOUT_FILENO, &words[i][j], 1);
      checkCall(&error);
    }
  }
    
  /* Free memory and return */
  if (S_ISREG(fileDes.st_mode))
    free(allWords);
  free(words);
  return 0;
}
