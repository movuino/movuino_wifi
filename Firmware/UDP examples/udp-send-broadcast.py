import socket
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, True)
message = b"S"
s.sendto(message, ('<broadcast>', 2389))
s.settimeout(1)
try:
    print ('Response')
except socket.timeout:
    print ("No server found")

s.close()