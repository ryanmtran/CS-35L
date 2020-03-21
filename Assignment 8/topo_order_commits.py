import os
import sys
import zlib

#Class that defines a CommitNode which consists of a commit's hash, parents, and children
class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()
        self.branch_names = set()

#function that takes us to the directory where the .git is 
def find_top_git():
    while 1:
        try:
            #grabs the entire tuple of current directory
            collection = next(os.walk('.'))
            string=[]
            #We do not want the cwd to end with '//'
            if len(os.getcwd())>= 2:
                string.append(os.getcwd()[-2])
                string.append(os.getcwd()[-1])
                if string[0]=='/' and string[1]=='/':
                    sys.stderr.write('Not inside a Git repository')
                    exit(1)
            #If we encounter a /, we did not find the git repository
            if os.getcwd() == '/':
                sys.stderr.write('Not inside a Git repository')
                exit(1)
            dir_collection=collection[1]
            for word in dir_collection:
                if word == '.git':
                    return
            os.chdir('..')
        except StopIteration:
            sys.stderr.write('Not inside a Git repository')
            exit(1)

def get_branches():
    #Invoke findTopGit to get the the directory where.git is found
    find_top_git()
    #Store the contents of the current directory into collection
    collection=next(os.walk('./.git/refs/heads'))
    #recall that branches are files, and files would be stored in index 2 of our collection
    branch_collection=collection[2]
    return branch_collection

#Function to build our commit graph
def build_branch_node_arr():
    #We are going to have an array of branch nodes that we will return that will coincide with all of our branche's nodes (which include their id, children, and parents)
    final_branch_arr=[]
    #dictionary that maps our commit node to our branch names so that we know if there are repeats
    
    branch_to_node = {}

    branches=get_branches()
    #print(get_branches())
    #Goes through all our branches and grabs the files
    for file_name in branches:
        #opens all the files found in our branches
        f= open('./.git/refs/heads/' + file_name,"r")
        if f.mode == 'r':
            contents=f.readlines()
            #print(contents)

            #We are going to iterate through our contents because we may have
            #multiple hashes and extrapolate data accordingly
            for hasher in contents:
                #This will be the nodes we start off with that are given by our branches
                #Note that there wsill be a new line included if we do not remove last character
                
                #The value of the commit_hash
                commit_val = hasher[0:len(hasher)-1]
                #print(commit_val)

                #if we already worked with a commit_val in our dictionary, we know that there is a duplicate branch on the same commit
                if commit_val in branch_to_node:
                    branch_to_node[commit_val].branch_names.add(file_name)
                    
                #Create new commit node because we have never created a node with our commit_val
                else:
                    new_node = CommitNode(commit_val)
                    new_node.branch_names.add(file_name)
                    branch_to_node[commit_val] =  new_node

                #Keeps track of the two number directory that is in objects 
                two_num_dir=hasher[0:2]
                #Corresponds with abridged hasher
                hasher_shorter=hasher[2:(len(hasher)-1)]
                #print(hasher_shorter)
                #used to open our file and read binary
                open_file=open('./.git/objects/'+ two_num_dir + '/' + hasher_shorter, "rb")
                #Decompresses the read data from our file
                dec_data=str(zlib.decompress(open_file.read()))
                #print('\n' + str(dec_data))
                
                
                #Now we need to iterate through our data, and find every instance of parent with the corresponding id
                parent_arr = []
                if("parent" in dec_data):
                    while("parent" in dec_data):
                        #grab all data after instance of "parent "
                        dec_data = dec_data.partition('parent ')[2]
                        parent_hash= dec_data[0:40]
                        parent_arr.append(parent_hash)
                        #print('\n' + parent_hash)
                if(len(parent_arr) != 0):
                   for parent in parent_arr:
                       branch_to_node[commit_val].parents.add(parent)

    #Take our dictionary and iterate through it to add to our final_branch_arr
    for branch_commit in branch_to_node:
        final_branch_arr.append(branch_to_node[branch_commit])

    return final_branch_arr

def build_dictionary_and_roots():

    #the array the holds all of the nodes of our branches, which we will utilize for comparison later
    branches_arr=build_branch_node_arr()
    #We need a reached set that will keep track of all of the commits we have already reached
    reached=set() 
    #in the very beginning, we want our stack to be comprised of all the nodes of our branch array
    #This is how we inititate our algorithm below, that populates all the data of the commit nodes and puts them in a dictionary
    stack=build_branch_node_arr()
    #We also want to populate the dictionary with all of our initial branches
    dictionary={}
    for branch in branches_arr:
        dictionary[branch.commit_hash] = branch
    #Set that will hold all of the roots of our tree
    root_commits=set()
    
    #print(branches_arr[0].parents)
    for branch in branches_arr:
        if len(branch.parents)==0:
            root_commits.add(branch)

    #print(len(branches_arr))
    while(stack):
        curr_node=stack.pop()
        reached.add(curr_node.commit_hash)
        #print(node.commit_hash)
        for parent_hash in curr_node.parents:
            if parent_hash not in reached:
                #Makes new node equal to the node found in dictionary if it is already created
                if(parent_hash in dictionary):
                    new_node=dictionary[parent_hash]
                else:
                    new_node = CommitNode(parent_hash)
                #Keeps track of the two number directory that is in objects
                two_num_dir = parent_hash[0:2]
                hasher_shorter = parent_hash[2:len(parent_hash)]
                #print(hasher_shorter)
                #used to open our file and read binary
                open_file=open('./.git/objects/'+ two_num_dir + '/' + hasher_shorter, "rb")
                #Decompresses the read data from our file
                dec_data=str(zlib.decompress(open_file.read()))
                #We want to set the current node's children to be the recently popped node
                new_node.children.add(curr_node.commit_hash)
                
                #Update the stack and dictionary accordingly
                stack.append(new_node)
                dictionary[parent_hash]=new_node

                #Now we are going to iterate through our decrypted data and find all the parents
                if("parent" in dec_data):
                    while("parent" in dec_data):
                        #grab all data after instance of "parent "
                        dec_data = dec_data.partition('parent ')[2]
                        parent_hash= dec_data[0:40]
                        new_node.parents.add(parent_hash)
                        #print(parent_hash)
                #When parent component is not found in decrypted data, we know that it is a root       
                else:
                    #flag to keep track of whether or not our root node we found has already been added when we added the roots if the branches had no parents
                    #print(new_node.commit_hash)
                    flag = 0
                    for branch in branches_arr:
                        if(branch.commit_hash == new_node.commit_hash):
                            flag = 1
                    #We have already added the root, so no need to add it again
                    if(flag != 1):
                        root_commits.add(new_node)
    
            #when parent_hash has already been reached, simply update its children to be the node we just popped
            else:
                dictionary[parent_hash].children.add(curr_node.commit_hash)
    return [dictionary, root_commits]
                
"""
#Testing purposes                
 #========================================================
    for line in dictionary:
        print(line)
        print(' Parents:')  
        print(dictionary[line].parents)
        print(' Children:')
        print(dictionary[line].children)
        print('\n')
   # ============================================================
"""    
    

def get_topo_ordered_vertices():
    get_dictionary_and_roots=build_dictionary_and_roots()
    dictionary=get_dictionary_and_roots[0]

    roots=get_dictionary_and_roots[1]
    root_commits=[]
    #We are going to iterate through all of our roots from our previous function and append them to our roots_commits array
    for r in roots:
        root_commits.append(r.commit_hash)
    #Array to keep track of the final order of the tree
    order = []
    #Keeps track of already visited nodes
    visited = set() # visited is the union of the gray and black vertices
    #stack that permits iteration through our data
    stack = root_commits

    while stack:
        v = stack[-1]
        visited.add(v)
        count = 0

        for children in dictionary[v].children:
            if children not in visited:
                stack.append(children)
                count = count + 1
        if count == 0:
            order.append(v)
            stack.pop()
    return [order, dictionary]


def print_commits():
    stack=get_topo_ordered_vertices()[0]
    dictionary=get_topo_ordered_vertices()[1]

    #Iterating through the entirety of our array
    for i in range(len(stack)):
        v=stack[i]
        #We first print our or node commit in the front of our list
        print(v,end='')

        #Now we print out branches for our specific node
        sorted_branches=sorted(dictionary[v].branch_names)
        for branch in sorted_branches:
            print(' ' + branch, end='')
        print('\n', end='')

        #Check to make sure we are not using the last element in our list because we are going to be acessing the next index after that
        if(i != len(stack)-1):
            #We will print out sticky ends
            if v not in dictionary[stack[i+1]].children:
                #Create parent_list array so that we can index into it
                parent_list=sorted(dictionary[v].children)
                for index in range(len(parent_list)):
                    #Last item in the list should not have a space after it
                    if (index == len(parent_list)-1):
                        print(parent_list[index], end='')
                    else:
                        print(parent_list[index] + ' ', end='')
                #After printing out sticky end, print an equal sign and new line
                print('=\n\n=', end='')
                #now we print the sticky start
                #we will create a child_list array from our set so we can index it
                child_list=sorted(dictionary[stack[i+1]].parents)
                for index in range(len(child_list)):
                    #if our element is the very first element, we do not want to print a space in the end
                    if (index == 0):
                        print(child_list[index], end='')
                    else:
                        print(' ' + child_list[index], end='')
                #print a new line in the end
                print('\n', end='')
            
def main():
   #for branch in build_branch_node_arr():
       #print(branch.parents)
    print_commits()
    
   # build_dictionary_and_roots()

if __name__ == "__main__":
    main()
