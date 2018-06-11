from distutils.core import setup, Extension
#from distutils.extension import Extension

wrapper = Extension(
    'wrapper',
    sources=['testNEST.cpp','TestSpectra.cpp','NEST.cpp','VDetector.cpp','RandomGen.cpp'],
                    extra_compile_args = ['-O2', '-std=c++11', '-stdlib=libc++', '-mmacosx-version-min=10.7'],
                    libraries=['boost_python3']#,
)

setup(
    name='wrapper',
    version='0.1',
    ext_modules=[wrapper])
