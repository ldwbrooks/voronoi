Physics Game
===

This is a hobby project coded in C++ using OpenGL, "licensed" in the public domain.

Voronoi
---
The project renders a Voronoi diagram as an exercise in OpenGL development. This makes use of the MyGAL library to handle Voronoi diagram generation, and earcut.hpp for converting the Voronoi diagram's polygons into triangles for my basic OpenGL triangle renderer to render to the screen.

The diagram can be regenerated with Lloyd's relaxation by pressing 'g', reset with 50 new nodes by pressing 'r', or reset with 100 new nodes by pressing 't'. Various parts of the graph's rendering can be disabled by pressing 'u', 'j', and 'b', and reenabled by pressing 'y', 'h', and 'v'.

https://user-images.githubusercontent.com/11843720/170187910-f9e5a8e9-2e10-479a-a0c1-349571271f81.mp4

Dependencies
===
This project has the following dependencies: GLFW, GLAD, and GLM for graphics, MyGAL for Voronoi diagram handling, and mapbox's earcut.hpp for polygon triangulation/tesselation of said Voronoi diagrams. There is no implied endorsement from any of these dependencies, and each has their own license.

#### Links for each dependency
[GLFW](https://www.glfw.org/download.html)
[GLAD](https://glad.dav1d.de/)
[GLM](https://glm.g-truc.net/0.9.8/index.html)
[MyGAL](https://github.com/pvigier/MyGAL)
[earcut.hpp](https://github.com/mapbox/earcut.hpp)

Currently, this project requires the user to handle the above dependencies themselves. The CMakeLists.txt expects the include files for all of the above repositories, the lib files for Box2D and GLFW, and the glad.c file for GLAD to be found in a folder called coding/external/ in the active Windows user's Documents directory. Please edit those paths in CMakeLists.txt to match the appropriate files on your own machine.

License
---
As mentioned above, this project is in the public domain, and is free for anyone to view and use as they see fit, including redistribution in any form you choose.

Namespace
---
Everything is contained within the namespace "ldwb" in order to avoid potential collisions with other software. This namespace was chosen for its short length and unlikeliness of being used elsewhere; the combination of letters is simply my own initials. (Inspiration for this namespace was taken from the "stb" of "stb-image.h".)
