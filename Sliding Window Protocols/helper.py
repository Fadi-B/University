""" Fadi Barazi s1969801 """

import sys
import time

from socket import *

""" This is the EOF indicator """
EOF : int = 1

""" This is the position of the EOF in the packet """
EOF_POS : int = 2

""" These are the start and end positions of the SEQ number in the packet """
SEQ_POS_START : int = 0
SEQ_POS_END : int = 1


"""
This function returns true if the received acknowledgement packet
indicates the that sent packet was received successfully
"""
def isACK(packet, sentSequence):

    """Compare sequence numbers"""
    if (int.from_bytes(packet, 'little') == sentSequence):

        return True

    else:

        return False

"""
This function encapsulates the provided data into a segment that
can be sent by adding the relevant header information
"""
def encapsulate(data, process, SEQ):

    """ Boolean process indicates if this is last data segment or not """    
    if (process):

        message = (0).to_bytes(1, byteorder='little') + data
            
    else:
            
        message = (EOF).to_bytes(1, byteorder='little') + data


    message = (SEQ).to_bytes(2, byteorder='little') + message

    return message

"""
Extracts the packet sequence from a packet
"""
def getPacketSEQ(packet):

    return int.from_bytes(packet[SEQ_POS_START:SEQ_POS_END + 1], 'little')

"""
Converts a packet sequence into a packet (i.e into bytes) 
"""
def toPacketSEQ(SEQ):
    return (SEQ).to_bytes(2, byteorder='little')

"""
This function extracts all the data in a file and stores
it into a buffer array with each entry being of size 1027
bytes (header + data)
"""
def extractData(fileName):

    buffer = []

    process = True

    SEQ = 0

    totalFileSize = 0

    with open(fileName, 'rb') as file:

        while process:
            data = file.read(1024)
            totalFileSize = totalFileSize + len(data)

            if (len(data) < 1024):

                process = False

            message = encapsulate(data, process, SEQ)
            buffer.append(message)

            SEQ = SEQ + 1

    return buffer, len(buffer), totalFileSize

