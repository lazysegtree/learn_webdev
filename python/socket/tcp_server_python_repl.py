import socket
server_port = 8082
target_port = 8081
hostName = "localhost"
server_ip = "127.0.0.1"
target_ip = "127.0.0.1"

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((server_ip, server_port))
server_socket.listen(5)
server_socket.connect((target_ip, target_port))
server_socket.send(b"hi")