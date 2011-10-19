import sys
import os
import socket

import prime
from common import *

SERV_ADDR = None

def connect():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.connect(SERV_ADDR)
    except socket.error, ex:
        raise SchedulerError('[Error %d] %s' % ex.args)
    return sock


def main(args):
    global SERV_ADDR
    if not args:
        SERV_ADDR = DEFAULT_ADDR
    elif len(args) == 1:
        SERV_ADDR = parse_addr(args[0])
    else:
        print 'Usage: client.py <host:port>'
        return 1

    import time
    while True:
        sock = connect()
        sock.send('Here')

        bounds = tuple(int(x) for x in sock.recv(1024).split())
        print 'Recieved [%d, %d]' % bounds

        primes = [x for x in xrange(*bounds) if prime.trivial(x)]
        print '%d prime numbers found' % len(primes)

        sock.send(str(primes))
        print 'Result sent'

        time.sleep(5)

if __name__ == '__main__':
    try:
        sys.exit(main(sys.argv[1:]))
    except KeyboardInterrupt:
        print 'Interrupted by user'
    except SchedulerError, ex:
        print '%s: %s' % (os.path.basename(__file__), ex.args[0].lower())
    sys.exit(1)
