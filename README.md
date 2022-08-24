# Raspberry PI 4 GPU Bare Metal Examples

This repository contains examples of code using the Raspberry PI 4 GPU (V3D) on a bare-metal environment.

Most of the interesting code (related to the GPU) lies in `src/kernel/examples/`, the rest is mostly runtime and utilities.
Currently, the code contains a single example that demonstrates how to render a triangle using the GL API commands provided by the GPU.

### Examples

- `src/kernel/examples/cl_example.cpp` : rainbow triangle

![](https://i.imgur.com/CXsErIc.jpg)

### TODO:
- add more GL examples (spinning cube, suzanne)
- write a mini opengl implementation example?
- research how to use vulkan commands
- add other types of jobs supported by the GPU (namely CSD for Compute Shader Dispatch and TFU for Texture Formatter Unit)
- some parts still need cleaning
- write some documentation about the GPU