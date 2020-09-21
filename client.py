import socket

HOST = '192.168.1.23'
PORT = 11000
BUFF_SIZE = 1024

csock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
csock.connect((HOST, PORT))

while True:
    msg = input('>>>')
    csock.send(msg.encode())
    rmsg = csock.recv(BUFF_SIZE).decode()
    print(rmsg)
    if rmsg.find('<EOF>') != -1:
        break

csock.close()

print('simple_client finito')

