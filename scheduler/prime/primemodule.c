#include <Python.h>
#include <math.h>
#include <sys/time.h>

#define number unsigned long
#define big unsigned long long int

// (a * b) % c
inline number mulmod(number a, number b, number c)
{
    big x = 0, y = a % c;

    while (b > 0)
    {
        if (b & 1)
        {
            x = (x + y) % c;
        }
        y = (y * 2) % c;
        b >>= 1;
    }
    return x % c;
}

// (a * a) % n
inline number sqrmod(number x, number n)
{
    return mulmod(x, x, n);
}

// (a ^ k) % c
inline number powmod(number a, number k, number c)
{
    big x = 1, y = a;

    while (k > 0)
    {
        if (k & 1)
        {
            x = mulmod(x, y, c);
        }
        y = sqrmod(y, c);
        k >>= 1;
    }
    return x % c;
}

static PyObject* prime_miller_rabin(PyObject *self, PyObject *args)
{
    number n, d;
    int i, rounds;

    if (!PyArg_ParseTuple(args, "l", &n))
    {
        return NULL;
    }

    if (!(n & 1) || n < 2)
    {
        return Py_BuildValue("i", n == 2);
    }

    d = n - 1;
    while (!(d & 1))
    {
        d >>= 1;
    }

    rounds = log2(n);
    for (i = 0; i < rounds; ++i)
    {
        number a = (rand() + 2) % n;
        number tmp = d;
        number mod = powmod(a, d, n);

        while (tmp != n - 1 && mod != 1 && mod != n - 1)
        {
            mod = sqrmod(mod, n);
            tmp <<= 1;
        }
        if (mod != n - 1 && !(tmp & 1))
        {
            return Py_BuildValue("i", 0);
        }
    }
    return Py_BuildValue("i", 1);
}

static PyObject* prime_trivial(PyObject *self, PyObject *args)
{
    number n;
    int i, bound;

    if (!PyArg_ParseTuple(args, "l", &n))
    {
        return NULL;
    }

    if (!(n & 1) || n < 2)
    {
        return Py_BuildValue("i", n == 2);
    }

    bound = sqrt(n) + 1;
    for (i = 2; i < bound; ++i)
    {
        if (n % i == 0)
        {
            return Py_BuildValue("i", 0);
        }
    }
    return Py_BuildValue("i", 1);
}

static PyMethodDef PrimeMethods[] = {
    { "miller_rabin", prime_miller_rabin, METH_VARARGS, "Miller-Rabin primality test." },
    { "trivial", prime_trivial, METH_VARARGS, "Trivial primality test." },
    { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initprime(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    srand(tv.tv_sec + tv.tv_usec);

    Py_InitModule("prime", PrimeMethods);
}
