import math
import random

euclidian = -1   #Flag for euclidian case

def euclid(p,q):
    x = p[0]-q[0]
    y = p[1]-q[1]
    return math.sqrt(x*x+y*y)

#This function parses the euclidian graph data contained in the specified file.
#The data is parsed by constructing a list that contains the coordinates of
#the different vertices as tuples.
#@Param
#   -> filename - the name of the file that contains the graph data
#@Return
#    -> returns a list of two elements. The first element is the parsed data list
#       containing the coordinates of the vertices, whereas the second element
#       is the number of nodes in the euclidean graph
def euclid_graph_data(filename):
    file = open(filename, "r")

    list = []   #Will store coordinates of nodes as tuples
    count = 0   #Will keep track of the number of nodes in the file
    success = False
    
    while (not success):
        line = file.readline()
        if (line == ""):    #Check if we've reached end of file
            success = True
        else:
            count += 1
            data = tuple(map(int, line.split())) #Convert to integer values
            list.append((data[0], data[1])) #data[0] and data[1] contain the X and Y coordinates respectively

    file.close()
    return [list, count]

#This function constructs the 2-dimensional table that will contain the
#the distances between every pair of nodes in the euclidian graph
#@Param
#   -> data - a list of tuples containing the x,y coordinates of all the nodes in the graph
#@Return
#   -> the constructed 2-dimensional table
def euclidian_table(data):
    n = len(data)
    table = [[0] * n for i in range(0,n)]   #Create 2-dimensional table

    for i in range(0, n):   
        for j in range(0, n):
            distance = euclid(data[i], data[j]) #Calculate euclidian distance between pair of points
            table[i][j] = distance
            table[j][i] = distance

    return table

#This function generates the 2-dimensional TSP weight table for
#more general graph inputs which are not euclidian. The expected format
#of the general inputs are files that contain three integers on each line.
#These indicate the first endpoint, second endpoint and weight respectively.
#@Param
#   -> filename - the name of the file that contains the TSP input
#   -> n - the number of nodes in the TSP input
def general_TSP_table(filename, n):
    file = open(filename, "r")
    table = [[math.inf] * n for i in range(0,n)]  #Initialise 2-dim table using infinity
                                                  #as this should be the default value
                                                  #if there is no path between two nodes
    complete = False

    for i in range(0, n):   #Initialise distances between same nodes to zero, i.e the diagonals
        for j in range(0, n):
            if (i == j):
                table[i][j] = 0
    
    while(not complete):
        line = file.readline()
        if (line == ""):    #Check if we've reached end of file
            complete = True
        else:
            data = tuple(map(int, line.split())) #contains data in the format (start, end, weight)
            table[data[0]][data[1]] = data[2]
            table[data[1]][data[0]] = data[2] #Goes both ways since we assume w(u,v) = w(v,u)!

    file.close()
    return table

#This initialises the permutations of the cities/nodes of the graph
#This is by default initialised with respect to the the identity permutation
#@Param
#   -> n - the number of nodes
#@Return
#   -> list containing the permutation of the nodes
def initPerm(n): #init permutation list - contains node permutations
    list = []
    
    for i in range(0, n):
        list.append(i)

    return list

#This function reverses a specific tour segment of the node permutations
#@Param
#   -> perm - the list of permutations of the nodes
#   -> n - the number of nodes
#   -> i - the start index of the segment in the permutation
#   -> j - the end index of the segment in the permutation
#@Return
#   -> returns the permutation with the reversed segment
def reversePerm(perm, n, i, j):

    i = i%n  #Ensure that a valid index is provided
    j = j%n  #Ensure that a valid index is provided
    
    while(not i == j):  #If they equal then the segment has been reversed
        
        node = perm[i]
        conjugateNode = perm[j]

        perm[i] = conjugateNode
        perm[j] = node

        if (j - i == 1): #Note that if the segment we are reversing is of even length
                         #then want to ensure that i and j indices don't pass each other
            break
        
        i = (i+1)%n
        j = (j-1)%n

        
    return perm


#This function finds the closest node in distance to a specified node,
#subject to the restriction that this node has not yet been used to build the tour
#@Param
#   -> unusedNodes - list containing the set of nodes that have not been used yet to build the tour
#   -> dists - 2-dim array contaning the distances between the different nodes
#   -> currentNode - the search will be with respect to this node
def get_closest_node(unusedNodes, dists, currentNode):
    shortestNode = unusedNodes[0]   #Initialise first element as shortest for now
    
    for i in range(0, len(unusedNodes)):
        nextNode = unusedNodes[i]

        if (dists[currentNode][shortestNode] > dists[currentNode][nextNode]):
            shortestNode = nextNode    

    return shortestNode

#This function constructs the first subtout for the Farthest insertion heuristic
#The initialised subtour are the two vertices that share the highest cost edge
#@Param
#   -> tour - the list containing the permutation of the subtour
#   -> perm - the initial identity permutation constructed in the init method of the graph class
#   -> dists - 2-dim table that encapsulates the weight between the nodes
#   -> n - number of nodes in the graph
#@Return
#   -> list containing the set of uninserted nodes
def initTour(tour, perm, dists, n):
    unusedNodes = perm[:]

    maxEdge = dists[0][0]
        
    for  i in range(0, n):
          for j in range(0, n):
            if (maxEdge < dists[i][j]):
                maxEdge = dists[i][j]
                tour[0] = i #Insert nodes with highest cost edge
                tour[1] = j #Insert nodes with highest cost edge

    unusedNodes.remove(tour[0])
    unusedNodes.remove(tour[1])
    return unusedNodes

#This selects the next node to be inserted into the subtour for the Farthest insertion heuristic
#@Param
#   -> unusedNodes - list containing set of uninserted nodes
#   -> insertedNodes - number of inserted nodes
#   -> tour - the list containing the permutation of the subtour
#   -> dists - 2-dim table that encapsulates the weight between the nodes
#Return
#   -> the selected node
def nextInsertedNode(unusedNodes, insertedNodes, tour, dists):
    node = unusedNodes[0]       #Will hold the new node to be inserted
    maxDist = dists[node][tour[0]]

    for i in range(0, len(unusedNodes)):
        for j in range(0, insertedNodes):
            v = unusedNodes[i]
            u = tour[j]
            if (maxDist < dists[v][u]):
                maxDist = dists[v][u]
                node = v

    return node

#This determines the position in the tour permutation that the selected node
#in the Farthest insertion heuristic should be inserted in. The position is
#determined by investigating if the expression d(u,k) + d(k,v) - d(u,v) is
#minimized for the selected node k.              
#@Param
#   -> node - the node to be inserted
#   -> tour - the list containing the permutation of the subtour
#   -> dists - 2-dim table that encapsulates the weight between the nodes
#   -> insertedNodes - number of inserted nodes
#Return
#   -> A tuple containing the indices (x,y), which indicate the
#      edge that the selected node should alter
def insertPosition(node, tour, dists, insertedNodes):
    x = tour[0]
    y = tour[1]

    insertPos = (0,1)
    tourImprovement = dists[x][node] + dists[node][y] - dists[x][y]
        
    for i in range(0, insertedNodes):
         x = tour[i]
         y = tour[(i + 1)%insertedNodes]
            
         newTourImprovement = dists[x][node] + dists[node][y] - dists[x][y]

         if (tourImprovement > newTourImprovement):
             insertPos = (i, (i + 1)%insertedNodes) #Update insertion position

    return insertPos

#This inserts the selected node into the correct position of the progressively
#larger constructed subtour. The idea is to shift the vertices to the right of the postion
#that the selected node will be inserted in by one step to allow the correct insertion of the node.
#However, if the insertPos edge (x,y) is the wraparound edge then no vertices should be
#shifted and the selected node should just be inserted in the specified location.
#containing the tour permutation to the
#@Param
#   -> tour - the list containing the permutation of the subtour
#   -> node - the node to be inserted
#   -> insertPos - tuple containing (x,y), which indicate the edge that the selected node should alter
#   -> insertedNodes - number of inserted nodes
def insertNode(tour, node, insertPos, insertedNodes):
    
    unalteredTour = tour[:]
    insertIndex = insertPos[1] #The y coordinate
    
    for i in range(0, insertedNodes):
        x = insertPos[0]

        if (x > insertIndex):     #Wraparround case - don't want to shift any vertices
            insertIndex = x + 1   #Adding 1 ensures position is between (x,y)
            break

        if (i >= insertIndex): #Shift all vertices
            tour[i + 1] = unalteredTour[i]

    tour[insertIndex] = node

class Graph:

    # Complete as described in the specification, taking care of two cases:
    # the -1 case, where we read points in the Euclidean plane, and
    # the n>0 case, where we read a general graph in a different format.
    # self.perm, self.dists, self.n are the key variables to be set up.
    def __init__(self,n,filename):
        self.dists = [[]] #Initialize with dummy array for now
        self.n = n
        
        if (n == euclidian):
            data = euclid_graph_data(filename)    #Read in data for the graph
            self.n = data[1]                      #Second element of data contains number of nodes
            self.dists = euclidian_table(data[0]) #Construct euclidian table using the specified coordinates
        else:
            self.dists = general_TSP_table(filename, self.n) #Not euclidian? Generate general TSP table

        self.perm = initPerm(self.n)

    # Complete as described in the spec, to calculate the cost of the
    # current tour (as represented by self.perm).
    def tourValue(self):
        cost = 0
        
        for i in range(0, self.n):
            firstNode = self.perm[i]
            secondNode = self.perm[(i + 1)%self.n] #Don't forget the wraparound edge!

            cost += self.dists[firstNode][secondNode]
        
        return cost

    # Attempt the swap of cities i and i+1 in self.perm and commit
    # commit to the swap if it improves the cost of the tour.
    # Return True/False depending on success.
    def trySwap(self,i):
        current_cost = self.tourValue();

        node = self.perm[i]
        nextNode = self.perm[(i+1)%self.n]
        
        self.perm[i] = nextNode
        self.perm[(i+1)%self.n] = node

        new_cost = self.tourValue()

        if (new_cost < current_cost): #Strictly less than
            return True
        else:
            self.perm[i] = node                 #Swap back to original value
            self.perm[(i+1)%self.n] = nextNode  #Swap back to original value

        return False

    # Consider the effect of reversiing the segment between
    # self.perm[i] and self.perm[j], and commit to the reversal
    # if it improves the tour value.
    # Return True/False depending on success.              
    def tryReverse(self,i,j):
        current_cost = self.tourValue()
       
        self.perm = reversePerm(self.perm, self.n, i, j)

        new_cost = self.tourValue()

        if (new_cost < current_cost):
            return True
        else:
            reversePerm(self.perm, self.n, i, j) #Reverse back if no improvement
        
        return False

    def swapHeuristic(self,k):
        better = True
        count = 0
        while better and (count < k or k == -1):
            better = False
            count += 1
            for i in range(self.n):
                if self.trySwap(i):
                    better = True

    def TwoOptHeuristic(self,k):
        better = True
        count = 0
        while better and (count < k or k == -1):
            better = False
            count += 1
            for j in range(self.n-1):
                for i in range(j):
                    if self.tryReverse(i,j):
                        better = True

                        
    # Implement the Greedy heuristic which builds a tour starting
    # from node 0, taking the closest (unused) node as 'next'
    # each time.
    def Greedy(self):
        self.perm = initPerm(self.n) #Ensuring we are working with the identity permutation initially
        
        unusedNodes = self.perm[1:]  #The first used node is the one indexed at 0
        
        for i in range(0, self.n - 1):
            currentNode = self.perm[i]
            closestNode = get_closest_node(unusedNodes, self.dists, currentNode)
            self.perm[i + 1] = closestNode
            unusedNodes.remove(closestNode)

    def FarthestInsertHeuristic(self):
        tour = [None] * self.n
        unusedNodes = initTour(tour, self.perm, self.dists, self.n) #Note: this function inserts two nodes into the tour list that is passed to it

        insertedNodes = 2    #Keeps track of number of inserted nodes
        success = False
        
        while(not success):
            if(unusedNodes):
                node = nextInsertedNode(unusedNodes, insertedNodes, tour, self.dists)
                insertPos = insertPosition(node, tour, self.dists, insertedNodes)

                insertNode(tour, node, insertPos, insertedNodes)

                insertedNodes += 1
                unusedNodes.remove(node)
            else:
                success = True
        
        self.perm = tour[:]



                
