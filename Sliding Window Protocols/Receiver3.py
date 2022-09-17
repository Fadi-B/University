""" Fadi Barazi s1969801 """

import sys

from socket import *

from helper import *

EOF_LOST : int = -2

port = int(sys.argv[1])
fileName = sys.argv[2]

serverSocket = socket(AF_INET, SOCK_DGRAM)  #IPv4 and UDP protocols
serverSocket.bind(('', port))

""" Buffer for the data until fully received"""
dataBuffer = []
processed = False

""" The expSEQ variable will hold the expected packet """
expSEQ : int = 0


prevSEQ : int = -1

while not processed:
    message, clientAddress = serverSocket.recvfrom(2048) 

    packetSequence = getPacketSEQ(message)

    #print("Receiving {} expSEQ {}".format(packetSequence, expSEQ))

    if (packetSequence != expSEQ):

        #""" For the case when acknowledgement for EOF message lost """
        #if (message[EOF_POS] == EOF and prevSEQ == EOF_LOST):
        #    serverSocket.sendto(message[SEQ_POS_START:SEQ_POS_END + 1], clientAddress)

        if (expSEQ == 0):

            """ Not first packet? Then wait for retransmission """
            pass
        
        else:
            serverSocket.sendto(toPacketSEQ(expSEQ - 1), clientAddress)
        
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
            #prevSEQ = EOF_LOST
            
        else:
            expSEQ = 1 + expSEQ
            
serverSocket.close()
