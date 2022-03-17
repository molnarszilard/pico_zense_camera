#!/usr/bin/env python

from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

setup_args = generate_distutils_setup(
    packages=['pico_zense_camera'],
    package_dir={'': 'src'},
    requires=[]
)

setup(**setup_args)


