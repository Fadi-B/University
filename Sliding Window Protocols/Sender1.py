""" Fadi Barazi s1969801 """

import sys
import time

from socket import *
from helper import *

remoteHost : str
port : int
fileName : str

remoteHost = sys.argv[1]
port = int(sys.argv[2])
fileName = sys.argv[3]
    
clientSocket = socket(AF_INET, SOCK_DGRAM) #IPv4 and UDP protocols

process = True
SEQ = 0

with open(fileName, 'rb') as file: #rb ensures that file is treated as binary

    while process:

        data = file.read(1024)

        if (len(data) < 1024):

            process = False

        message = encapsulate(data, process, SEQ)

        """ Sleep to avoid packet loss """
        time.sleep(0.001)
        
        clientSocket.sendto(message, (remoteHost, port))
    
        """ Increment for next sequence number """
        SEQ = SEQ + 1

clientSocket.close()
