import sys
import os
import socket
import time
import json

from utils import console

import prime
import hwinfo
from messenger import ClientMessenger
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
        conn.send(json.dumps(hwinfo.collect()))

        while True:
            conn.wait(MSG_PREPARE)
            conn.send(MSG_OK)

            left, right = json.loads(conn.recv())
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

            conn.send(json.dumps(primes))

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
        print ex.args[0]
    sys.exit(1)
