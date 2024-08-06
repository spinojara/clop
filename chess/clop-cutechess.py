#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Usage: clop-cutechess-cli.py CPU_ID SEED [PARAM_NAME PARAM_VALUE]...
Run cutechess-cli with CLOP_PARAM(s).

  CPU_ID	Symbolic name of the CPU or machine that should run the game
  SEED		Running number for the game to be played
  PARAM_NAME	Name of a parameter that's being optimized
  PARAM_VALUE	Integer value for parameter PARAM_NAME

CLOP is a black-box parameter tuning tool designed and written by Rémi Coulom.
More information about CLOP can be found at the CLOP website:
http://remi.coulom.free.fr/CLOP/

This script works between CLOP and cutechess-cli. The path to this script,
without any parameters, should be on the "Script" line of the .clop file.
'Replications' in the .clop file should be set to 2 so that this script can
alternate the engine's playing side correctly.

In this script the variables 'cutechess_cli_path', 'engine', 'engine_param_cmd',
'opponents' and 'options' must be modified to fit the test environment and
conditions. The default values are just examples.

When the game is completed the script writes the game outcome to its
standard output:
  W = win
  L = loss
  D = draw
"""

from subprocess import Popen, PIPE
import sys


# Path to the cutechess-cli executable.
# On Windows this should point to cutechess-cli.exe
cutechess_cli_path = 'cutechess-cli'

# The engine whose parameters will be optimized

# Format for the commands that are sent to the engine to
# set the parameter values. When the command is sent,
# {name} will be replaced with the parameter name and {value}
# with the parameter value.
# engine_param_cmd = 'setoption name {name} value {value}'

# A pool of opponents for the engine. The opponent will be
# chosen based on the seed sent by CLOP.

# Additional cutechess-cli options, eg. time control and opening book
options = '-each proto=uci tc={tc} -draw movenumber=80 movecount=5 score=5 -resign movecount=5 score=500 -openings file={book} order=sequential start={start} format=epd'

def main(argv = None):
    global options

    if argv is None:
        argv = sys.argv[1:]

    if len(argv) == 0 or argv[0] == '--help':
        sys.stdout.write(__doc__)
        return 0

    if len(argv) < 5 or len(argv) % 2 == 0:
        sys.stderr.write('Too few arguments\n')
        return 2

    engine = 'cmd={binary}'.format(binary=argv[0])
    opponents = [
        engine,
    ]


    clop_seed = 0
    try:
        clop_seed = int(argv[4])
    except ValueError:
        sys.stderr.write('invalid seed value: %s\n' % argv[4])
        return 2
    options = options.format(start=((clop_seed // 2) % 100000) + 1, book=argv[1], tc=argv[2])

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

    cutechess_args = '-engine %s -engine %s %s' % (fcp, scp, options)
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
