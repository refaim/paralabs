#!/usr/bin/env python
import cmd
import os
import subprocess
import sys


class UselessShell(cmd.Cmd):
    def __init__(self):
        cmd.Cmd.__init__(self)
        self.prompt = '$ '
        self.environ = os.environ
        self.processes = []

    def terminate_children(self):
        for process in self.processes:
            process.terminate()

    def cmdloop(self):
        try:
            cmd.Cmd.cmdloop(self)
        except KeyboardInterrupt:
            self.terminate_children()
            print ''
            sys.exit(0)

    def do_exit(self, args):
        self.terminate_children()
        sys.exit(0)

    def do_env(self, args):
        if not args:
            for key, value in sorted(self.environ.iteritems()):
                print '%s=%s' % (key, value)
        else:
            pairs = [pair for pair in args.split(' ') if pair.strip()]
            for pair in pairs:
                try:
                    key, value = pair.split('=')
                except ValueError:
                    print 'Invalid syntax'
                    return
                self.environ[key] = value

    def default(self, line):
        def environ_replacer(match):
            return self.environ.get(match.groups()[0], '')

        line = line.strip()
        for key, value in sorted(self.environ.iteritems()):
            line = line.replace('$' + key, value)

        commands = []
        cmd = line
        amp = line.find('&')
        while amp != -1:
            cmd = line[:amp]
            line = line[amp + 1:]
            commands.append((cmd, True))
            amp = line.find('&')
            cmd = line
        if cmd:
            commands.append((cmd, False))

        for command, background in commands:
            args = [arg for arg in command.split(' ') if arg.strip()]
            try:
                process = subprocess.Popen(args, env=self.environ)
                self.processes.append(process)
                self.lastcmd = ''
                if not background:
                    process.wait()
                    self.processes.pop()
            except OSError, ex:
                print ex.strerror


def main(args):
    shell = UselessShell()

    if args and os.path.isfile(args[0]):
        with open(args[0]) as script:
            for line in script:
                shell.onecmd(line)
    else:
        shell.cmdloop()


if __name__ == '__main__':
    main(sys.argv[1:])
