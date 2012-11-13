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

        def split_command(line, char):
            commands = []
            cmd = line
            amp = line.find(char)
            while amp != -1:
                cmd = line[:amp]
                line = line[amp + 1:]
                commands.append((cmd, True))
                amp = line.find(char)
                cmd = line
            if cmd:
                commands.append((cmd, False))
            return commands

        line = line.strip()
        for key, value in sorted(self.environ.iteritems()):
            line = line.replace('$' + key, value)

        pipes = split_command(line, '|')
        is_prev_piped = False
        for pidx, (rawcmd, is_piped) in enumerate(pipes):
            commands = split_command(rawcmd, '&')
            for cidx, (command, background) in enumerate(commands):
                if is_prev_piped and pidx > 0 and cidx == 0:
                    stdin = stdout
                else:
                    stdin = None
                if is_piped and cidx == len(commands) - 1:
                    background = True
                    stdout = subprocess.PIPE
                else:
                    stdout = None
                args = [arg for arg in command.split(' ') if arg.strip()]
                try:
                    process = subprocess.Popen(args, env=self.environ,
                        stdin=stdin, stdout=stdout)
                except OSError, ex:
                    print ex.strerror
                    continue
                stdout = process.stdout
                self.processes.append(process)
                self.lastcmd = ''
                if not background:
                    process.wait()
                    self.processes.pop()
                is_prev_piped = is_piped


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
