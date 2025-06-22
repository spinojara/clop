#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from subprocess import Popen, PIPE
import sys


# Path to the cutechess-cli executable.
# On Windows this should point to cutechess-cli.exe
cutechess_cli_path = 'fastchess'

# The engine whose parameters will be optimized

# Format for the commands that are sent to the engine to
# set the parameter values. When the command is sent,
# {name} will be replaced with the parameter name and {value}
# with the parameter value.
# engine_param_cmd = 'setoption name {name} value {value}'

# A pool of opponents for the engine. The opponent will be
# chosen based on the seed sent by CLOP.

# Additional cutechess-cli options, eg. time control and opening book
options = '-use-affinity {cpu} -rounds 1 -games 1 -each proto=uci tc={tc} -draw movenumber=40 movecount=8 score=10 -resign movecount=3 score=800 twosided=true -openings file={book} order=sequential start={start} format=epd'

def tcadjust(tc):
    f = open('/etc/bitbit/tcfactor', 'r')
    tcfactor = float(f.read().strip())
    f.close()

    moves = 0
    maintime = 0
    increment = 0

    i = tc.find('/')
    if i != -1:
        moves = int(tc[:i])
        tc = tc[i + 1:]
    i = tc.find('+')
    if i != -1:
        maintime = float(tc[:i])
        increment = float(tc[i + 1:])
    else:
        maintime = tc

    tc = ''
    if moves > 0:
        tc += f'{moves}/'
    tc += f'{tcfactor * maintime}'
    if increment > 0:
        tc += f'+{tcfactor * increment}'

    return tc


def main(argv = None):
    global options

    if argv is None:
        argv = sys.argv[1:]

    if len(argv) < 5 or len(argv) % 2 == 0:
        sys.stderr.write('Too few arguments\n')
        return 2

    engine = 'cmd={binary}'.format(binary=argv[0])
    opponents = [
        engine,
    ]


    cpu = -1
    clop_seed = 0
    try:
        cpu = int(argv[3][7:])
    except ValueError:
        sys.stderr.write('invalid cpu value: %s\n' % argv[3])
        return 2
    try:
        clop_seed = int(argv[4])
    except ValueError:
        sys.stderr.write('invalid seed value: %s\n' % argv[4])
        return 2
    options = options.format(start=((clop_seed // 2) % 100000) + 1, book=argv[1], tc=tcadjust(argv[2]), cpu=cpu)

    fcp = engine
    scp = opponents[(clop_seed >> 1) % len(opponents)]

    # Parse the parameters that should be optimized
    for i in range(5, len(argv) - 1, 2):
        # Make sure the parameter value is numeric
        try:
            float(argv[i + 1])
        except ValueError:
            sys.stderr.write('invalid value for parameter %s: %s\n' % (argv[i], argv[i + 1]))
            return 2
        # Pass CLOP's parameters to the engine by using
        # cutechess-cli's initialization string feature
        fcp += ' option.%s=%s' % (argv[i], argv[i + 1])

    # Choose the engine's playing side (color) based on CLOP's seed
    if clop_seed % 2 != 0:
        fcp, scp = scp, fcp

    cutechess_args = '-engine %s name=0 -engine %s name=1 %s' % (fcp, scp, options)
    command = '%s %s' % (cutechess_cli_path, cutechess_args)

    # Run cutechess-cli and wait for it to finish
    process = Popen(command, shell = True, stdout = PIPE)
    output = process.communicate()[0]
    if process.returncode != 0:
        sys.stderr.write('failed to execute command: %s\n' % command)
        return 2

    # Convert Cutechess-cli's result into W/L/D
    # Note that only one game should be played
    result = -1
    for line in output.decode("utf-8").splitlines():
        if line.startswith('Finished game'):
            if line.find(": 1-0") != -1:
                result = clop_seed % 2
            elif line.find(": 0-1") != -1:
                result = (clop_seed % 2) ^ 1
            elif line.find(": 1/2-1/2") != -1:
                result = 2
            else:
                sys.stderr.write('the game did not terminate properly\n')
                return 2
            break

    if result == 0:
        sys.stdout.write('W\n')
    elif result == 1:
        sys.stdout.write('L\n')
    elif result == 2:
        sys.stdout.write('D\n')

if __name__ == "__main__":
    sys.exit(main())
