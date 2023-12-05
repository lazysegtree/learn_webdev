import socket
import argparse
import struct
import time
import traceback

class Logger:
    @staticmethod
    def log(msg):

        print(f"[{time.asctime()}] {msg}")

parser = argparse.ArgumentParser()
parser.add_argument("-s", "--server", help="server IPv6 address", type=str)
parser.add_argument("-p", "--port", help="Port of server",
                    type=int, required=True)
parser.add_argument('--ip-opts',dest="ip_opts", help='send ip options as well', action='store_true',
                          default=False)
args = parser.parse_args()

Logger.log(f"args are {args}")

def set_ipv6_basic_opts(ipv6_socket):
    # Set IPv6 destination options (change as needed)
    hop_limit = 64  # Set the hop limit (TTL)
    flow_label = 0  # Set the flow label
    # Pack the destination options structure
    dest_options = struct.pack('!BBH', socket.IPV6_DSTOPTS, 0, hop_limit | (flow_label << 12))
    # Set the destination options on the socket
    ipv6_socket.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_DSTOPTS, dest_options)
    Logger.log("Done setting basic IPv6 opts")


def set_ipv6_opts(ipv6_socket: socket, sgw_ip):
    IP_FREEBIND=15
    ipv6_socket.setsockopt(socket.SOL_IP, IP_FREEBIND, 1)
    ipv6_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    ipv6_socket.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_RECVDSTOPTS, 1)
    mip6 = struct.pack('BBBBBBBB', 6, 2, 1, 2, 0, 0, 30, 16) + \
    socket.inet_pton(socket.AF_INET6, sgw_ip)

    Logger.log(f"Will set opt to {mip6}")
    ipv6_socket.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_DSTOPTS, mip6)
    Logger.log("Done setting IPv6 options")

def send_ipv6_request(host_addr, port):
    try:
        # Create an IPv6 socket
        ipv6_socket = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
        Logger.log("Created client socket")
        
        if(args.ip_opts):
            set_ipv6_opts(ipv6_socket, '2603:c010:0:9b01:b4dc:8d9b:8e85:a698')
            #set_ipv6_basic_opts(ipv6_socket)
        

        # Connect to the server
        ipv6_socket.connect((host_addr, port))

        # Send a sample HTTP GET request
        request = "GET / HTTP/1.1\r\nHost: [" + host_addr+ "]\r\n\r\n"
        request = request.encode()
        ipv6_socket.sendall(request)
        
        # Receive and print the response
        response = ipv6_socket.recv(1024)
        print("Response from server:\n", response.decode())

    except Exception as e:
        print("An error occurred:", e, traceback.format_exc())

    finally:
        # Close the socket
        ipv6_socket.close()

if __name__ == "__main__":
    send_ipv6_request(args.server, args.port)
