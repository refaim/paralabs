import os
import platform
import re
import subprocess
import shlex

import psutil


def mb(bytes):
    return int(bytes) / 1024 ** 2

def collect():
    return {
        'disk': _diskusage(),
        'memory': _memoryusage(),
        'cpu': _cpuspeed(),
    }

def _memoryusage():
    mem = psutil.phymem_usage()
    return {
        'total': mb(mem.total),
        'free': mb(mem.free),
    }

def _diskusage():
    disks = {}
    for partition in psutil.disk_partitions():
        info = psutil.disk_usage(partition.mountpoint)
        disks[partition.mountpoint] = {
            'total': mb(info.total),
            'free': mb(info.free),
        }
    return disks

def _cpuspeed():
    result = {}

    if platform.system() == 'Windows':

        import _winreg

        CPUKEY = 'HARDWARE\\DESCRIPTION\\System\\CentralProcessor'
        try:
            cpulist = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, CPUKEY)
        except WindowsError:
            return result

        for cpuid in range(psutil.NUM_CPUS):
            try:
                cpu = _winreg.OpenKey(cpulist, _winreg.EnumKey(cpulist, cpuid))
                freqmhz = _winreg.QueryValueEx(cpu, '~MHz')[0]
                result[cpuid] = float(freqmhz)
            except WindowsError:
                break
            finally:
                _winreg.CloseKey(cpu)

        _winreg.CloseKey(cpulist)

    LINUX_SYSCPU = '/sys/devices/system/cpu'
    if not result and os.access(LINUX_SYSCPU, os.R_OK):

        for cpuid in range(psutil.NUM_CPUS):
            freqpath = os.path.join(LINUX_SYSCPU, 'cpu%d' % cpuid, 'cpufreq',
                'cpuinfo_max_freq')
            if os.access(freqpath, os.R_OK):
                freqhz = open(freqpath).read()
                freqmhz = float(freqhz) / 1000.0
                result[int(cpuid)] = freqmhz

    MACOS_SYSPROFILER = '/usr/sbin/system_profiler'
    MACOS_SYSPROFILER_ARGS = '-detailLevel full SPHardwareDataType'
    if not result and os.access(MACOS_SYSPROFILER, os.X_OK):

        cmd = '%s %s' % (MACOS_SYSPROFILER, MACOS_SYSPROFILER_ARGS)
        profiler = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE)
        stdout, _ = profiler.communicate()
        for line in stdout.splitlines():
            match = re.search('Processor Speed: ([\d\.]+)', line)
            if match:
                freqghz = match.group(1)
                freqmhz = float(freqghz) * 1000.0
                for cpuid in range(psutil.NUM_CPUS):
                    result[cpuid] = freqmhz
                break

    return result
