
# Inf2-IADS Coursework 1, October 2020
# Python source file: search_queries.py
# Author: John Longley

# TEMPLATE FILE
# Please add your code at the point marked: # TODO


# PART B: PROCESSING SEARCH QUERIES

import index_build

# We find hits for queries using the index entries for the search terms.
# Since index entries for common words may be large, we don't want to
# process the entire index entry before commencing a search.
# Instead, we process the index entry as a stream of items, each of which
# references an occurrence of the search term.

# For example, the (short) index entry

#    'ABC01,23,DEF004,056,789\n'

# generates a stream which successively yields the items

#    ('ABC',1), ('ABC',23), ('DEF',4), ('DEF',56), ('DEF',789), None, None, ...

# Item streams also support peeking at the next item without advancing.

class ItemStream:
    def __init__(self,entryString):
        self.entryString = entryString
        self.pos = 0
        self.doc = 0
        self.comma = 0
    def updateDoc(self):
        if self.entryString[self.pos].isalpha():
            self.doc = self.entryString[self.pos:self.pos+3]
            self.pos += 3
    def peek(self):
        if self.pos < len(self.entryString):
            self.updateDoc()
            self.comma = self.entryString.find(',',self.pos)
                    # yields -1 if no more commas after pos
            line = int(self.entryString[self.pos:self.comma])
                    # magically works even when comma == -1, thanks to \n
            return (self.doc,line)
        # else will return None
    def pop(self):
        e = self.peek()
        if self.comma == -1:
            self.pos = len(self.entryString)
        else:
            self.pos = self.comma + 1
        return e

# TODO
# Add your code here.
class HitStream:
    def __init__(self, itemStreams, lineWindow, minRequired):
        self.itemStreams = itemStreams
        self.lineWindow = lineWindow
        self.minRequired = minRequired
        self.counter = 0     #Counter for keys found
        self.success = False #Flag for a successful hit lookup
        self.file = ""       #The file we are examining
        self.pos = 0         #Position of line we're at in a file

    #Finds the next hit
    #@Returns - next hit of the query
    def next(self):
        while(not(self.success)):
            self.counter = 0 #Reset counter
            firstEntry = self.getFirstEntry()
                
            if (firstEntry == None): #Then there are no more entries so just return none
                return None

            file = firstEntry[0]    #File we're examining
            if (not(self.file == file)):
                self.pos = 0        #New file? then reset line position

            searchResult = self.search(firstEntry)

        self.pos = int(searchResult[1]) + 1  #Should be reset on newfile
        self.file = searchResult[0]         #Store file currently in
        self.success = False        #Reset boolean for next search
        return searchResult

    #Searches for a hit in the range of the specified entry
    #@Param firstEntry - the entry we're searching with respect to
    #@Returns - firstEntry
    #@SideEffect - updates success flag if hit found
    def search(self, firstEntry):
        for i in range(len(self.itemStreams)):
            nextEntry = self.itemStreams[i].peek()
            
            if (nextEntry == None):
                continue
            
            if (firstEntry == nextEntry): #Remove entries situated on line we're examining
                self.updateCounter(firstEntry, nextEntry)
                self.itemStreams[i].pop()   
            else:
                self.updateCounter(firstEntry, nextEntry)
                
            if (self.counter == self.minRequired): #Found enough keys? Success!
                self.success = True
        return firstEntry

    #Finds the first occuring entry in the corpus
    #@Returns - first occuring entry - ('corpusfile', line)
    #           or none if no more entries exist
    def getFirstEntry(self):
        entry = self.itemStreams[0].peek()
        
        for i in range(1, len(self.itemStreams)):
            nextEntry = self.itemStreams[i].peek()
            
            if (entry == None):
                entry = nextEntry
            if (not(entry == None) and not(nextEntry == None)):
                entry = min(entry, nextEntry)
        return entry

    #@Param - Two entries whose line distance will be compared to the specified line window
    #Returns - boolean indicating if the distance between entries is within the line window
    def isWithinLineWindow(self, entry, nextEntry):
        difference = abs(int(entry[1]) - int(nextEntry[1]))
        return difference < self.lineWindow

    #Examines whether two entries are to be considered matches
    #@Param - The two entries to be examined
    def updateCounter(self, entry, nextEntry):
        if (not(entry[0] == nextEntry[0])): #Not same file? return
            return
        if (int(entry[1]) < self.pos): #Ignore duplicates                                       
            return
        if (self.isWithinLineWindow(entry, nextEntry)):
            self.counter = self.counter + 1
    
def streamTest():
    #index_build.buildIndex()
    index_build.generateMetaIndex("index.txt")
    A = 'CAA5,5,6\n'
    B = 'CAA5,6,DCC3\n'
    C = 'CAA5,6,DCC3,4\n'
    #A = 'ABC01,03,02\n'
    #B = 'ABC02,01,01\n'
    #C = 'ABC03,04,04,05\n'
    G = [ItemStream(A), ItemStream(B), ItemStream(C)]
    
    return HitStream(G, 1, 3)
    
# Displaying hits as corpus quotations:

import linecache

def displayLines(startref,lineWindow):
    # global CorpusFiles
    if startref is not None:
        doc = startref[0]
        docfile = index_build.CorpusFiles[doc]
        line = startref[1]
        print ((doc + ' ' + str(line)).ljust(16) +
               linecache.getline(docfile,line).strip())
        for i in range(1,lineWindow):
            print (' '*16 + linecache.getline(docfile,line+i).strip())
        print ('')

def displayHits(hitStream,numberOfHits,lineWindow):
    for i in range(0,numberOfHits):
        startref = hitStream.next()
        if startref is None:
            print('-'*16)
            break
        displayLines(startref,lineWindow)
    linecache.clearcache()
    return hitStream


# Putting it all together:

currHitStream = None

currLineWindow = 0

def advancedSearch(keys,lineWindow,minRequired,numberOfHits=5):
    indexEntries = [index_build.indexEntryFor(k) for k in keys]
    if not all(indexEntries):
        message = "Words absent from index:  "
        for i in range(0,len(keys)):
            if indexEntries[i] is None:
                message += (keys[i] + " ")
        print(message + '\n')
    itemStreams = [ItemStream(e) for e in indexEntries if e is not None]
    if len(itemStreams) >= minRequired:
        global currHitStream, currLineWindow
        currHitStream = HitStream (itemStreams,lineWindow,minRequired)
        currLineWindow = lineWindow
        displayHits(currHitStream,numberOfHits,lineWindow)

def easySearch(keys,numberOfHits=5):
    global currHitStream, currLineWindow
    advancedSearch(keys,1,len(keys),numberOfHits)

def more(numberOfHits=5):
    global currHitStream, currLineWindow
    displayHits(currHitStream,numberOfHits,currLineWindow)

# End of file
