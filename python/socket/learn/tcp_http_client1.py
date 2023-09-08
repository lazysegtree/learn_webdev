import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(("127.0.0.1", 8080))
sock.send(b"GET / HTTP/1.1\r\nHost:localhost:8080\r\n\r\n")
response = sock.recv(4096)
sock.close()
print(response.decode())