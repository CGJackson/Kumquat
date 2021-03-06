from setuptools import setup,Extension
import setuptools.command
import os, sys, re, importlib

def check_valid_boost_path(path):
      '''
      Returns True if the path entered gives the location of the boost library, including,
      at a minimum, all files required by Compi
      '''

      include_regex = re.compile(r'#include <(boost/\S*)>') # matches a c++ include for a boost header file

      quad = "boost/math/quadrature/"
      files_required = {quad+"gauss_kronrod.hpp",
                        quad+"tanh_sinh.hpp",
                        quad+"sinh_sinh.hpp",
                        quad+"trapezoidal.hpp",
                        quad+"exp_sinh.hpp",
                        "boost/math/tools/precision.hpp"}


      # Recusively checks that a file exists, searches it for boost includes
      # and then checks those files

      files_checked = set()
      while files_required:
            f = files_required.pop()
            try:
                  for line in open(path+'/'+f,'r'):
                        include = include_regex.match(line)
                        if include is not None and include.group(1) not in files_checked:
                              files_required.add(include.group(1))
                        

            except FileNotFoundError:
                  return False
            files_checked.add(f)


      return True

src = 'source/'

compi_extension = Extension('compi',[src+f for f in ('compi.c',
                                            'GaussKronrod.cpp',
                                            'tanh_sinh.cpp',
                                            'sinh_sinh.cpp',
                                            'exp_sinh.cpp',
                                            'trapezoid.cpp',
                                            'IntegrandFunctionWrapper.cpp')],

                                       extra_compile_args=["-std=c++17"]
                            )

def add_boost_path_option(cmd):
    '''
    Adds boost_path as an option for a setup command
    '''
    class MyCommand(cmd):
        user_options = cmd.user_options + [
        ('boost-path=',None,"Path to C++ boost header files")
        ]
        def initialize_options(self):
            super().initialize_options()
            self.boost_path = None

        def finalize_options(self):
            super().finalize_options()
            if self.boost_path is not None and not check_valid_boost_path(self.boost_path):
                raise FileNotFoundError("Invalid boost path "+str(self.boost_path) + " entered")
            global compi_extension
            if not compi_extension.include_dirs.count(self.boost_path):
                compi_extension.include_dirs.append(self.boost_path)

    return MyCommand

# creates a dictionary of commands, generated by applying add_boost_path_option to each of the 
# standard setup commands
cmds = {cmd:add_boost_path_option(
                getattr(importlib.import_module("setuptools.command." + cmd),cmd))
            for cmd in setuptools.command.__all__}

def build_run(self):
    '''
    The run method for the build_ext command expecting a boost-path
    '''
    if compi_extension.include_dirs == []:
        raise FileNotFoundError("No boost path entered")
    super(type(self),self).run()

cmds['build_ext'].run = build_run

setup(name='Compi',
      version='1.0',
      author='Conor Jackson',
      author_email='conorgjackson@gmail.com',
      url='https://github.com/CGJackson/Compi',
      ext_modules=[compi_extension],
      cmdclass = cmds
)
