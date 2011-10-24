import sys
import os
import SocketServer

from common import *
from protocol import *

RESULT_FILE = 'primes.txt'

class Dispatcher(object):
    def __init__(self):
        self.interval = 10 ** 5
        self.left = 1
        self.right = self.interval

        if os.path.exists(RESULT_FILE):
            os.remove(RESULT_FILE)

    def get(self):
        bounds = self.left, self.right
        self.left = self.right
        self.right += self.interval
        return bounds

    def put(self, client, range_, primes):
        print '%s %s => %d prime numbers' % (
            client, rangestring(range_), len(primes)
        )
        with open(RESULT_FILE, 'a') as fout:
            fout.write('\n'.join(primes))


class Server(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    def handle_error(self, request, client):
        self.shutdown_request(request)
        extype, ex, _ = sys.exc_info()
        if extype is SchedulerError:
            print ex.args[0]
        else:
            print '%s:%d disconnected' % client
            raise


class RequestHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        conn = Messenger(self.request)
        conn.wait(MSG_HELLO)
        conn.send(MSG_HELLO)
        print '%s connected' % conn.name()

        while True:
            conn.send(MSG_PREPARE)
            conn.wait(MSG_OK)

            range_ = dispatcher.get()
            print '%s <= %s' % (conn.name(), rangestring(range_))
            conn.send(RANGE_FMT % range_)
            conn.wait(MSG_OK)

            conn.unsettimeout()
            conn.wait(MSG_COMPLETED)
            conn.settimeout()
            conn.send(MSG_OK)

            ressize = conn.waitint()
            conn.send(MSG_OK)

            numbers = conn.getbytes(ressize).split(NUM_SEPARATOR)
            conn.send(MSG_OK)

            dispatcher.put(conn.name(), range_, numbers)


def main(args):
    if not args:
        host, port = DEFAULT_ADDR
    elif len(args) == 1:
        host, port = parse_addr(args[0])
    else:
        print 'Usage: server.py <host:port>'
        return 1

    print 'Serving on %s:%d' % (host, port)
    server = Server((host, port), RequestHandler)
    server.daemon_threads = True
    server.serve_forever()

if __name__ == '__main__':
    try:
        dispatcher = Dispatcher()
        sys.exit(main(sys.argv[1:]))
    except KeyboardInterrupt:
        print 'Interrupted by user'
    except SchedulerError, ex:
        print u'%s: error: %s' % (os.path.basename(__file__), ex.args[0].lower())
    sys.exit(1)
