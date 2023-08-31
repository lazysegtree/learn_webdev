# Python 3 server example
# https://pythonbasics.org/webserver/
from http.server import BaseHTTPRequestHandler, HTTPServer
import time

import sys 

if len(sys.argv) == 1 :
    print("Need to specify a port")
    sys.exit(1)

serverPort = int(sys.argv[1])
hostName = "localhost"

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(bytes("Webserver says Hello.", "utf-8"))

if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))


    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")