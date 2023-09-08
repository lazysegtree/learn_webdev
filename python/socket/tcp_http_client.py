# make a send call and recieves the result
import sys
import socket
import time
if len(sys.argv) != 3 :
    print("Need to specify a port and no. of req. : example . `python3 tcp_http_client.py 8080 3`")
    sys.exit(1)

server_port = int(sys.argv[1])
server_ip = "127.0.0.1"

nreq = int(sys.argv[2])

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((server_ip, server_port))

for i in range(0, nreq):
    req1 = f"GET /tc HTTP/1.1\r\nHost:localhost:{server_port}\r\n\r\n"
    req2 = f"GET /404 HTTP/1.1\r\nHost:localhost:{server_port}\r\n\r\n"
    client_socket.sendall(req1.encode())
    #client_socket.sendall(req2.encode())
    data = client_socket.recv(4096)
    print(f"Data recieved from server at ${server_ip}:${server_port} - {data}")
    time.sleep(1)

client_socket.close()