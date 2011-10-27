import copy

CHUNK = 4096
DEFAULT_TIMEOUT = 5.0 # seconds

NUM_SEPARATOR = ','
RANGE_FMT = '%d' + NUM_SEPARATOR + '%d'

MSG_HELLO = 'HI'
MSG_PREPARE = 'PREPARE'
MSG_INFO = 'INFO'
MSG_OK = 'OK'
MSG_COMPLETED = 'COMPLETED'
MSG_BYE = 'BYE'

MESSAGES = set(globals()[entry] for entry in copy.copy(globals())
    if entry.startswith('MSG_'))
