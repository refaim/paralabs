import sys
import os
import socket
import time
import pickle

from utils import console

import prime
import hwinfo
from common import *
from protocol import *

SERV_ADDR = None
PROGRESS_ATOM = 20

def main(args):
    global SERV_ADDR
    if not args:
        SERV_ADDR = DEFAULT_ADDR
    elif len(args) == 1:
        SERV_ADDR = parse_addr(args[0])
    else:
        print 'Usage: client.py <host:port>'
        return 1

    pbar = None
    conn = None
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(SERV_ADDR)

        conn = ClientMessenger(sock)
        conn.send(MSG_HELLO)
        conn.wait(MSG_HELLO)
        print 'Connected to %s:%d' % SERV_ADDR

        conn.wait(MSG_INFO)
        conn.send(pickle.dumps(hwinfo.collect()))

        while True:
            conn.wait(MSG_PREPARE)
            conn.send(MSG_OK)

            rawrange = conn.getchunk().split(NUM_SEPARATOR)
            conn.send(MSG_OK)
            left, right = tuple(map(int, rawrange))
            print 'Received %s' % rangestring((left, right))

            pbar = console.ProgressBar(right - left)
            step = (right - left) / PROGRESS_ATOM

            start = time.time()
            primes = []
            for i, x in enumerate(xrange(left, right)):
                if prime.miller_rabin(x):
                    primes.append(str(x))
                if i % step == 0:
                    pbar.set(i)
            pbar.finish()
            pbar.clear()

            print '%.3f seconds, %d prime numbers found' % (
                time.time() - start, len(primes)
            )

            conn.send(MSG_COMPLETED)
            conn.wait(MSG_OK)

            primes = ','.join(primes)
            conn.send(primes)

    except socket.error, ex:
        handle_socket_error(ex)
    finally:
        if conn:
            conn.send(MSG_BYE)
        if pbar:
            pbar.clear()

if __name__ == '__main__':
    try:
        sys.exit(main(sys.argv[1:]))
    except KeyboardInterrupt:
        print 'Interrupted by user'
    except SchedulerError, ex:
        print '%s: %s' % (os.path.basename(__file__), ex.args[0].lower())
    sys.exit(1)
