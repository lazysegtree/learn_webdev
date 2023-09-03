# Python 3 server example
# https://pythonbasics.org/webserver/
from http.server import BaseHTTPRequestHandler, HTTPServer
import time
import sys

if len(sys.argv) != 5 :
    print("Need to specify a port and error. Need exactly 5 arguements.")
    print("Sample command : `always.py -p 4000 -e 502` or `always.py -e 444 -p 3001`")
    sys.exit(1)

serverPort = -1
error = -1

idx = 1
while idx<5 :
    if sys.argv[idx] == "-p":
        serverPort = int(sys.argv[idx+1])
        idx += 2
    else :
        error = int(sys.argv[idx+1])
        idx += 2
int(sys.argv[1])
hostName = "localhost"

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_error(429)

if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))


    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")