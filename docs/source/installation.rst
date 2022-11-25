.. highlight:: sh

Installation
============
The easiest way to get pyparadiseo is to install it via ``pip``

Install with ``pip``
--------------------
You can install pyParadiseo with `pip`

.. code-block:: sh

    pip install pyparadiseo

Build from source
-----------------
To build pyParadiseo, you'll need to have a few prerequisites installed on your system and set the corresponding paths in ``setup.py`` and ``CMakeLists.txt``

Prerequisites
~~~~~~~~~~~~~
To compile the binary extension you need:
    - cmake >= 3.14
    - python3 >= 3.6
    - boost-python
    - boost-numpy

Docker image
------------
Here is a ``manylinux2014_x86_64`` `Docker image`_ with installed prerequisites

.. _Docker image: https://hub.docker.com/repository/docker/jangmys/manylinux2014_boost180_cp36_310
