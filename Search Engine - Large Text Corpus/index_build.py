
# Inf2-IADS Coursework 1, October 2020
# Python source file: index_build.py
# Author: John Longley

# TEMPLATE FILE
# Please add your code at the point marked: # TODO


# PART A: INDEXING A LARGE SET OF PLAINTEXT FILES

import buffered_io
from buffered_io import *

# Global variables

CorpusFiles = { 'CAA' : 'Carroll_Alice_in_Wonderland.txt',
                'DCC' : 'Dickens_Christmas_Carol.txt',
                'SJH' : 'Stevenson_Jekyll_and_Hyde.txt',
                'SCW' : 'Shakespeare_Complete_Works.txt',
                'TWP' : 'Tolstoy_War_and_Peace.txt',
               }
# each file should be identified by a three-letter code
# CAA, DCC, SJH are small, SCW and TCP are larger

IndexFile = 'index.txt'
# name of main index file to be generated

MetaIndex = {'' : 0}
# dictionary to be populated
# MetaIndex[k] will give line number in IndexFile for key k

MetaIndexOp = (lambda s: 0)


# Initial scan to determine number of lines in a given text file

def getNumberOfLines(filename):
    reader = BufferedInput(filename,0.8)
    lines = 0
    chunk = reader.readchunk()
    while chunk != []:
        lines += len(chunk)
        chunk = reader.readchunk()
    reader.close()
    return lines

# Extracting list of words present in a single text line:

def getWords(s):
    t = s   # could do some translation here to process accented symbols etc.
    words,flg = [],False
    for i in range(len(t)):
        if not flg:
            if t[i].isalpha():
                # potential start of word
                flg=True
                j=i
        else:
            if not t[i].isalpha():
                # potential end of word
                flg=False
                # design decision: we ignore words of length < 4
                if i-j >= 4: 
                    words.append(t[j:i].casefold())
        # assumes some terminator like \n is present
    return words

# Generation of unsorted index entries for a given textfile

import math

def generateIndexEntries(filename,filecode,writer):
    numberOfLines = getNumberOfLines(filename)
    digits = int(math.log10(numberOfLines))+1
    padCtrl = '0' + str(digits)  # controls leading zero padding
    reader = BufferedInput(filename,0.2)
    currline = reader.readln()
    inlineNo = 1
    outlineNo = 0
    while currline != None:
        # process currline:
        words = getWords(currline)
        for w in words:
            writer.writeln(w+':'+filecode+format(inlineNo,padCtrl)+'\n')
        outlineNo += len(words)
        # next line:
        inlineNo += 1
        currline = reader.readln()
    reader.close()
    return outlineNo  # for testing

def generateAllIndexEntries(entryfile):
    global CorpusFiles
    writer = BufferedOutput(entryfile,0.7)
    outlines = 0
    for filecode in CorpusFiles:
        outlines += generateIndexEntries(CorpusFiles[filecode],filecode,writer)
    writer.flush()
    return outlines


# Provide the following:

import os

def splitIntoSortedChunks(entryfile):
    reader = BufferedInput(entryfile,0.3)
    blockNo = 0
    chunk = reader.readchunk()
    while chunk != []:
        chunk.sort()
        blockfile = open('temp_' + str(blockNo) + '_' + str(blockNo+1),'w',
                         encoding='utf-8')
        # output file written all at once, so no need for buffering
        blockfile.writelines(chunk)
        blockfile.close()
        blockNo += 1
        chunk = reader.readchunk()
    reader.close()
    return blockNo

# TODO
# Add your code here.

#Merges the two files temp_a_b and temp_b_c into a single sorted file temp_a_c
#@Param - three integers indicating files to be merged
def mergeFiles(a,b,c):
    firstFile = BufferedInput("temp_" + str(a) + "_" + str(b), 0.3)
    secondFile = BufferedInput("temp_" + str(b) + "_" + str(c), 0.3)
    outputFile = BufferedOutput("temp_" + str(a) + "_" + str(c), 0.3)

    firstFileLine = firstFile.readln()
    secondFileLine = secondFile.readln()

    while firstFileLine != None and secondFileLine != None:
        
        if (firstFileLine < secondFileLine):    #Sort by word and line number
            outputFile.writeln(firstFileLine)
            firstFileLine = firstFile.readln()
        else:
            outputFile.writeln(secondFileLine)
            secondFileLine = secondFile.readln()
            
    if (firstFileLine == None): #next line first file none? Then extend outputFile by secondFile
        extend(secondFileLine, secondFile, outputFile)
    elif (secondFileLine == None): #Is next line in the second file None? Then extend outputFile by firstFile
        extend(firstFileLine, firstFile, outputFile)

    firstFile.close()
    secondFile.close()
    os.remove("temp_" + str(b) + "_" + str(c))
    os.remove("temp_" + str(a) + "_" + str(b))
    outputFile.flush()

#Merges all chunk files within a certain range into a single sorted file temp_a_c
#@Param - two integers indicating the range of chunk files to be merged
#@Return - name of generated file
def mergeFilesInRange(a,c):

    b = (c+a)//2 #Get the middle point approx
    files = c - a #Number of files in range

    if (files != 1): #If there is only a single file do nothing
        if (files != 2): #If we have more than 2 files then compute recursively
            mergeFilesInRange(a, b)
            mergeFilesInRange(b, c)

        mergeFiles(a,b,c)

    return "temp_" + str(a) + "_" + str(c) #Investigate if it prints while processing

#Extends the outputfile by the provided inputFile
#@Param outputFile - file to be extended
#@Param inputFile - file to extend outputFile by
#@Param line - start line in inputFile (so that we don't miss any entries)
def extend(line, inputFile, outputFile):
    while line != None:
        outputFile.writeln(line)
        line = inputFile.readln()
        
# Putting it all together:

def sortRawEntries(entryfile):
    chunks = splitIntoSortedChunks(entryfile)
    outfile = mergeFilesInRange(0,chunks)
    return outfile

# Now compile the index file itself, by 'compressing' the entries for each key
# into a single line.

def createIndexFromEntries(entryfile,indexfile):
    reader = BufferedInput (entryfile,0.4)
    writer = BufferedOutput (indexfile,0.4)
    inl = reader.readln()
    currKey, currDoc, lineBuffer = '', '', ''
    while inl != None:
        # get keyword and ref, start ref list:
        colon = inl.index(':')
        key = inl[:colon]
        doc = inl[colon+1:colon+4] # three-letter doc identifiers
        j = colon+4
        while inl[j] == '0':
            j += 1
        line = inl[j:-1]
        if key != currKey:
            # new key: start a new line in index
            if lineBuffer != '':
                writer.writeln (lineBuffer+'\n')
            currKey = key
            currDoc = ''
            lineBuffer = key + ':'
        if currDoc == '':
            # first doc for this key entry
            currDoc = doc
            lineBuffer = lineBuffer + doc + line
        elif doc != currDoc:
            # new doc within this key entry
            currDoc = doc
            lineBuffer = lineBuffer + ',' + doc + line
        else:
            lineBuffer = lineBuffer + ',' + line
        inl = reader.readln()
    # write last line and clean up
    writer.writeln (lineBuffer+'\n')
    writer.flush()
    reader.close()

# Generating the meta-index to the index as a Python dictionary.

def generateMetaIndex(indexFile):
    global MetaIndex, MetaIndexOp
    MetaIndex.clear()
    reader = BufferedInput (indexFile,0.9)
    indexline = 1
    inl = reader.readln()
    while inl != None:
        key = inl[:inl.index(':')]
        MetaIndex[key] = indexline
        indexline += 1
        inl = reader.readln()
    reader.close()
    MetaIndexOp = (lambda s: MetaIndex[s])

def buildIndex():
    rawEntryFile = 'raw_entries'
    entries = generateAllIndexEntries (rawEntryFile)
    sortedEntryFile = sortRawEntries (rawEntryFile)
    global IndexFile
    createIndexFromEntries (sortedEntryFile, IndexFile)
    generateMetaIndex (IndexFile)
    os.remove(rawEntryFile)
    os.remove(sortedEntryFile)
    print('Success! ' + str(len(MetaIndex)) + ' keys, ' +
          str(entries) + ' entries.')

# Accessing the index using 'linecache' (random access to text files by line)

import linecache

def indexEntryFor(key):
    global IndexFile, MetaIndex, MetaIndexOp
    try:
        lineNo = MetaIndexOp(key)  # allows for other meta-indexing schemes
        indexLine = linecache.getline(IndexFile,lineNo)
    except KeyError:
        return None
    colon = indexLine.index(':')
    if indexLine[:colon] == key:
        return indexLine[colon+1:]
    else:
        raise Exception('Wrong key in index line.')

# End of file
