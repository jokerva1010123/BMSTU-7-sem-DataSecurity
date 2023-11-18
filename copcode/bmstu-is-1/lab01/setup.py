from distutils.core import setup
from Cython.Build import cythonize
import os
import shutil

setup(
    ext_modules = cythonize(
        ["src/levenstein.py"],
        compiler_directives={'language_level' : "3"}
    )
)

os.remove("./src/levenstein.c")
shutil.rmtree("./build")
os.rename("./levenstein.cpython-310-x86_64-linux-gnu.so", "levenstein.so")
