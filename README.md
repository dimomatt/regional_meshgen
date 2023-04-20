# MPAS Regional Mesh Generation

[![ci](https://github.com/cpp-best-practices/gui_starter_template/actions/workflows/ci.yml/badge.svg)](https://github.com/cpp-best-practices/gui_starter_template/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/cpp-best-practices/gui_starter_template/branch/main/graph/badge.svg)](https://codecov.io/gh/cpp-best-practices/gui_starter_template)
[![Language grade: C++](https://img.shields.io/lgtm/grade/cpp/github/cpp-best-practices/gui_starter_template)](https://lgtm.com/projects/g/cpp-best-practices/gui_starter_template/context:cpp)
[![CodeQL](https://github.com/cpp-best-practices/gui_starter_template/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/cpp-best-practices/gui_starter_template/actions/workflows/codeql-analysis.yml)

## About

This repository provides tools that can be used to generate regional meshes for
MPAS-Atmosphere.

Initially, this will only be capable of producing quasi-uniform rectangular
meshes with a given resolution, but work is beign done to expand standard MPAS
mesh generation abilities to this tool.

This tool will:
1. Produce a series of generating points and the Delaunay triangulation

2. Generate mesh connectivity and geometry fields from the point set and
   triangulation
3. Write this file to a netCDF mesh file

## Building

## Dependencies


## Testing

See [Catch2 tutorial](https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md)

## Fuzz testing

See [libFuzzer Tutorial](https://github.com/google/fuzzing/blob/master/tutorial/libFuzzerTutorial.md)


