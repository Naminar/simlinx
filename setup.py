from setuptools import setup, find_packages

setup(
    name='generate',
    version='0.1.0',
    packages=find_packages(),
    # packages=find_packages(where='generate'),  # Find packages in the src directory
    # package_dir={'': 'generate'},             # Tell setuptools where the root package is
)
