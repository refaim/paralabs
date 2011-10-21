import sys
import os
import SocketServer

from common import *

class Dispatcher(object):
    def __init__(self):
        self.interval = 10 ** 6
        self.left = 1
        self.right = self.interval

    def get(self):
        bounds = self.left, self.right
        self.left = self.right
        self.right += self.interval
        return bounds

    def put(self, primes):
        pass


class RequestHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        request = self.request.recv(1024)
        self.request.send('%d %d' % dispatcher.get())
        dispatcher.put(self.request.recv(2 ** 16))


def main(args):
    if not args:
        host, port = DEFAULT_ADDR
    elif len(args) == 1:
        host, port = parse_addr(args[0])
    else:
        print 'Usage: server.py <host:port>'
        return 1

    print 'Serving on %s:%d' % (host, port)
    server = SocketServer.TCPServer((host, port), RequestHandler)
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
