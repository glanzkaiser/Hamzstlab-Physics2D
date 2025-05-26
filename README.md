# Hamzstlab-Physics
We create a lot of examples of Physics problems from Fundamental of Physics book and remove the original examples from Box2D version 2.4.2.

The main purpose is to be able to simulate basic Physics problems from undergraduate degree. 

In some of the examples, we have add the codes to export textfile of the simulation' computation so we can plot it later on with plotting library like `gnuplot`.

## Dependencies

1. Box2D

Box2D v2.4.2 is the main engine behind Hamzstlab Physics. In this version of Hamzstlab Physics (v1.5), we still use Box2D v2.4.2 that are still using C++. The newest of Box2D v3.1.0 / v3 above is using C now.

You have to create the shared library of Box2D first then install it in `/usr/` or `/usr/local` or any prefix that already set as a location to look for library files and include folder.

### Build and Install Box2D Library
Assuming you are using UNIX / Linux OS. These commands below are designated for these OS.

Go to `/dependencies/` then open terminal

```
tar -xvf box2d-2.4.2.tar.gz
cd box2d-2.4.2

mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=/usr ..
make
make install
```

It will install / put `libbox2d.so` into `/usr/lib` and also the `/include` folder that contain tons of header files for Box2D will be installed in `/usr/include`.

Box2D itself requires some dependencies:

1. glad
2. glfw
3. imgui
4. sajson

they are already in the `dependencies/include` folder and you can move them to e.g. `/usr/include/` or you can learn by yourself to build then install them one by one if you haven't got one yet.

# Build testbed 

Go to the root directory of this repository `/` then open terminal

```

mkdir build
cd build
cmake ..
make

./testbed
```

It will open the Imgui window console / GUI.

If you want to create your own example, check out the directory `/tests/` that are full with `.cpp` files, you can learn the source code to create the simulation here.

After that don't forget to adjust the `/CMakeLists.txt` at the root directory, then build again at the folder `build` so your example will appear.

# Simulation Examples

| Name | Simulation |
| -------------     | ------------- | 
| Vertical Spring   | <img src="https://github.com/glanzkaiser/Hamzstlab-Physics/blob/main/images/1.png" width="83%">
| Crate Pendulum    | <img src="https://github.com/glanzkaiser/Hamzstlab-Physics/blob/main/images/2.png" width="83%">
| Circular Motion   | <img src="https://github.com/glanzkaiser/Hamzstlab-Physics/blob/main/images/3.png" width="83%">
| Spring Two Masses | <img src="https://github.com/glanzkaiser/Hamzstlab-Physics/blob/main/images/4.png" width="83%">

# Source

1. David Halliday, Robert Resnick, and Jearl Walker, Fundamental of Physics 10th Edition.