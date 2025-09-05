#!/bin/bash
find . -name 'CMakeFiles' -type d -exec rm -rf {} +
find . -name 'CMakeCache.txt' -delete
find . -name 'cmake_install.cmake' -delete
find . -name 'Makefile' -delete
rm -rf build/ cmake-build-debug/