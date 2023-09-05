import socket 
import sys 
import random

if len(sys.argv) == 1 :
    print("Need to specify a port")
    sys.exit(1)

server_port = int(sys.argv[1])
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
    res = "Hello from " + str(server_num)
    client_socket.send(res.encode())

    client_socket.close()