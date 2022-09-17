""" Fadi Barazi s1969801 """

import sys

from socket import *

from helper import *

EOF_LOST : int = -2

port = int(sys.argv[1])
fileName = sys.argv[2]

serverSocket = socket(AF_INET, SOCK_DGRAM)  #IPv4 and UDP protocols
serverSocket.bind(('', port))

""" Data buffer will buffer the data until
    it is fully received"""
dataBuffer = []
processed = False

""" The prevSEQ variable will hold the
    previous packet sequence number received, whereas
    the expSEQ variable will hold the expected packet """
expSEQ : int = 0
prevSEQ : int = -1

while not processed:
    message, clientAddress = serverSocket.recvfrom(2048) 

    packetSequence = getPacketSEQ(message)

    #print("Receiving {}".format(packetSequence))
    
    """ This indicates duplicate """
    if (packetSequence == prevSEQ):
        
        serverSocket.sendto(message[SEQ_POS_START:SEQ_POS_END + 1], clientAddress)

    elif (packetSequence == expSEQ):
        
        if (message[EOF_POS] == EOF):

            processed = True

            """ Probabilistic approach to ensure sender receives ack """
            for i in range(0,20):

                serverSocket.sendto(message[SEQ_POS_START:SEQ_POS_END + 1], clientAddress)

        else:

            """ Send Acknowledgement """
            serverSocket.sendto(message[SEQ_POS_START:SEQ_POS_END + 1], clientAddress)

        data = message[EOF_POS + 1:]
    
        dataBuffer.append(data)
        
        if (processed):
            #print(b"".join(dataBuffer).__sizeof__())
            with open(fileName, 'wb') as file:
            
                file.write(b"".join(dataBuffer))
        
            dataBuffer.clear()
            #processed = False
            expSEQ = 0
            prevSEQ = -1
            
        else:

            prevSEQ = expSEQ
            expSEQ = 1 + expSEQ

        
serverSocket.close()
