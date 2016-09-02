import sys
from socket import *
from Queue import Queue
import random

def main(threadname,q):
    serverSocket = socket(AF_INET, SOCK_DGRAM)
    serverSocket.bind(('', 2390))

    while True:
        rand = random.randint(0, 10)
        message, address = serverSocket.recvfrom(1024)
        #message = message.upper()
        print message
        if q!=0:
            q.put(message)
        #if rand >= 4:
           #serverSocket.sendto(message, address) 
if __name__ == '__main__':
    sys.exit(main(0,0))