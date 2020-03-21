#include <stdio.h>
#include <stdlib.h> 
//Given a character that is frobinicated, unfrobincated it
char unfrob (char const ch) {
  return ch^42;
}

//Function that frees all of our data created at the very end of our program
void freeData(char** lines, int linesVal) {
  //Keeps track of the count Index we are on
  int countIndex = 0;
  //Goes through all of our elements and frees them;
  while(countIndex <= linesVal) {
    free(lines[countIndex]);
    countIndex++;
  }
  free(lines);
}

//Check stdin to see if there is an error, printing an error message, freeing memory and exiting accordingly
void checkStdin(char** arr, int lineNum) {
  if (ferror(stdin)) {
    fprintf(stderr, "Error in Standard Input");
    freeData(arr, lineNum);
    exit(1);
    // Handle file error
  }
}

//Check a pointer to see if after malloc or realloc, there was an error grabbing memory.
//Free the pointer accordingly
//Then proceed to free all the memory I have used
checkPointer(void *ptr) {
  if (ptr == NULL) {
    free(ptr);
    fprintf(stderr, "Error in memory allocation");
    exit(1);
   }
}

//Taking in two character arrays, iterate through the array
//and return 1 if the unfrobicanated array is larger, -1 if less, and 0 if same
int frobcmp (char const *a, char const *b) {
  while (1) {
    while (*a == '\0'){
	a++;
      }
    while (*b == '\0') {
	b++;
      }
    //Cases where iterating through leads to an end of our string
    //The words are the same
    if (*a == ' ' && *b == ' ') {
	return 0;
      }
    //String a is a prefix of string b
    else if (*a == ' ') {
	return -1;
      }
    //String b is a prefix of string a
    else if (*b == ' ') {
	return 1;
      }
    else if (unfrob(*a) < unfrob(*b)) {
        return -1;
      }
    else if (unfrob(*a) > unfrob(*b)) {
        return 1;
      }
    //iterate to next characters if no case is found
    else {
    a++;
    b++;
    }
  }
}

//Our qsort gives pointers to our pointers for the comparison, weridly enough. We need to
//Dereference it and pass it into our frbcmp function accordinly
int frobcmpCast(const void *a1, const void *b1)
{
  //We are given a void pointer to our actual a and b, so we must cast it and then dereference it
  //Taking in our pointers to characters, we can now do our comparisons
  return frobcmp(*(const char**)a1, *(const char**)b1);
}

//Given an array of pointers to char and lineVal, print out all the characters in our array
void printArrays (char** arr, int lineVal) {
  int i = 0;
  while (i <= lineVal) {
    int index = 0;
    while (arr[i][index] != ' ')
      {
        printf("%c", arr[i][index]);
        index++;
      }
    //Print the space because we stopped at in in the while loop
    printf("%c", ' ');
    i++;
  }
}

int main() {
  //initializes pointer to store pointer to elements
  char** lines = (char**)malloc(sizeof(char*));
  checkPointer(lines);

  //Used to store the array of characters
  char* element = (char*)malloc(sizeof(char));
  checkPointer(element);
  //The very first element in lines will hold the pointer to our very first element. This is
  //the beginning of our "2-D" array
  *lines = element;

  //Trackers to keep track of the length of our current line and the number of lines we have
  int linesIndex = 0;
  int elementIndex = 0;
  
  //Our character we are grabbing from stdin
  int d;
  while ((d = getchar()) != EOF) {
    // First check stdin for errors every iteration
    checkStdin(lines, linesIndex);
    char c = d;
    //If a space is read in, we know we need to create a new element, and a character in that element accordingly
   if (c == ' ') {
     //Fill our element with the character read in. The space denotes the end of an element
     lines[linesIndex][elementIndex] = ' ';
     //Rellocate lines to add a new element
     char** tempLines = (char**) realloc(lines, (linesIndex+2) * sizeof(char*));
     
     //checkPointer(tempLines);
     //If there was an error in our reallocation, free all of our data
     if(tempLines == NULL) {
       freeData(lines, linesIndex);
       fprintf(stderr, "Error in Memory Allocation");
       exit(1);
     }

     lines =  tempLines;
     //LinesIndex increases by 1 with our new element
     linesIndex++;
     char* tempElement = (char*)malloc(sizeof(char));
   
     //checkPointer(tempElement);
     //If there was an error in memory allocation
     if(tempElement == NULL) {
       //We have one less element to clean considering that we just incremented our linesindex
       linesIndex--;
       freeData(lines, linesIndex);
       fprintf(stderr, "Error in Memory Allocation");
       exit(1);
     }

     element = tempElement;
     lines[linesIndex] = element;
     elementIndex=0;
     //Creates empty new line with empty element character space by the end of iteration
      }
   //Given nonspace characters
   else { 
     lines[linesIndex][elementIndex] = c;
     //We need to expand element to allow for a new character in the next iteration
     char* tempElement = (char*)realloc(element, (elementIndex+2) * sizeof(char));
    

     //checkPointer(tempElement);
     //If our memory allocation did not work
     if(tempElement == NULL) {
       freeData(lines, linesIndex);
       fprintf(stderr, "Error in Memory Allocation");
       exit(1);
     }


     element=tempElement;
     lines[linesIndex]=element;
     elementIndex++;
     //Creates empty character spaces by the end of iteration
   }

  }
   if (feof(stdin)) {
    //Puts a space in the last character of the last element
     if (elementIndex != 0) {
    lines[linesIndex][elementIndex] = ' ';
     }
     else {
       free(lines[linesIndex]);
       linesIndex--;
     }
    // Handles end of file
     } 
   else if (ferror(stdin)) {
    fprintf(stderr, "Error in Standard Input");
    freeData(lines, linesIndex);
    exit(1);
    // Handle file error
  }

  /* else { */
  /*   printf("Error: Received character that resembles EOF"); */
  /*   exit(1); */
  /*   /\* Received a character that resembles EOF, */
  /*      which means the int has the same width as char on this machine platform */
  /*      Handle error *\/ */
  /* } */

  //The length of our lines is equal to the index that we last left off on + 1
  int linesLength= linesIndex+1;
  //Sort our two dimesnional arrays, passing in our frobcmpCast function to give
  //The rules of this sorting
  qsort(lines, linesLength, sizeof(char*), frobcmpCast);
  printArrays(lines, linesIndex);
  freeData(lines, linesIndex);
}

