@echo off

mkdir build 2>nul

rmdir /s /q "build/doxygen"
doxygen ../Doxyfile

rmdir /s /q "build/html"
sphinx-build -b html . build/html