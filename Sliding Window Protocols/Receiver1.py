""" Fadi Barazi s1969801 """

import sys

from socket import *

from helper import *

port : int
fileName : str

port = int(sys.argv[1])
fileName = sys.argv[2]

serverSocket = socket(AF_INET, SOCK_DGRAM)  #IPv4 and UDP protocols
serverSocket.bind(('', port))

dataBuffer = []
processed = False

while not processed:
    message, _ = serverSocket.recvfrom(2048) 

    if (message[EOF_POS] == EOF):
        processed = True

    """ The first 3 bytes are just header info """
    data = message[EOF_POS + 1:]
    
    dataBuffer.append(data)
    
    if (processed):

        with open(fileName, 'wb') as file:
            
            file.write(b"".join(dataBuffer))
        
        dataBuffer.clear()
        
serverSocket.close()
