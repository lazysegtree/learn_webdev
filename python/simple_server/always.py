# Python 3 server example
# https://pythonbasics.org/webserver/
from http.server import BaseHTTPRequestHandler, HTTPServer
import time
import sys

if len(sys.argv) != 5 and len(sys.argv) != 7 :
    print("Need to specify a port and error. Need exactly 5 or 7 arguements.")
    print("Sample command : `always.py -p 4000 -s 502` or `always.py -s 444 -p 3001` or always.py -s 444 -p 3001 -d 2`")
    sys.exit(1)

serverPort = -1
status = -1
delay = 0
idx = 1
while idx<len(sys.argv) :
    if sys.argv[idx] == "-p":
        serverPort = int(sys.argv[idx+1])
        idx += 2
    elif sys.argv[idx] == "-s" :
        status = int(sys.argv[idx+1])
        idx += 2
    elif sys.argv[idx] == "-d":
        delay = int(sys.argv[idx+1])
        idx += 2
    else:
        print("invalid args")
        sys.exit(3)

if (serverPort == -1 or status == -1) :
    print("Invalid args.")
    sys.exit(2);

hostName = "localhost"

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        time.sleep(delay)
        if status != 200 :
            self.send_error(status)
        else :
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