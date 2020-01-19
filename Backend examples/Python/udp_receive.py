from socket import *

serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', 2390))

while True:
    message, address = serverSocket.recvfrom(1024)
    message = message.upper()
    print (message)