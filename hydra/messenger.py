import socket
import time

from common import *
from protocol import *

class Messenger(object):
    def __init__(self, sock):
        self.sock = sock
        self.settimeout()
        self.chunks = []

    def _safe(self, func, *args):
        try:
            return func(*args)
        except socket.timeout:
            pass
        except socket.error, ex:
            handle_socket_error(ex)

    def getchunk(self):
        if self.chunks:
            chunk = self.chunks.pop(0)
        else:
            chunk = self._safe(self.sock.recv, CHUNK)
            if isinstance(chunk, basestring):
                chunk = chunk.strip('\n')
            if chunk:
                #print 'GET %s' % str(chunk).replace('\n', '\\n')
                messages = chunk.split('\n')
                if len(messages) > 1:
                    self.chunks.extend(messages[1:])
                chunk = messages[0]
        return chunk

    def wait(self, *messages):
        start = time.time()
        timeout = self.sock.gettimeout()
        chunk = self.getchunk()
        while chunk not in messages:
            if timeout and time.time() - start > timeout:
                raise SchedulerError('%s timed out' % self.name())
            chunk = self.getchunk()
        return chunk

    def _getbytes(self, count):
        res = []
        recieved = 0
        while recieved != count:
            chunk = self.getchunk()
            recieved += len(chunk)
            res.append(chunk)
        return ''.join(res)

    def _getint(self):
        res = self.getchunk()
        while not res.isdigit():
            res = self.getchunk()
        return int(res)

    def recv(self):
        size = self._getint()
        self.send(MSG_OK)
        data = self._getbytes(size)
        self.send(MSG_OK)
        return data

    def _inner_send(self, data):
        #print 'SEND %s' % str(data)
        left = 0
        for right in range(0, len(data), CHUNK):
            self._safe(self.sock.send, data[left:right])
            left = right
        if left < len(data):
            self._safe(self.sock.send, data[left:] + '\n')
        else:
            self._safe(self.sock.send, '\n')

    def send(self, data):
        if data in MESSAGES:
            self._inner_send(data)
        else:
            self._inner_send(str(len(data)))
            self.wait(MSG_OK)
            self._inner_send(data)
            self.wait(MSG_OK)

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

class ClientMessenger(Messenger): pass
