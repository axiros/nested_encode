from setuptools import setup
from setuptools import find_packages
from setuptools import Extension

mod = Extension(
    'nested_encode._convert_nested',
    ['nested_encode/_convert_nested.c'],
    extra_compile_args=['-O3']
)


setup(
    name='nested_encode',
    version='0.1',
    description="Convert unicode objects, in a nested structure, into uft-8 encoded bytes",
    author='Stephan Hofmockel',
    author_email="Use the github issues",
    url="https://github.com/axiros/nested_encode",
    license="BSD License",

    package_dir={"nested_encode": "nested_encode"},
    include_package_data = True,
    install_requires=['setuptools'],
    ext_modules=[mod],
    test_suite='nested_encode.tests',
    packages=find_packages('.'),
)
