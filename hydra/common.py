import re

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
