""" Fadi Barazi s1969801 """

import sys
import time

from socket import *

from helper import *

remoteHost = sys.argv[1]
port = int(sys.argv[2])
fileName = sys.argv[3]
RetryTimeOut = float(sys.argv[4])

clientSocket = socket(AF_INET, SOCK_DGRAM) #IPv4 and UDP protocols
clientSocket.setblocking(False)

class timer():
    
    def __init__(self, timeOutInterval):

        self.timeOutInterval = (timeOutInterval / 1000)
        self.startTime = None
        
    def startTimer(self):

        self.startTime = time.time()
        #print("Starting Timer {}".format(self.startTime))

    def isTimeOut(self):

        timeOut = False

        if (self.isAlive()):
        
            now = time.time()
            timeOut = now > self.startTime + self.timeOutInterval

            #if timeOut: print("Timing out at {}".format(now))

        return timeOut

    def stopTimer(self):

        #print("Stopping Timer {}".format(time.time()))
        self.startTime = None

    def isAlive(self):

        if self.startTime:
            return True
        
        return False

def main():   

    process = True
    SEQ = -1

    retransmissions : int = 0
    totalFileSize : float = 0

    startTime : float = 0
    endTime : float = 0

    firstSegment = True

    packetTimer = timer(RetryTimeOut)

    with open(fileName, 'rb') as file:
        
        while process:
        
            SEQ = 1 + SEQ
        
            data = file.read(1024)
            totalFileSize = totalFileSize + len(data)

            if (len(data) < 1024):

                process = False

            """ Create and send the message """
            message = encapsulate(data, process, SEQ)        

            clientSocket.sendto(message, (remoteHost, port))
            #print("Sending {} ".format(SEQ))

            packetTimer.startTimer()
            
            startTime = time.time() if firstSegment else startTime
            firstSegment = False

            """ Process the acknowledgement packet """
            receivedACK = False
            
            while not receivedACK:

                if (packetTimer.isTimeOut()):

                    clientSocket.sendto(message, (remoteHost, port))
                    packetTimer.startTimer()
                    retransmissions = retransmissions + 1

                try:
                    
                    packet, _ = clientSocket.recvfrom(2048)

                    #print("Receiving {} Expecting {}".format(
                    #    getPacketSEQ(packet), SEQ))

                    """ If we receive a duplicate packet ack
                        then we want to ignore it """
                    if (isACK(packet, SEQ)):
                        packetTimer.stopTimer()

                        if not process:
                            
                            endTime = time.time()
                            
                        receivedACK = True
                    
                except error:

                    pass

    clientSocket.close()

    print("{} {:.2f}".format(retransmissions, (totalFileSize/1000)/(endTime - startTime)))


if __name__ == '__main__':
    main()
