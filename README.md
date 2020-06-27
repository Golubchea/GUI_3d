# GUI_3d
	
![view](/gui3d.png)

Simple graphic application for modelling or map editor purposes
 
# Features: 

* json load save
* gui written on callback functions
* may support ios macOS Windows Unix (debian, arch), EMSCRIPTEN
* modern opengl (4>)
* SDF utf-8 text rendering (support russian lang)
* CollisionRayModel may used for vertex selection
* plane collision
* 1 dependency, only SDL2 (included in libs folder)
* my own matrix math

# Hotkeys:

* wasd, middle mouse  -moving camera, i o- top, front view of object, t,r,e,q,c - translate, rotate, scale object,snap to grid ,use local coordinates
f-12 screenshot

# Compiling:

in root directory:
mkdir build 
cd build 
cmake ..
make -j8

# Third party:

tiny-gizmo
stbfreetype
stbimage
stbimage-write
sdl2

# Needs to run opengl in ubuntu

* sudo apt install libext-dev
* sudo apt install libx11-dev
* sudo apt install libgl1-mesa-dev
* sudo apt install libglu1-mesa-dev
* sudo apt install libxext-dev

* sudo apt install libsdl2-dev



