import sys
import math
import prime

def test(left, right):
    numbers = xrange(left, right)
    print 'Processing [%d, %d]' % (left, right)

    rabin = set(n for n in numbers if prime.miller_rabin(n))
    primes = set(n for n in numbers if prime.trivial(n))
   
    errors = rabin - primes

    possible = len(rabin) - len(errors)
    print 'Miller-Rabin: found %d%% (%d of %d)' % (
        int(possible / (len(primes) * 0.01)),
        possible,
        len(primes)
    )
    if errors:
        print '%d false positives' % len(errors)    

if __name__ == '__main__':
    left, right = 1, 1000
    while right <= 10 ** 7:
        test(left, right)
        left = right
        right *= 10