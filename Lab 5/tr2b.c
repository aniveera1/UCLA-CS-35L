/* Transliterates every byte from STDIN if it matches
 * a character in the first string to the respective
 * character in the second string
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

int main (int argc, char *argv[]) {
  /* Check for proper number of operands */
  if (argc != 3) {
    printf("ERROR: Wrong number of operands\n");
    exit(1);
  }
    
  char* fromString = argv[1];
  char* toString = argv[2];
    
  /* Check fromString for repeats */
  long current, next;
  for (current = 0; fromString[current] != '\0'; ++current)
    for (next = current + 1; fromString[next] != '\0'; ++next)
      if (fromString[current] == fromString[next]) {
	printf("ERROR: Repeating charactres in from operand\n");
	exit(1);
      }
    
  /* Compare lengths */
  long toLength;
  for (toLength = 0; toString[toLength] != '\0'; ++toLength);
  if (current != toLength) {
    printf("ERROR: Operands are not of same length\n");
    exit(1);
  }
    
  /* Process through all of STDIN */
  char currentChar = getchar();
  checkInput();
  while (currentChar != EOF) {
    /* If match, transliterate byte */
    bool noMatch = true;
    for (current = 0; fromString[current] != '\0'; ++current)
      if (fromString[current] == currentChar) {
	putc(toString[current], stdout);
	checkOutput();
	noMatch = false;
      }
        
    /* Otherwise simply print */
    if (noMatch) {
      putc(currentChar, stdout);
      checkOutput();
    }
        
    /* Get next char */
    currentChar = getchar();
    checkInput();
  }
}
