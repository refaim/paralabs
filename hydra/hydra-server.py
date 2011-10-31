import sys
import os
import SocketServer
import threading
import json

from common import *
from protocol import *


def synchronized(lock):
    def wrap(func):
        def inner(*args, **kwargs):
            lock.acquire()
            try:
                return func(*args, **kwargs)
            finally:
                lock.release()
        return inner
    return wrap

dispatcher_lock = threading.Lock()

class Dispatcher(object):
    def __init__(self):
        self.interval = 10 ** 5
        self.left = 1
        self.right = self.interval

        self.clients = {}

        self.fout_name = 'primes.lst'
        if os.path.exists(self.fout_name):
            os.remove(self.fout_name)

    @synchronized(dispatcher_lock)
    def get(self):
        bounds = self.left, self.right
        self.left = self.right
        self.right += self.interval
        return bounds

    @synchronized(dispatcher_lock)
    def put(self, client, range_, primes):
        tsprint('%s %s => %d prime numbers' % (
            client, rangestring(range_), len(primes)
        ))
        with open(self.fout_name, 'a') as fout:
            fout.write('\n'.join(primes))

    @synchronized(dispatcher_lock)
    def register(self, conn, info):
        self.clients[conn.name()] = (conn, info)

    @synchronized(dispatcher_lock)
    def unregister(self, name):
        del self.clients[name]


class Server(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    def handle_error(self, request, client):
        request.shutdown(socket.SHUT_RDWR)
        request.close()

        client = '%s:%d' % client
        dispatcher.unregister(client)

        extype, ex, _ = sys.exc_info()
        if extype is SchedulerError:
            tsprint('%s %s' % (client, ex.args[0]))
        else:
            tsprint('%s disconnected' % client)
            raise


class RequestHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        conn = ServerMessenger(self.request)
        conn.wait(MSG_HELLO)
        conn.send(MSG_HELLO)
        tsprint('%s connected' % conn.name())

        conn.send(MSG_INFO)
        hwinfo = json.loads(conn.recv())
        dispatcher.register(conn, hwinfo)

        while True:
            conn.send(MSG_PREPARE)
            conn.wait(MSG_OK)

            range_ = dispatcher.get()
            tsprint('%s <= %s' % (conn.name(), rangestring(range_)))
            conn.send(json.dumps(range_))

            conn.unsettimeout()
            conn.wait(MSG_COMPLETED)
            conn.settimeout()
            conn.send(MSG_OK)

            numbers = conn.recv().split(NUM_SEPARATOR)
            dispatcher.put(conn.name(), range_, numbers)


def tsprint(message):
    stdout_lock.acquire()
    print message
    stdout_lock.release()

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
        stdout_lock = threading.Lock()
        sys.exit(main(sys.argv[1:]))
    except KeyboardInterrupt:
        print 'Interrupted by user'
    except SchedulerError, ex:
        print ex.args[0]
    sys.exit(1)
