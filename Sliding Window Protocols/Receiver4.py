""" Fadi Barazi s1969801 """

import sys

from socket import *

from helper import *

import bisect

EOF_LOST : int = -2

port = int(sys.argv[1])
fileName = sys.argv[2]
window = int(sys.argv[3])

serverSocket = socket(AF_INET, SOCK_DGRAM)  #IPv4 and UDP protocols
serverSocket.bind(('', port))

""" Data buffer will buffer the data until
    it is fully received"""
dataBuffer = []
EOF_DETECTED = False

""" rcvBase is desired insequence packet """
rcvBase : int = 0

rcvBuffer = []

while not EOF_DETECTED:
    
    message, clientAddress = serverSocket.recvfrom(2048) 

    packetSequence = getPacketSEQ(message)

    #print("RcvBase: {} Receiving:{} Buffer: {}".format(rcvBase, packetSequence,
    #                                            [x[0] for x in rcvBuffer]))
    
    """ Packet received is within the receiver window """
    if (packetSequence >= rcvBase and packetSequence < rcvBase + window):

        """ Send individual acknowledgement """
        serverSocket.sendto(message[SEQ_POS_START:SEQ_POS_END + 1], clientAddress)

        """ Buffer the packet if has not been received before """
        if (not (packetSequence, message) in rcvBuffer):
            
            bisect.insort(rcvBuffer, (packetSequence, message))

        """ Has the expected sequence been obtained?
            Deliver all in-order packets"""
        if (packetSequence == rcvBase):

            prev = rcvBase - 1
            delivered = []
            counter = 0
            
            for i in range(0, len(rcvBuffer)):

                SEQ = rcvBuffer[i][0]
                data = rcvBuffer[i][1]

                if (SEQ == prev + 1):
                    
                    delivered.insert(0, rcvBuffer[i])

                    if (data[EOF_POS] == EOF):
                        
                        EOF_DETECTED = True

                        """ Probabilistic approach to ensure sender receives ack """
                        for i in range(0,20):

                            serverSocket.sendto(data[SEQ_POS_START:SEQ_POS_END + 1]
                                                , clientAddress)
                        
                    dataBuffer.append(data[EOF_POS + 1:])
                    prev = SEQ
                    counter = counter + 1
                    
                else:
                    break

            for i in range(0, len(delivered)):

                rcvBuffer.remove(delivered[i])

            if (EOF_DETECTED):
                
                #print(b"".join(dataBuffer).__sizeof__())
                with open(fileName, 'wb') as file:
            
                    file.write(b"".join(dataBuffer))
        
                dataBuffer.clear()
                rcvBuffer.clear()
                #EOF_DETECTED = False
                rcvBase = 0
            
            else:

                """ Update the receiver window """
                rcvBase = rcvBase + counter
                

    elif (packetSequence < rcvBase):

        """ Ensure that sender knows that this packet has already been received """
        serverSocket.sendto(message[SEQ_POS_START:SEQ_POS_END + 1], clientAddress)

    else:

        """ Do nothing """
        pass
        
        
            
serverSocket.close()
