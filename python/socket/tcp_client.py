# make a send call and recieves the result
import sys
import socket

if len(sys.argv) == 1 :
    print("Need to specify a port")
    sys.exit(1)

server_port = int(sys.argv[1])
server_ip = "127.0.0.1"

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((server_ip, server_port))
client_socket.sendall(b"hi")
data = client_socket.recv(1024)

print(f"Data recieved from server at ${server_ip}:${server_port} - {data}")

client_socket.close()