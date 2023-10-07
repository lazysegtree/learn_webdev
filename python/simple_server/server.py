from http.server import BaseHTTPRequestHandler, HTTPServer
import time
import argparse



parser = argparse.ArgumentParser()
parser.add_argument("-d", "--delay-msec", help="response delay in msec", 
                    default=100, type=int)
parser.add_argument("-s", "--status", help="Status code to be returned", 
                    default=200, type=int)
parser.add_argument("-p", "--port", help="Port on which server should run",
                    type=int, required=True)
args = parser.parse_args()

serverPort = args.port
status = args.status
delay = args.delay_msec / 1000
hostName = "localhost"
reqcount = 0

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        global reqcount
        reqcount += 1
        print(f"Server at {serverPort} with status = {status} got req no. {reqcount}")
        time.sleep(delay)
        self.send_response(status)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        if status == 200 : 
            self.wfile.write(bytes(f"Webserver at {serverPort} says Hello for req no. {reqcount}", "utf-8"))
        else :
            self.wfile.write(bytes(f"Error {status} from server at {serverPort} for req no. {reqcount}", "utf-8"))
        
if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print(  f"Server started http://{hostName}:{serverPort}." + \
            f"Will server response code {status}, with delay {delay}")

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")