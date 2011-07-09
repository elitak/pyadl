#!/usr/bin/python
from __future__ import print_function
import pyadl
import os
import sys

def main(argv=None):
   argv = argv or sys.argv

   # guess the display, if unset 
   if 'DISPLAY' not in os.environ:
      os.putenv('DISPLAY', ':0')

   raw_input('WARNING: DO NOT run this script without first editing '
             'the overclocking configuration contained within it. '
             'Press Ctrl-C to abort, enter to continue.')

   # TODO: detect model name and active adapters; use .yml config

   config = {
      # cardNdx: [EngineClock, MemClock, Vddc]
      # The unit used for clock values here is hundredths of MHz.
      # Zero-values are skipped rather than set.
      0: [950,100,0],
      4: [920,100,0],
      8: [920,100,0],
   }
   factors = [100, 100, 1000]
   config = dict( (k, [int(v[i] * factor) for i, factor in enumerate(factors)]) for k, v in config.items() )

   pyadl.ADL.Instance()
   raw_input('Look above and ensure you\'re configuring the active cards! Then, press enter.')

   for gpuNdx, vals in config.iteritems():
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
