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
    
    def __init__(self, timeOutInterval, buffer, socket, dest, locker, fileSize):

        self.stop = False
        self.packetTimers = []
        self.socket = socket
        self.dest = dest
        self.locker = locker
        self.timeOutInterval = timeOutInterval
        self.buffer = buffer
        
    def start(self):

        session = Thread(target=self.runTimer, args=())
        session.start()

    def runTimer(self):

        while not self.stop:

            self.locker.acquire()

            if (self.size() > 0):
                
                if (time.time() > self.packetTimers[0][1]):

                    #print("Current Time Timer {}".format(time.time()))

                    packet = self.packetTimers[0][0]

                    self.removePacketExpiry(packet)

                    #print("Resending {}".format(packet))

                    self.socket.sendto(self.buffer[packet], self.dest)


                    self.addPacketExpiry(packet,
                                    time.time() + (self.timeOutInterval/1000))
                        
                        
            self.locker.release()
                
    def terminate(self):

        self.stop = True

    def addPacketExpiry(self, packetNumber, expiryTime):
            
        """ Ensure that latest expiry times at end of list"""
        self.packetTimers.append((packetNumber, expiryTime))

    def removePacketExpiry(self, packetNumber):

        success = False
        
        """ When removing we will still preserve that the
            latest expiry time is at the end of list """
        
        for i in range(0, len(self.packetTimers)):
                            
            if (self.packetTimers[i][0] == packetNumber):
                self.packetTimers.remove(self.packetTimers[i])
                success = True
                break

        return success

    def size(self):

        return len(self.packetTimers)

locker = Lock()

sendBase = 0
nxtSEQ = 0

buffer, bufferSize, fileSize = extractData(fileName)

clientSocket = socket(AF_INET, SOCK_DGRAM) #IPv4 and UDP protocols

packetTimer = timer(retryTimeOut, buffer, clientSocket, (remoteHost, port),
                    locker, fileSize)

STOP = False

startTime : float = 0
endTime : float = 0
        
def receiver(clientSocket, size):
    global locker
    global sendBase
    global nxtSEQ
    global packetTimer
    global STOP
    global receivedACK
    
    global endTime

    count = 0
    
    while not STOP:
       
       packet, _ = clientSocket.recvfrom(2048)

       acknowledgement = getPacketSEQ(packet)

       """ Ignore outdated acknowledgements """
       if (acknowledgement >= sendBase):

           with locker:

               #print("Receiving {}".format(acknowledgement))

               """ Mark the packet as being received by deleting
                   its expiry time """
               packetTimer.removePacketExpiry(acknowledgement)

               if (acknowledgement == sendBase):

                   if (packetTimer.size() > 0):

                       unacknowledgedPackets = [x[0] for x in packetTimer.packetTimers]
                       sendBase = min(unacknowledgedPackets)

                   else:

                        """ Entire window acknowledged """
                        sendBase = nxtSEQ

               if (acknowledgement == size - 1):
                   #print(acknowledgement)                   
                   count = count + 1
                   #print(count)

               if (sendBase == size or count > 1):
                   endTime = time.time()
                   packetTimer.packetTimers.clear()
                   STOP = True

               #print("Packet Timers are {}".format(packetTimer.packetTimers))
               #print("Current Time Receiver {}".format(time.time()))
               #print("New sendBase is {}".format(sendBase))


def sender(clientSocket):
    global sendBase
    global nxtSEQ
    global packetTimer
    global locker
    global STOP
    global buffer
    global bufferSize
    
    global startTime
    
    window = windowSize

    
    receiverSession = Thread(target=receiver, args=(clientSocket,bufferSize,))
    receiverSession.start()

    packetTimer.start()

    while not STOP:
        locker.acquire()
        
        window = min(windowSize, bufferSize - sendBase)
           
        while (nxtSEQ < sendBase + window):
            
            clientSocket.sendto(buffer[nxtSEQ], (remoteHost, port))

            startTime = time.time() if nxtSEQ == 0 else startTime

            packetTimer.addPacketExpiry(nxtSEQ,
                                        time.time() + (retryTimeOut/1000))

            #print("NxtSeq is {} SenBase {} Window {} Time {}".format(nxtSEQ,
            #                                                         sendBase,
            #                                                         windowSize,
            #                                                         time.time() + retryTimeOut/1000))

            nxtSEQ = 1 + nxtSEQ

        locker.release()

    packetTimer.terminate()
    receiverSession.join()
    
        
def main():
    global startTime
    global endTime

    sender(clientSocket)
    
    clientSocket.close()

    print("{:.2f}".format((fileSize/1000)/(endTime - startTime)))
    

if __name__ == '__main__':
    main()
