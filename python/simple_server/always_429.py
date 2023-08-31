# Python 3 server example
from http.server import BaseHTTPRequestHandler, HTTPServer
import time
import os
if len(sys.argv) == 1 :
    print("Need to specify a port")
    sys.exit(1)

serverPort = int(sys.argv[1])
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