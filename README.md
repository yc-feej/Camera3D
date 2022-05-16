# Simple 3D Camera

## Target

This repo implements a simple 3d camera that could process universal inputs from controllers. You should implement all functions marked as `TODO` based on what we discussed in transform part, function names or comments. Please use `camera/camera_3d.cpp` for your own code. 

## Build

We use bazel to build our repo. Please follow the [guide](https://docs.bazel.build/versions/main/install.html) to install bazel properly.

We suggest to use bazel version >= 4.2.1. You can remove the check in `WORKSPACE` if you insist on using a lower version.

You can use following command to build your code:

```
bazel build //camera:camera_3d
```

You can use this camera code in any standard graphics library examples.