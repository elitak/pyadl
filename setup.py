#!/usr/bin/env python
 
from distutils.core import setup
from distutils.extension import Extension
 
setup(
   name = 'pyadl',
   ext_modules = [
      Extension(
         name = 'pyadl',
         sources = [
            'pyadl.cpp',
            'amdovdrvctrl/src/adl.cpp'
         ],
         define_macros = [
            ('LINUX', 1), # fixes adl_sdk.h
            # This doesn't override the unconditional define in amdovdrvctrl/src/adl.h
            #('INF_LOG(x)', ''), # stops spam to console
         ],
         include_dirs = [
            'amd-adl-sdk/include',
            'amdovdrvctrl/src',
         ],
         libraries = ['boost_python'],
      )
   ]
)
