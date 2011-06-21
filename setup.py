#!/usr/bin/env python
 
from distutils.core import setup
from distutils.extension import Extension
 
setup(name="pyadl",
    ext_modules=[
        Extension(
          "pyadl", ["pyadl.cpp"],
          define_macros=[("LINUX",1)], #fixes adl_sdk.h
          include_dirs=['ADL_SDK/include'],
          libraries = ["boost_python"],
        )
    ])
