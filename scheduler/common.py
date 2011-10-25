import re
import socket
import time

from protocol import *

DEFAULT_ADDR = ('localhost', 8080)

class SchedulerError(Exception): pass

def parse_addr(raw):
    match = re.match('([\w\.]+?):(\d+)', raw)
    if not match:
        raise SchedulerError('invalid server address')
    host, port = match.groups()
    port = int(port)
    return host, port

def handle_socket_error(ex):
    raise SchedulerError('[Error %d] %s' % ex.args)

def rangestring(range_):
    return '[%d, %d]' % range_

class Messenger(object):
    def __init__(self, sock):
        self.sock = sock
        self.settimeout()

    def _safe(self, func, *args):
        try:
            return func(*args)
        except socket.timeout:
            pass
        except socket.error, ex:
            handle_socket_error(ex)

    def getchunk(self):
        return self._safe(self.sock.recv, CHUNK)

    def wait(self, *messages):
        start = time.time()
        timeout = self.sock.gettimeout()
        chunk = self.getchunk()
        while chunk not in messages:
            if timeout and time.time() - start > timeout:
                raise SchedulerError('%s timed out' % self.name())
            chunk = self.getchunk()
        return chunk

    def send(self, message):
        left = 0
        for right in range(0, len(message), CHUNK):
            self._safe(self.sock.send, message[left:right])
            left = right
        if left < len(message):
            self._safe(self.sock.send, message[left:])

    def name(self):
        return '%s:%d' % self.sock.getpeername()

    def close(self):
        self.sock.shutdown(socket.SHUT_RDWR)
        self.sock.close()

    def settimeout(self):
        self.sock.settimeout(DEFAULT_TIMEOUT)

    def unsettimeout(self):
        self.sock.settimeout(None)

class ServerMessenger(Messenger):
    def wait(self, *messages):
        messages = list(messages) + [MSG_BYE]
        message = super(ServerMessenger, self).wait(*messages)
        if message == MSG_BYE:
            raise SchedulerError('disconnected')
        return message

    def getbytes(self, count):
        res = []
        recieved = 0
        while recieved != count:
            chunk = self.getchunk()
            if chunk is None:
                raise SchedulerError('%d %d' % (received, count))
            recieved += len(chunk)
            res.append(chunk)
        return ''.join(res)

    def waitint(self):
        res = self.getchunk()
        while not res.isdigit():
            res = self.getchunk()
        return int(res)

class ClientMessenger(Messenger): pass
