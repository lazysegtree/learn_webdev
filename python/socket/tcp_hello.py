import socket 
import sys 
import random
import time

if len(sys.argv) != 5 :
    print("Need to specify a port and delay. Example `python3 server.py -p 8080 -d 1`")
    sys.exit(1)

server_port = -1
delay = 0
idx = 1

while idx<len(sys.argv) :
    if sys.argv[idx] == "-p":
        server_port = int(sys.argv[idx+1])
        idx += 2
    elif sys.argv[idx] == "-d":
        delay = float(sys.argv[idx+1])
        idx += 2
    else:
        print("invalid args")
        sys.exit(3)

if (server_port == -1) :
    print("Invalid args.")

server_ip = "127.0.0.1"
server_addr = (server_ip, server_port)

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(server_addr)
server_socket.listen()

print(f"{server_ip}, {server_port}")

server_num = random.randint(0,1000)

while True:
    client_socket, client_address = server_socket.accept()
    print(f"Accepted from {client_socket},{client_address}")
    data = client_socket.recv(10)
    print(f"Data recieved : {data}")

    time.sleep(delay);

    res = "Hello from " + str(server_num)
    client_socket.send(res.encode())

    client_socket.close()