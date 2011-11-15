from distutils.core import setup, Extension

module = Extension(
    'prime',
    sources=['primemodule.c', 'myrand.c'],
    extra_compile_args=['-std=c99', '-Wextra', '-pedantic', '-Wno-unused', '-O2'],
)

setup(name='Prime', ext_modules=[module])
