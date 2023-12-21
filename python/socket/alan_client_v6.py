# Source : Alan Maguire

import argparse
import socket
import struct
import sys
import signal
import argparse

# Setting SOL_IP option IP_FREEBIND allows us to set arbibrary UDP source
# address.
IP_FREEBIND=15

IP_PKTINFO=8

def handle_arguments(name, description, usage, isclient):

    argparser = argparse.ArgumentParser(prog = name, description = description)

    argparser.add_argument('-p', dest='port', action='store',
                           help='port')
    argparser.add_argument('-q', dest='quiet_mode', action='store_true',
                           help='Silent operation')


    argparser.add_argument('-6', dest='v6', action='store_true',
                           help='use IPv6', default=True)
    
    argparser.add_argument('--ip-opts',dest="ip_opts", help='send ip options as well', action='store_true',
                          default=False)
    argparser.add_argument('-S', dest='sgw', action='store',
                           help='SGW IP address') 

    if isclient:
        argparser.add_argument('-C', dest='connect', action='store_true',
                           help='do connect')
        argparser.add_argument('-c', dest='client', action='store',
                           help='client IP address')
        argparser.add_argument('-P', dest='clientport', action='store',
                           help='client port to bind to')
        argparser.add_argument('-s', dest='server', action='store',
                       help='server IP address')
        argparser.add_argument('-i', dest='ifname', action='store',
                       help='local ifindex to send on')
    else:
        argparser.add_argument('-g', dest='getsockopt', action='store_true',
                            help='get,setsockopt')
    args = { }
    params = argparser.parse_args(sys.argv[1:])

    args['connect'] = False
    args['family'] = socket.AF_INET
    args['bindaddr'] = '0.0.0.0'
    args['clientaddr'] = args['bindaddr']
    args['clientport'] = 0
    args['localifindex'] = 0
    args['port'] = 7777
    args['quiet_mode'] = False
    args['getsockopt'] = False
    args['ip_opts'] = params.ip_opts

    if params.port:
        args['port'] = int(params.port)

    if params.quiet_mode:
        args['quiet_mode'] = True

    if params.v6:
        args['family'] = socket.AF_INET6
        args['bindaddr'] = '::'

    if params.sgw:
        args['sgw'] = params.sgw
        sgw=args['sgw']
    else:
        args['sgw'] = None 
    if isclient:
        if params.connect:
            args['connect'] = True
        if params.clientport:
            args['clientport'] = int(params.clientport)
        if params.server:
            args['serveraddr'] = params.server
        else:
            print(usage)
            exit(1)
        if params.client:
            args['clientaddr'] = params.client
        else:
            args['clientaddr'] = args['bindaddr']
        if params.ifname:
            args['localifindex'] = socket.if_nametoindex(params.ifname)
    else:
        if params.getsockopt:
            args['getsockopt'] = True
    # print(f"args are {args}")
    return args

def next_proto(sock):
    if sock.type == socket.SOCK_STREAM:
        return 6
    else:
        return 17

def update_sock_options(sock, args):
    opts = sock.getsockopt(socket.IPPROTO_IPV6, socket.IPV6_DSTOPTS, 24)
    if len(opts) == 24:
       print('got expected socket option len 24, setting options...')
       sock.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_DSTOPTS, opts)
    else:
       print('getsockopt() got option len %d' % len(opts))
       sock.close()

def set_sock_options(sock, args):
    # FREEBIND lets us to bind to fd04::/14 addr; SO_REUSEADDR avoids TIME_WAIT
    # errors between tests
    sock.setsockopt(socket.SOL_IP, IP_FREEBIND, 1)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    if args['sgw'] != None:
       if sock.family == socket.AF_INET:
         # this is equivalent to
         # Options: (20 bytes), Record Route
         # IP Option - Record Route (19 bytes)
         #     Type: 7
         #         0... .... = Copy on fragmentation: No
         #         .00. .... = Class: Control (0)
         #         ...0 0111 = Number: Record route (7)
         #     Length: 19
         #     Pointer: 20
         #     Recorded Route: 0.0.0.0
         #     Recorded Route: 0.0.210.209
         #     Recorded Route: 140.144.0.2
         #     Recorded Route: 172.24.255.160
         # IP Option - End of Options List (EOL)
         rrstruct = struct.pack('BBB', 7, 19, 20) + \
                  socket.inet_aton('0.0.0.0') + \
                  socket.inet_aton('0.0.210.209') + \
                  socket.inet_aton('140.144.0.2') + \
                  socket.inet_aton(args['sgw']) + \
                  struct.pack('B', 0)
         sock.setsockopt(socket.IPPROTO_IP, socket.IP_OPTIONS, rrstruct)
       else:
         # nexthdr TCP (6), optlen (2), TLV padN (4 bytes padding)
         # TLV type EXPERIMENTAL (0x1e), len 16
         sock.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_RECVDSTOPTS, 1)
         mip6 = struct.pack('BBBBBBBB', 6, 2, 1, 2, 0, 0, 30, 16) + \
                socket.inet_pton(socket.AF_INET6, args['sgw'])
         print(f"IPv6 Will set opt to {mip6}")
         sock.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_DSTOPTS, mip6)
    print("Done setting IP options")
         

def do_recvmsg(sock, size):
    data, ancillary, flags, peer = sock.recvmsg(size, size)
    local = sock.getsockname()
    localifindex = 0
    return data.decode('ascii'), peer, local, localifindex

def do_sendmsg(sock, msg, peer, sgw):
    local = sock.getsockname()

    srcaddr = socket.inet_pton(sock.family, local[0])
    #print(f"srcaddr : {srcaddr}")
    try:
        sock.sendmsg([bytes(msg, 'ascii')], [], 0, peer)
    except OSError as e:
        print('Could not send from %s: %s, reverting to sendto()' % (local[0], format(e)))
        sock.sendto(bytes(msg, 'ascii'), peer)

def do_recv(sock, size):
    if sock.family == socket.AF_INET6:
        sock.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_RECVDSTOPTS, 1)
    data, peer = sock.recvfrom(size)
    return data.decode('ascii')

def do_send(sock,msg):
    sock.send(bytes(msg,'ascii'))

def get_args():
    return handle_arguments(
              'stream_echoclient',
              'echoclient used to send requests/receive responses from server',
              '%s [-p port] -s [server IP address] [-q]' % sys.argv[0],
              True)
    

def main():
    args = get_args()
    sock = socket.socket(args['family'], socket.SOCK_STREAM)

    if(args['ip_opts']):
        set_sock_options(sock, args)

    bind_address = ( args['clientaddr'], args['clientport'])
    server_address = ( args['serveraddr'], args['port'])

    sock.bind(bind_address)

    sock.connect(server_address)
            
    peer = sock.getpeername()
    msg = "GET / HTTP/1.1\r\nHost: [" + server_address[0] + "]\r\n\r\n"
    print(f"Sending request \n------------\n{msg}\n----------")
    do_sendmsg(sock, msg, peer, args['sgw'])
    msg =  do_recvmsg(sock, 1024)
    if msg:
        print(f"server_addr : {server_address[0]}, peer : {peer[0]}\n response : \n------------\n{msg}\n----------")
    else:
         if args['quiet_mode'] == False:
             print(peer , ' : connection closed')

main()