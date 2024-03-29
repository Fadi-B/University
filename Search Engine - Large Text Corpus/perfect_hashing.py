
# Inf2-IADS Coursework 1, October 2020
# Python source file: perfect_hashing.py
# Author: John Longley

# TEMPLATE FILE
# Please add your code at the point marked: # TODO


# PART C: A STATE-OF-THE-ART PERFECT HASHING SCHEME

# Adapting a method of Belazzougui, Botelho and Dietzfelbinger 2008


# Start with very crude 'mod' hashing

# First, let's read a lowercase word as a base 27 integer:

def toInt(w):
    b = w.encode()
    t = 0
    for i in range(len(b)):
        t = t*27 + b[i] - 96
    return t

# Simple mod hash with some scrambling
# (we want hashes mod p,p' to be 'independent' when p != p')
# We shall take p prime for the outer hash, but not necessarily the inner ones

def modHash(s,p):
    return (toInt(s)*21436587 + 12345678912345) % p

# Classic 'bucket array' hash table:

def buildHashTable(L,h,r):
    table = [[] for i in range(r)]
    for w in L:
        table[h(w)].append(w)
    return table

def buildModHashTable(L,p):
    return buildHashTable (L, lambda w: modHash(w,p), p)
    # worth trying out for small L and p
    

# Finding a suitable prime for the outer hash:

def isPrime(n):
    if n%2==0 and n!=2: return False
    else:
        j = 3
        while j*j <= n:
            if n%j==0: return False
            else: j += 2
        else: return True

def prevPrime(n):
    if n%2==0: return prevPrime(n-1)
    elif isPrime(n): return n
    else: return prevPrime(n-2)


# For the mini-hashes, the following very simple enumeration works just fine
# (moduli needn't be prime, but we at least avoid multiples of 2 or 3)
# Results will later be further reduced modulo m (main table size)

def miniHash(m,j):
    d = j*6 + 3000001
    return (lambda w: modHash(w,d) % m)

# TODO
# Add your code here.

#Returns array R containing the suitable values of j
def hashCompress(L, m):
    L = sortList(L)     #We get [([bucket1], i), ([bucket2], k)...]
    T = [False] * m     #Will keep track of occupied slots
    R = [0] * len(L)    #List that will contain suitable number of j for the buckets
    
    for i in range(0, len(L)):
        bucket = L[i][0]
        initialBucketPos = L[i][1]

        suitableJ = stdHashEnum(bucket, T, m)
        T  = suitableJ[1]   #Updates list of occupied slots
        
        R[initialBucketPos] = suitableJ[0]  #Puts value of suitable j at corresponding bucket index
            
    return R

#Enumerates through the possible values of j until a successful one is found
#Returns a list containing the suitable j and the
#newly adjusted list of occupied slots - [j, T]
def stdHashEnum(bucket, T, m):
    j = -1
    found = False
    
    while (not(found)):
        j = j + 1
        
        result = probe(bucket, T, m, j)
        if (result[0] == True): #Check if probing successful
            found = True

        T = result[1] #Update occupied indices
        
    return [j, T]

#Probes the bucket elements for a given j
#Returns a list containing a boolean indicating if the probe was successful
#and the newly adjusted list of occupied slots  - [success, T]
def probe(bucket, T, m, j):
    initialSlots = T[:]     #The intially occupied slots before any adjustments are made
    success = True

    for k in range(0, len(bucket)):
        index = miniHash(m, j)(bucket[k]) #Index that key will occupy in table
        
        if (T[index] == False): #Then free slot
            T[index] = True     #Position now occupied
        else:
            success = False     #This value of j not suitable so quit
            break
    if (success):
        return [success, T] #Suitable J? Return the updated slots as well
    else:
        return [success, initialSlots] #Not Suitable J? Return the untouched slots

#Sorts a list in decreasing order and returns the
#elements with a mapping to their previous index
#[(element1, i), (element2, k)...]
def sortList(L):
    return mergeSort(L, 0, len(L))

#The classic mergeSort algorithm from class, but slightly adjusted
def mergeSort(L,m,n):
    p = (m+n)//2
    if (n-m) == 1:
        return [(L[m], p)]  #Map element to initial index as well

    else:
        B = mergeSort(L,m,p)
        C = mergeSort(L,p,n)
        D = merge(B,C)
    return D

#The classic merge algorithm from lab sheet 2, but slightly adjusted
#It will be called on lists of buckets
def merge(B,C):
    D = [0] * (len(B)+len(C))
    i,j = 0,0
    for k in range(0,len(D)):
        if (i < len(B) and (j == len(C) or len(B[i][0]) > len(C[j][0]))):
            # this does what we want even when i,j out of range for B,C
            D[k] = B[i]
            i += 1
        else:
            D[k] = C[j]
            j += 1
    return D
            
# Putting it all together:
# compact data structure for representing a perfect hash function

class Hasher:
    def __init__ (self,keys,lam,load):
        # keys : list of keys to be hashed
        # lam  : load on outer table, i.e. average bucket size
        #        (broadly, higher lam means more compression, 
        #        but perfect hash function will be harder to construct)
        # load : desired load on resulting hash table, must be < 1
        # hashEnum : enumeration osof hash functions used (e.g. miniHash)
        self.n = len(keys)
        self.r = prevPrime (int(self.n//lam))
        self.m = int(self.n//load)
        HT = buildModHashTable(keys,self.r)
        self.hashChoices = hashCompress(HT,self.m)
        # results in a very small data structure with no trace of keys!
    def hash (self,key):
        i = modHash (key,self.r)
        h = miniHash (self.m, self.hashChoices[i])
        return h(key)

# Example: Try this with keys = MetaIndex.keys()

# We can double-check that our hash function really is perfect
# by building the corresponding ordinary hash table:

def checkPerfectHasher (keys,H):
    T = buildHashTable (keys, lambda key: H.hash(key), H.m)
    clashes = [b for b in T if len(b)>=2]
    if len(clashes)==0:
        print("No clashes!")
        # return T
    else:
        print("Clashes found.")
        return clashes

  
# NOT YET IMPLEMENTED:
# Reordering the main index so that the index entry for key
# lives at the line number given by hashWith(C,key)


# FOR INTEREST ONLY:

# Calculating 'essential size' of a Hasher, given a crude compression scheme
# (compression itself not yet implemented)

import math

def compressedSizeOf(H,bitWidth,maxOutlierSize):
    cutoff = 2 ** bitWidth - 1
    outliers = len([j for j in H.hashChoices if j >= cutoff])
    intermedKeySize = math.ceil(math.log2(H.r))
    return (((H.r - outliers) * bitWidth) +
            (outliers * (maxOutlierSize + intermedKeySize)))

def bestCompression(H):
    maxOutlierSize = math.ceil(math.log2(max(H.hashChoices)))
    comprList = [(i,compressedSizeOf(H,i,maxOutlierSize))
                 for i in range(3,maxOutlierSize)]
    best = comprList[0]
    for i in range(1,len(comprList)):
        if comprList[i][1] < best[1]:
            best = comprList[i]
    return {'bestBitWidth' : best[0],
            'totalBitSize' : best[1],
            'bitsPerKey'   : best[1]/H.n}

# End of file
