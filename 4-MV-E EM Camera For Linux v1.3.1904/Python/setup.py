from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
import os
import os.path
import numpy

pylon_dir=r'./MVGigE'

build_options = dict()
build_options['include_dirs'] = [pylon_dir]
build_options['library_dirs'] = [pylon_dir]
build_options['libraries'] = list(['MVGev'])
# Set build language
build_options['language'] = 'c++'

# Add numpy build options
build_options['include_dirs'].append(numpy.get_include())

setup(
  name = 'MVGigE',
  ext_modules=cythonize([
    Extension("MVGigE", ["MVGigE.pyx"], **build_options),
    ]),
)
