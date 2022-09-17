""" Fadi Barazi s1969801 """

import sys
from threading import Thread, Lock
import time

from socket import *

from helper import *

remoteHost = sys.argv[1]
port = int(sys.argv[2])
fileName = sys.argv[3]
retryTimeOut = float(sys.argv[4])
windowSize = int(sys.argv[5])

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

        #print("Stopping Time {}".format(time.time()))
        self.startTime = None

    def isAlive(self):

        if self.startTime:
            return True
        
        return False

locker = Lock()

sendBase = 0
nxtSEQ = 0

packetTimer = timer(retryTimeOut)

STOP = False

startTime : float = 0
endTime : float = 0
        
def receiver(clientSocket, size):
    global locker
    global sendBase
    global nxtSEQ
    global packetTimer
    global STOP

    global endTime
    
    while not STOP:
       
       packet, _ = clientSocket.recvfrom(2048)

       acknowledgement = getPacketSEQ(packet)

       """ Ignore outdated acknowledgements """
       if (acknowledgement >= sendBase):

           with locker:

               #print("Receiving {} at {}".format(acknowledgement, time.time()))
               sendBase = 1 + acknowledgement

               if (sendBase == size):
                   endTime = time.time()
                   STOP = True
                   break

               """ Received all packets? Stop the timer """
               if (sendBase == nxtSEQ):
                   #print("Stopping timer at {}".format(time.time()))

                   packetTimer.stopTimer()
                   
               else:
                   
                   packetTimer.startTimer()
                   #print("ACK timer restart {}".format(time.time()))
                   

def sender(clientSocket):
    global sendBase
    global nxtSEQ
    global packetTimer
    global locker
    global STOP

    global startTime
    
    window = windowSize
    buffer, bufferSize, fileSize = extractData(fileName)
    firstSegment = True
    
    receiverSession = Thread(target=receiver, args=(clientSocket,bufferSize,))
    receiverSession.start()

    while not STOP:
        locker.acquire()

        window = min(windowSize, bufferSize - sendBase)

        if (packetTimer.isTimeOut()):
            
            nxtSEQ = sendBase

            #print("Resending {} onward ".format(nxtSEQ))
            
        while (nxtSEQ < sendBase + window):
            #print("NxtSeq is {} SenBase {} Window {}".format(nxtSEQ, sendBase, windowSize))

            clientSocket.sendto(buffer[nxtSEQ], (remoteHost, port))

            startTime = time.time() if firstSegment else startTime
            firstSegment = False
            
            """ Will be executed during timeout or all packets received """
            if (sendBase == nxtSEQ):

                packetTimer.startTimer()

            nxtSEQ = 1 + nxtSEQ
            

        locker.release()

    receiverSession.join()

    return fileSize
    
        
def main():
    global startTime
    global endTime

    clientSocket = socket(AF_INET, SOCK_DGRAM) #IPv4 and UDP protocols

    fileSize = sender(clientSocket)
    
    clientSocket.close()

    print("{:.2f}".format((fileSize/1000)/(endTime - startTime)))
    

if __name__ == '__main__':
    main()
