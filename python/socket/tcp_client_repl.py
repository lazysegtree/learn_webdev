import socket
server_port = 8082
target_port = 8081
hostName = "localhost"
server_ip = "127.0.0.1"
target_ip = "127.0.0.1"

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((target_ip, target_port))

client_socket.sendall(b"hi")
client_socket.recv(100)