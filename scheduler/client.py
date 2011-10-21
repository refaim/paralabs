import sys
import os
import socket

from utils import console

import prime
from common import *

SERV_ADDR = None
PROGRESS_ATOM = 20

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

        left, right = tuple(int(x) for x in sock.recv(1024).split())
        print 'Recieved [%d, %d]' % (left, right)

        pbar = console.ProgressBar(right - left)
        step = (right - left) / PROGRESS_ATOM

        primes = []
        for i, x in enumerate(xrange(left, right)):
            if prime.miller_rabin(x):
                primes.append(x)
            if i % step == 0:
                pbar.set(i)
        pbar.finish()
        pbar.clear()

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
