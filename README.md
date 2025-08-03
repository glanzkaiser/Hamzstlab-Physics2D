# Hamzstlab-Physics
We create a lot of examples of Physics problems from Fundamental of Physics book and remove the original examples from Box2D version 2.4.2.

The main purpose is to be able to simulate basic Physics problems from undergraduate degree. 

In some of the examples, we have add the codes to export textfile of the simulation' computation so we can plot it later on with plotting library like `gnuplot`.

<img src="https://github.com/glanzkaiser/Hamzstlab-Physics/blob/main/images/HamzstlabLogo972.png" width="51%">

## Dependencies

1. Box2D

Box2D v2.4.2 is the main engine behind Hamzstlab Physics. In this version of Hamzstlab Physics (v1.5), we still use Box2D v2.4.2 that are still using C++. The newest of Box2D v3.1.0 / v3 above is using C now.

You have to create the shared library of Box2D first then install it in `/usr/` or `/usr/local` or any prefix that already set as a location to look for library files and include folder.

### Build and Install Box2D Library
Assuming you are using UNIX / Linux OS. These commands below are designated for these OS.

Go to `/dependencies/` then open terminal

```
cd box2d-2.4.2

mkdir build
cd build
cmake -DBOX2D_BUILD_TESTBED=OFF -DBOX2D_BUILD_UNIT_TESTS=OFF -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=/usr ..
make
make install
```

You can also use this alternative:

```
cd box2d-2.4.2

mkdir build
cd build
ccmake ..
make
make install
```
<img src="https://github.com/glanzkaiser/Hamzstlab-Physics/blob/main/images/5.png" width="50%">

choose from the GUI on terminal to only build the shared library.

It will install / put `libbox2d.so` into `/usr/lib` and also the `/include` folder that contain tons of header files for Box2D will be installed in `/usr/include`.

We have modified some source codes of Box2D so it will show the dynamic body with different color for different mass bodies (light body, medium body, heavy body).
If you would like to stay with the original coloring then you should download the official Box2D version 2.4.2 from Erin Catto' github/website.

Box2D itself requires some dependencies:

1. glad (https://glad.dav1d.de/)
2. glfw (https://www.glfw.org/)
3. imgui (https://github.com/ocornut/imgui)
4. sajson (https://github.com/chadaustin/sajson)

they are already in the `dependencies/include` folder and you can move them to e.g. `/usr/include/` or you can learn by yourself to build then install them one by one if you haven't got one yet.

# Build testbed 

Go to the root directory of this repository `/` then open terminal

```

mkdir build
cd build
cmake ..
make

./hamzstlabphysics
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
| Body Falling in a Dense Fluid | <img src="https://github.com/glanzkaiser/Hamzstlab-Physics/blob/main/images/bodyfallinginadensefluidfinal.gif" width="83%">
| Water Tank Mixing | <img src="https://github.com/glanzkaiser/Hamzstlab-Physics/blob/main/images/tankwaterfinal2.gif" width="83%">
| Filling Water Tank| <img src="https://github.com/glanzkaiser/Hamzstlab-Physics/blob/main/images/fillingwatertank.gif" width="83%">

The water tank mixing is a test of creating the 2D version from Example 1 from chapter 2.3 of the Boyce DiPrima book [2].

<img src="https://github.com/glanzkaiser/Hamzstlab-Physics/blob/main/images/6.png" width="60%">

# Source

1. David Halliday, Robert Resnick, and Jearl Walker, Fundamental of Physics 10th Edition.
2. Boyce, William E., DiPrima, Richard C., Elementary Differential Equations and Boundary Value Problems 9th Edition.