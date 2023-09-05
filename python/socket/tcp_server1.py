import socket 


import sys 

if len(sys.argv) == 1 :
    print("Need to specify a port")
    sys.exit(1)

server_port = int(sys.argv[1])
hostName = "localhost"
server_ip = "127.0.0.1"

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((server_ip, server_port))
server_socket.listen(5)

print(f"{server_ip}, {server_port}")

while True:
    client_socket, client_address = server_socket.accept()
    print(f"Accepted from {client_socket},{client_address}")
    data = client_socket.recv(10)

    print(f"data reciever : {data}")

    client_socket.close()