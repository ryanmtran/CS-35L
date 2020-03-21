#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


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
  //Buffer to hold current character
  char buf[1];
  //Pointer that stores our fromStr and toStr
  char* fromStr = argv[1];
  char* toStr = argv[2];
  //Used to store the return value of read to check for errors
  //While read is not an error or is not the end of the file
  while(read(0, buf, 1) > 0) {
    int i = 0;
    while(i < strlen(fromStr)) {
      //Indicates that there is a match in input character and from string, so we want to replace it
      if (buf[0] == fromStr[i]) {
	//Make translation and write the corresponding character from "to" string
	buf[0] = toStr[i];
	break;
      }
      i++;
    }
    //Upon escaping the while loop, we either print out the character of our
    //input, or print out the chracter from our toStr if a match was found
    write(1, buf, 1);
  }
}

