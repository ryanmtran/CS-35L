import argparse
import sys
import random


#Checks to make sure our number for head count is valid
def headCountCheck(num):
    if (num < 0):
                print("shuf: invalid line count " + "‘" + str(num) + "’" )
                exit(1)

#given an array, prints random values in that array with no repeats allowed
def printRandNReps(arr):
    #counter that holds the last index of our array
    count=len(arr)-1;
    while count >= 0:
        #computes a random index
        randIndex = random.randint(0,count)
        print(arr[randIndex])
        arr.pop(randIndex)
        count=count-1

#given an array, print num of random values, no repeats allowed
def printRandNRepsWithNum(arr, num):
    #counter that holds the last index of our array
    count=len(arr)-1
    iterations=num
    while iterations > 0:
        #computes a random index
        randIndex = random.randint(0,count)
        print(arr[randIndex])
        arr.pop(randIndex)
        count=count-1
        iterations=iterations-1


#function takes in the array given in inputRange and checks to make sure it is valid. If it is, it should randomly print out the values in the input range with no repeats
def printRandomInputRangeNoReps(arr):
    #count signifies last index of array
    intArr = storeInputRangeArr(arr)
    count=len(intArr)-1;
    #while loop that will randomly grab index, print out value, and take it out. Does this until array is empty
    while count >= 0:
        #computes a random index
        randIndex = random.randint(0,count)
        #prints out our random value
        print(intArr[randIndex])
        intArr.pop(randIndex)
        count=count-1

#function takes in the array given in inputRange and checks to make sure it is valid. If it is, store them in an array
def storeInputRangeArr(arr):
    #Check to see size of input range to make sure user didn't put more than one input
    if len(arr) > 1:
        print("shuf: multiple -i options specified")
        exit(1)
    else:
        #variable to store input string that denotes our range of values
        inputStr = arr[0]
        #holds our string split by a '-'
        splitStr = inputStr.split("-")
        if len(splitStr) != 2:
            print("shuf: invalid input range "  + "‘" + inputStr  + "’")
            exit(1)
            # + "‘"  + "’"
        if not splitStr[0].isdigit() or not splitStr[1].isdigit():
            print("shuf: invalid input range "  + "‘" + inputStr  + "’")
            exit(1)
        num1=int(splitStr[0])
        num2=int(splitStr[1])
        if num2 < num1 or num1 < 0 or num2 < 0:
            print("shuf: invalid input range "  + "‘" + inputStr  + "’")
            exit(1)
        #array that holds our integer range
        intArr=[]
        #we need to populate intArr
        for i in range(num1, num2+1):
            intArr.append(i)
        return intArr

#defines the conditions of what happens depending on the give values in args
def condExec(argVals):

#0 Argument Cases and - case   

    #condition where we enter python3 shuf.py alone. If this is the case, we should get data for shuf.py from stdin
    if len(sys.argv) == 1 or (argVals.fileName=="-" and len(sys.argv)==2):
        #holds lines that are inputted into std input
        inputLines = []
        #reads in data from standard input
        for line in sys.stdin:
            #appends line into our array data, taking out new lines as we go
            inputLines.append(line.rstrip())
        #calls function to print our our values in inputLines randomly with no repeats        
        printRandNReps(inputLines)        

#1 argument cases

    #fileName case with no other arguemnts
    elif argVals.fileName and not argVals.inputRange and argVals.headCount is None and not argVals.repeat:
        #try catch to see if we can open the file
        try:
            #opens file and puts it into fileText
            fileText = open(argVals.fileName)
            #array to hold the lines of our files
            fileLines=[]
            #iterates through our fileText and stores the lines into the arrays
            for line in fileText:
                fileLines.append(line.rstrip())
            printRandNReps(fileLines)
        #if file does not exist
        except IOError:
            print( "shuf: " + argVals.fileName + ": No such file or directory")
            exit(1)

    #input range case (-i)
    elif not argVals.fileName and argVals.inputRange and argVals.headCount is None and not argVals.repeat:
        #passes in the array from argVals.inputRange to check if our inputRange is valid and if so, prints out the corresponding range randomly
        printRandomInputRangeNoReps(argVals.inputRange)

    #head count case (-n)
    elif  not argVals.fileName and not argVals.inputRange and argVals.headCount is not None and not argVals.repeat:
        headCountCheck(argVals.headCount)
        #holds lines that are inputted into std input
        inputLines = []
        #reads in data from standard input
        for line in sys.stdin:
            #appends line into our array data, taking out new lines as we go
            inputLines.append(line.rstrip())
        headCountCheck(argVals.headCount)
        
        #if head count is less than or equal to length of stdin
        if argVals.headCount <= len(inputLines):
            printRandNRepsWithNum(inputLines, argVals.headCount)
        else:
            #if head count is greater than length of stdin
            printRandNReps(inputLines)
            

    #repeat case (-r)
    elif not argVals.fileName and not argVals.inputRange and argVals.headCount is None and argVals.repeat:
        #array to store data
        inputLines = []
        #reads in data from standard input
        for line in sys.stdin:
            #appends line into our array data, taking out new lines as we go
            inputLines.append(line.rstrip())
        lastIndex=len(inputLines)-1
        while(1):
            randIndex = random.randint(0,lastIndex)
            print(inputLines[randIndex])

#2 Argument Cases

    # inputRange (-i) and head-count (-n) case
    elif not argVals.fileName and argVals.inputRange and argVals.headCount is not None and not argVals.repeat:
        #stores the valid integers in our input range array
        intArr=storeInputRangeArr(argVals.inputRange)
        #stores the last index of our array
        lastIndex=len(intArr)-1
        
        #check for when input is negative
        if (argVals.headCount < 0):
            print("shuf: invalid line count " + "`" + str(argVals.headCount) + "’" )
            exit(1)

        #if the headCount is less than or equal to the length of our integer array
        if argVals.headCount <= len(intArr):
            #iterates the number of times in head count
            for i in range (0,argVals.headCount):
                randIndex = random.randint(0,lastIndex)
                print(intArr[randIndex])
                intArr.pop(randIndex)
                lastIndex=lastIndex-1
        else:
            #randomly prints values in our array
            printRandNReps(intArr)
    # input range (-i) and (-r) case
    elif not argVals.fileName and argVals.inputRange and argVals.headCount is None and argVals.repeat:
        intArr=storeInputRangeArr(argVals.inputRange)
        lastIndex=len(intArr)-1
        while(1):
            randIndex = random.randint(0,lastIndex)
            print(intArr[randIndex])
    #file and input range (-i) case
    elif argVals.fileName and argVals.inputRange and argVals.headCount is None and not argVals.repeat:
        print("Segmentation Fault")
        exit(1)

    # head-count (-n) and repeat (-r) case
    elif not argVals.fileName and not argVals.inputRange and argVals.headCount is not None and argVals.repeat:
        headCountCheck(argVals.headCount)
        inputLines=[]
        #reads in data from standard input
        for line in sys.stdin:
            #appends line into our array data, taking out new lines as we go
            inputLines.append(line.rstrip())
        if len(inputLines) == 0:
            print("Segmentation Fault")
            exit(1)
        #creates loop that iterates the number of our headCount times
        for i in range (0,argVals.headCount):
            #find random index
            randIndex = random.randint(0, len(inputLines)-1)
            print(inputLines[randIndex])

    #head-count (-n) and file case
    elif argVals.fileName and not argVals.inputRange and argVals.headCount is not None and not argVals.repeat:
        #try catch to see if we can open the file
        try:
            #opens file and puts it into fileText
            fileText = open(argVals.fileName)
            #when input is negative
            if (argVals.headCount < 0):
                print("shuf: invalid line count " + "`" + str(argVals.headCount) + "’" )
                exit(1)
            #array to for lines in the file
            fileLines=[]
            #populate fileLines
            for line in fileText:
                fileLines.append(line.rstrip())
            #if our head count is the same size or larger than fileLines, simply print out our fileText
            if argVals.headCount >= len(fileLines):
                printRandNReps(fileLines)
            else:
                #prints the random values in fileLines (headcount number)
                printRandNRepsWithNum(fileLines, argVals.headCount)
                
            #if file does not exist
        except IOError:
            print( "shuf: " + argVals.fileName + ": No such file or directory")
            exit(1)
    #repeat (-r) and file case
    elif argVals.fileName and not argVals.inputRange and argVals.headCount is None and argVals.repeat:
        #try catch to see if we can open the file
        try:
            #opens file and puts it into fileText
            fileText = open(argVals.fileName)
            #array to for lines in the file
            fileLines=[]
            #populate fileLines
            for line in fileText:
                fileLines.append(line.rstrip())
            if len(fileLines) == 0:
                print("Segmentation Fault")
                exit(1)
            lastIndex = len(fileLines) - 1
            #infinte loop
            while 1:
                randIndex = random.randint(0, lastIndex)
                #print random line
                print(fileLines[randIndex])

        #if file does not exist
        except IOError:
            print( "shuf: " + argVals.fileName + ": No such file or directory")
            exit(1)
#3 Arguments Cases

    #input range (-i) head-count (-n) and repeat (-r) case
    elif not argVals.fileName and argVals.inputRange and argVals.headCount is not None and argVals.repeat:
        headCountCheck(argVals.headCount)
        intArr= storeInputRangeArr(argVals.inputRange)
        #Only prints head-count values
        for i in range (0, argVals.headCount):
            #generates a random index between 0 and last index
            randIndex = random.randint(0, len(intArr)-1)
            print(intArr[randIndex])

    #input-range (-i) head-count (-n) and file case
    elif argVals.fileName and argVals.inputRange and argVals.headCount is not None and not argVals.repeat:
        print("Segmentation Fault")
        exit(1)
    #input-range (-i) repeat (-r) and file case
    elif argVals.fileName and argVals.inputRange and argVals.headCount is None and argVals.repeat:
        print("Segmentation Fault")
        exit(1)
    #head-count (-n) repeat (-r) and file case
    elif argVals.fileName and not argVals.inputRange and argVals.headCount is not None and argVals.repeat:
        #try catch to see if we can open the file
        headCountCheck(argVals.headCount)
        #if head count is 0, initate infinite loop
        if argVals.headCount == 0:
            for line in sys.stdin:
                continue
        try:           
            #opens file and puts it into fileText
            fileText = open(argVals.fileName)
            #array to hold the lines of our files
            fileLines=[]
            #iterates through our fileText and stores the lines into the arrays
            for line in fileText:
                fileLines.append(line.rstrip())

            if len(fileLines)==0:
                print("Segmentation Fault")
                exit(1)
            #creates loop that iterates the number of our headCount times
            for i in range (0,argVals.headCount):
                #finde random index
                randIndex = random.randint(0, len(fileLines)-1)
                print(fileLines[randIndex])                
        #if file does not exist    
        except IOError:
            print( "shuf: " + argVals.fileName + ": No such file or directory")
            exit(1)
#4 Argument Case

    elif argVals.fileName and argVals.inputRange and argVals.headCount is not None and argVals.repeat:
        print("Segmentation Fault")
        exit(1)
def main():
    parser=argparse.ArgumentParser()
    #adds optional positional argument so that the individual can apply the shuf command to a file
    parser.add_argument('fileName', nargs='?')
    #action is append so we can later check if there are multiple i's inputted
    parser.add_argument('-i','--input-range', dest='inputRange', action='append', help="prints numbers in range in random order to standard output")
    parser.add_argument('-n', '--head-count', dest='headCount', help="print at most the number of values passed in as argument", type=int)
    parser.add_argument('-r','--repeat', dest='repeat', help="will repeatedly print out specific command, unless given -n, which will print exactly COUNT lines with random sampling and replacement", action='store_true')
    args = parser.parse_args()
    condExec(args)

    
if __name__ == "__main__":
    #creates Argument Parser
    # parser = argparse.ArgumentParser()
    # parser.add_argument('-e')
   # print args
    main()
