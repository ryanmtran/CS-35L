#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void checkStdin() {
  if (ferror(stdin)) {
    fprintf(stderr, "Error in Standard Input");
    exit(1);
    // Handle file error
  }

}

int main(int argc, char* argv[]) {
  //Error checks
  //We only want two arguments passed in
  if(argc != 3) {
    fprintf(stderr, "Error: Improper Number of Arguments passed in");
    exit(1);
  }
  //If the lengths of our two arguments are not equal
  else if(strlen(argv[1]) != strlen(argv[2])) {
    fprintf(stderr, "Error: Length of From and To strings are not equal");
    exit(1);  
  }
  //Check to see if there are repeats in "from" string
  //Index to permit comparisons
  int i = 0;
  //Iterates through our first string
  while (i < strlen(argv[1])) {
    //Second index to permit comparisons to first index.
    //Initiated to i+1 because we do not want to compare to itself and we do not want to repeat comparisons
    int j = i + 1;
    while (j < strlen(argv[1])) {
      //If there is ever a repeat of characters
      if(argv[1][i] == argv[1][j]) {
	fprintf(stderr, "Error: Repeat in From String Found");
	exit(1);
      }
      j++;
    }
    i++;
  }
  
  
  //After error checks, we will begin reading in our data from standard input, performing our translation accordingly
  
  //Pointer that stores our fromStr and toStr
  char* fromStr = argv[1];
  char* toStr = argv[2];
  //d will be an integer so that it will not run into problems with signed and
  //unsigned characters not correctly identifying EOF in special cases
  int d;
  while((d=getchar()) != EOF) {
    checkStdin();
    char c = d;
    //integer that stores the current index we are on in our fromStr
    int i = 0;
    while(i < strlen(fromStr)) {
      //Indicates that there is a match in input string and from string, so we want to replace it
      if (c == fromStr[i]) {
  	//Make translation and write the corresponding character from "to" string
  	c = toStr[i];
	break;
      }
      i++;
    }
    //Upon escaping the while loop, we either print out the character of our
    //input, or print out the chracter from our toStr if a match was found
    putchar(c);
  }
}
