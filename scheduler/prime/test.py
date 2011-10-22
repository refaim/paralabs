import sys
import os
import math

parentdir = os.path.normpath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(parentdir)
import prime

def test(left, right):
    numbers = xrange(left, right)
    print 'Processing [%d, %d]' % (left, right)

    rabin = set(n for n in numbers if prime.miller_rabin(n))
    primes = set(n for n in numbers if prime.trivial(n))

    errors = rabin - primes

    possible = len(rabin) - len(errors)
    if len(primes) == possible:
        percentage = 100
    else:
        percentage = int(possible / (len(primes) * 0.01))

    print 'Miller-Rabin: found %d%% (%d of %d)' % (
        percentage,
        possible,
        len(primes)
    )
    if errors:
        print '%d false positives' % len(errors)

if __name__ == '__main__':
    try:
        left, right = 1, 1000
        while right <= 10 ** 7:
            test(left, right)
            left = right
            right *= 10
    except KeyboardInterrupt:
        print 'Interrupted by user'
