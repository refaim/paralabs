from distutils.core import setup, Extension

module = Extension('prime', sources=['primemodule.c'])
setup(name='Prime', ext_modules=[module])
