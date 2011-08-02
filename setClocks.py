#!/usr/bin/python
from __future__ import print_function
import pyadl
import os
import sys
import yaml
import re

def main(argv=None):
   argv = argv or sys.argv
   progDir = os.path.abspath(os.path.dirname(argv[0]))
   confFile = re.sub(r'^(.*?)(\.py)?$', r'\1.yml', os.path.basename(argv[0]))

   config = yaml.load(open(os.path.join(progDir, confFile)))

   # guess display if not set
   if 'DISPLAY' not in os.environ:
      os.putenv('DISPLAY', config.setdefault('DISPLAY', ':0'))
   # override if set in config
   elif 'DISPLAY' in config:
      os.putenv('DISPLAY', config['DISPLAY'])

   if config.setdefault('warn', True):
      raw_input(('WARNING: DO NOT run this script without first editing '
                'the overclocking configuration in "{0}". '
                'Press Ctrl-C to abort, enter to continue.').format(confFile))

   # TODO: detect model name and active adapters; use .yml config

   factors = [100, 100, 1000]
   gpuConfig = dict( (k, [int(v[i] * factor) for i, factor in enumerate(factors)]) for k, v in config['clocks'].items() )

   pyadl.ADL.Instance()
   if not config.setdefault('force', False):
      raw_input('Look above and ensure you\'re configuring the active cards! Then, press enter.')

   for gpuNdx, vals in gpuConfig.iteritems():
      adapter = pyadl.Adapter(gpuNdx)
      levels = adapter.getPerformanceLevels()
      [ 
         setattr(level, key, vals[valNdx])
            for level in levels
               for valNdx, key in enumerate([
                  'iEngineClock',
                  'iMemoryClock',
                  'iVddc'
               ]) if vals[valNdx]
      ]
      adapter.setPerformanceLevels(levels)

if __name__ == '__main__':
   sys.exit(main())
