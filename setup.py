# -*- coding: utf-8 -*-
from __future__ import print_function

import sys

try:
    from skbuild import setup
except ImportError:
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

from setuptools import find_packages


if __name__ == "__main__":
    with open("README.md", "r", encoding="utf-8") as fh:
        long_description = fh.read()

    py_version = str(sys.version_info.major)+str(sys.version_info.minor)

    print("Setup for Python version: ",py_version)

    setup(
        # name="pyparadiseo",
        # version="0.1",
        author="Jan Gmys",
        author_email="jan.gmys@inria.fr",
        url="https://gitlab.inria.fr/paradiseo/pyparadiseo",
        # description="PyParadiseo - A software framework for heuristic optimization",
        long_description=long_description,
        long_description_content_type="text/markdown",
        license="CeCILL",
        packages=find_packages(where = 'src'),
        package_dir={"": "src"},
        cmake_install_dir="src/pyparadiseo",
        # include_package_data = True,
        package_data={'libboost_numpy'+py_version: ['/usr/local/lib/libboost_numpy'+py_version+'.so.1.80.0']},
        extras_require={"test": ["pytest"]},
        install_requires=[
            'numpy',
        ],
        python_requires=">=3.6"
    )
