import sys
import platform
import subprocess
import shlex

if __name__ == '__main__':
    if platform.system() == 'Windows':
        make = 'mingw32-make -fMakefile.mingw'
    else:
        make = 'make'
    cmd = shlex.split(make) + sys.argv[1:]
    print ' '.join(cmd)
    subprocess.call(cmd, shell=True)
