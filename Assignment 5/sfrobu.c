#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Prints Error when fstat fails
void fstatErr() {
  fprintf(stderr, "Error: Error in fstat system call");
  exit(1);
}

//Function that frees all of our data created at the very end of our program
void freeData(char** lines, int linesLength) {
  //Keeps track of the count Index we are on
  int countIndex = 0;
  //Goes through all of our elements and frees them;
  while(countIndex < linesLength) {
    free(lines[countIndex]);
    countIndex++;
  }
  free(lines);
}


//Check read to see if there is an error, printing an error message, freeing memory and exiting accordingly
void checkReadError(char** arr, int lineNum, int retRead) {
  if (retRead < 1) {
    fprintf(stderr, "Error in Standard Input");
    freeData(arr, lineNum);
    exit(1);
  }
}


//Prints out our 2d array lines
void printArrays (char** arr, int linesVal) {
  int i = 0;
  //Buffer that holds our character with a single quote
  char ch[1];
  ch[0] = ' ';
  while (i < linesVal) {
    int index = 0;
    while (arr[i][index] != ' ')
      {
	if(write(1, &arr[i][index], sizeof(char)) < 0)
	  {
	    fprintf(stderr, "Error in Write");
	    exit(1);
	  }
	      
        //printf("%c", arr[i][index]);
        index++;
      }
    //Print the space because we stopped at in in the while loop
    if(write(1, &ch, sizeof(char)) < 0) {
      fprintf(stderr, "Error in Write");
      exit(1);
    }
    //printf("%c", ' ');
    i++;
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

//Given a character that is frobinicated, unfrobincated it
char unfrob (char const ch) {
  return ch^42;
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


int frobcmpUp (char const *a, char const *b) {
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
    else if (toupper(unfrob(*a)) < toupper(unfrob(*b))) {
      return -1;
    }
    else if (toupper(unfrob(*a)) > toupper(unfrob(*b))) {
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

//Upper case version of frobcmpCast that is done when -f flag is activated
int frobcmpCastUp(const void *a1, const void *b1)
{
  return frobcmpUp(*(const char**)a1, *(const char**)b1);
}


int main(int argc, char* argv[]) {
  //Two dimensional array that holds pointers to our elements
  char** lines;
  //Array that holds our element
  char* element;
  //fFlag, which is 0 if there is no f flag, and 1 if there is
  int fFlag = 0;
  
  //Only one argument can be passed, and that would be the -f flag
  if (argc > 2) {
    fprintf(stderr, "Error: Too many arguments given");
    exit(1);
  }
  //If we are given one argument
  else if (argc == 2) {
    char* flag = argv[1];
    //What our flag should be if the user inputs a -f. This is the only flag allowed
    char* refFlag = "-f";
    //If the -f flag is identtified as an input
    if(strcmp(flag, refFlag) == 0) {
      fFlag = 1;
    }
    //If given one argument, and it is not a match with -f, then an error occured
    else {
      fprintf(stderr, "Error: Flag given is Invalid");
      exit(1);
    }
  }
  //Holds info about the infromation we read in from qstat
  struct stat info;
  //if our fstat command returns an error
  if(fstat(0, &info) != 0) {
    fprintf(stderr, "Error: fstat read info wrong");
    exit(1);
  }
  //Stores the length of our file array input
  int fileLength = info.st_size;
  //Array that will be utilized to store the contents of our regular file
  char* fileArray;
  //Coubter to keep track of the number of elements we have
  int numElements = 0;
  //Counter to keep track of the maximum size of our elemnts in fileArray
  int maxSize = 0;
  //Counter to keep track of current size of each element in fileArray
  int currSize = 0;
  // Flag to see if we need to add an extra space to the last element in our array
  int flagSpace = 0;
  //index that keeps track of which index we are in in lines
  int linesIndex = 0;
  //index that keeps track of which index we are in in an element
  int elementIndex = 0;
  //Flag that determines if our file grew;
  int growFlag = 1;
  //Variable to hold the length of our lines
  int linesLength;
  //Flag that keeps track of regular file passed in 
  int regFile = 0;

  //We only want to continue creating our 2d array lines, if the file inputted initally was not empty and our file was regular
  if((fileLength != 0) && (S_ISREG(info.st_mode))) {
    regFile = 1;
    while(growFlag == 1) {
      //We want to malloc our fileArray to be the size of our fileLength + 1 so we can put in an EOF
      fileArray = (char *) malloc((fileLength + 1) * sizeof(char));
      checkPointer(fileArray);
      //Make last value in our array equal to EOF
      fileArray[fileLength] =  EOF;
      //Read from stdin and put our information into our fileArray
      read(0, fileArray, fileLength);

      //We will now iterate through our fileArray, figuring out the number of elements, and the maximum size
      //of our elements, so that we can easily allocated data for filling our line
      int i = 0;
      while(i < fileLength) {
	if(fileArray[i] != ' ') {
	  currSize++;
	}
	//When we find a space
	else {
	  numElements++;
	currSize++;
	//Check to see if currSize is bigger than maxsize, if so, switch
	if(currSize > maxSize) {
	  maxSize = currSize;
	}
	currSize=0;
	}
	i++;
      }
      //We need to check for edge cases of if the last element is not an space
      if(fileArray[i-1] != ' ') {
	flagSpace = 1;
	numElements++;
	currSize++;
	if(currSize > maxSize) {
	  maxSize = currSize;
	}
	currSize=0;
      }

      //We are going to check the current state of the buffer, to see if our file is growing
      struct stat info2;
      if(fstat(0, &info2) != 0){
	fprintf(stderr, "Error: fstat read info wrong");
	exit(1);
      }
      //Our secondFileLength will be used to see if our file grew by the end of our file read
      int secondFileLength = info2.st_size;
      if(fileLength != secondFileLength) {
	growFlag = 1;
      }
      else {
	growFlag = 0;
      }

      //If our grow flag was not set, then we know that we need to get out of our while loop, as there was no growth in our file
      if(growFlag == 0) {
	break;
      }
      //If our grow glag was set, we reset our data and go back into our while loop, repeating the process of populating
      //our linesArray
      else {
	//Our new file length should be changed to what q stat gave us
	fileLength=secondFileLength;
	numElements = 0;
	maxSize = 0;
	currSize = 0;
	flagSpace = 0;
	lseek(0, 0, SEEK_SET);
	free(fileArray);
      }
      //Bracket for our while growFlag == 1
    }
    
    //Making lines point to an array of pointer. Initializing the size of the
    //array to be equal to the size of numElements found previously
    lines = (char **)malloc(sizeof(char*) * numElements);
    checkPointer(lines);
    int j = 0;
    //Go through all the indices of our lines array, setting each value inside the index
    //To point to a character array with a size equal to maxSize that we found previously
    while(j < numElements) {
      lines[j] = (char *)malloc(sizeof(char) * maxSize);
      checkPointer(lines[j]);
      j++;
    }
    
    //iterator that goes through the entirety of our fileArray
    int k = 0;
    //We are now going to populate lines
    while(k < fileLength) {
      //keeps track of the current char we are on in the file array
      char currCh = fileArray[k];
      if(currCh != ' ') {
	lines[linesIndex][elementIndex] = currCh;
	elementIndex++;
      }
      else {
	lines[linesIndex][elementIndex] = currCh;
	elementIndex=0;
	linesIndex++;
      }
      k++;
    }
    
    if(flagSpace) {
      lines[linesIndex][elementIndex]= ' ';
      linesIndex++;
      elementIndex=0;
    }

    linesLength=linesIndex;
  }

  //Any other case, when we are asking for std input, in which case, we use previous implementation, except use reads
  else {
    //mallocs for lines so we can store pointer to elements
    lines = (char**)malloc(sizeof(char*));
    checkPointer(lines);

    //mallocs for element so we can store our character arrays
    element = (char*)malloc(sizeof(char));
    checkPointer(element);
    //The very first element in lines will hold the pointer to our very first element. This is
    //the beginning of our "2-D" array
    *lines = element;
    char buf[1];
    //Keeps track of state of read
    int readInt;
    while ((readInt = read(0, buf, 1)) > 0) {
      // First check read for errors every iteration
      checkReadError(lines, linesIndex, readInt);
      //If a space is read in, we know we need to create a new element, and a character in that element accordingly
      if (buf[0] == ' ') {
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
	lines[linesIndex][elementIndex] = buf[0];
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
    //Handles end of file
    if (readInt == 0) {
      //Puts a space in the last character of the last element
      if (elementIndex != 0) {
	lines[linesIndex][elementIndex] = ' ';
      }
      else {
	free(lines[linesIndex]);
	linesIndex--;
      }
    }
    //Handles file error
    else if (readInt < 0) {
      fprintf(stderr, "Error in Standard Input");
      freeData(lines, linesIndex);
      exit(1);
    }
    linesLength = linesIndex + 1;    
  }
  //Sort our two dimesnional arrays, passing in our frobcmpCast function to give the rules of our sorting
  //We will use differnt frobcmpCasts depending on whether we have the -f flag or not
  //if f flag is not activated
  if (!fFlag) {
    qsort(lines, linesLength, sizeof(char*), frobcmpCast);
  }
  else {
    qsort(lines, linesLength, sizeof(char*), frobcmpCastUp);
  }
  printArrays(lines, linesLength);
  freeData(lines, linesLength);
  //if a regular file was passed in, we need to free fileArray
  if(regFile) {
    free(fileArray);
  }
}
  
