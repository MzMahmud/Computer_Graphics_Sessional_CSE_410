CSE 410: Computer Graphics Sessional
====================================
This was a course in my Level 4 Term 1.The name clearly suggests this is a course on Computer Graphics. In the theory class we learnt about basics computer graphics rendering techniques and algorithms.
The sessional was all about implementing some techniques using C++'s **OpenGL** library.


Fully Controllable Camera,Sphere to/from Cube,Wheel
---------------------------------------------------
1. **Fully Controllable Camera**
    Key map for different operation was implemented using math and raw OpenGL library.
    
    * up arrow - move forward
    * down arrow - move backward
    * right arrow - move right
    * left arrow - move left
    * PgUp - move up
    * PgDn - move down
    * 1 - rotate/look left
    * 2 - rotate/look right
    * 3 - look up
    * 4 - look down
    * 5 - tilt clockwise
    * 6 - tilt counterclockwise

2. **Sphere to/from Cube**
    Key map for different operation was implemented using math and raw OpenGL library.

    * Home - cube to sphere
    * End - sphere to cube
    
    Drawn one eighth of a sphere, one fourth of a cylinder and a square once. Used transformations (translation, rotation etc.) to put them in the right places. And implemented the operations.


    *Sphere to/from Cube with Fully Controllable Camera*
    ![Sphere to/from Cube with Fully Controllable Camera](https://raw.githubusercontent.com/MzMahmud/CSE410-ComputerGraphicsSessional/master/Assignment%201/camera%2Bsphere.PNG)

3. **Wheel**
    Key map for different operation was implemented using math and raw OpenGL library.

* w - move forward
* s - move backward
* a - rotate left
* d - rotate right

*Wheel Demo*
![Wheel Demo](https://raw.githubusercontent.com/MzMahmud/CSE410-ComputerGraphicsSessional/master/Assignment%201/wheel.PNG)

Unboxing a Cube
---------------
We were given a task in the lab to build a unboxing cube with basic translation in OpenGL.

**Cube unboxing**
![Cube unboxing](https://raw.githubusercontent.com/MzMahmud/CSE410-ComputerGraphicsSessional/master/Online%201/cube.png)  

Modeling and ViewTransformation
-------------------------------
Given the coordinated of a set of **objects**(triangle) and a set of **transformations**(translate,scale,rotate,push,pop) and the **camera position** (look and up) The task of this assignment was to find the final 2D coordinate that is modelling and view transformation with **raw C++** without any library.

Ray Casting
-----------
Given a set of objects(**sphere**,**pyramid** and a **chess board floor**),a set of **light source** and the camera position. The goal is to generate an image with **ray tracing** and give the image a realistic look using **shading** like **Phong Model,Lambert Mode**.


*Ray tracing output (**before shading**)*
![Ray tracing output(before shading)](https://raw.githubusercontent.com/MzMahmud/CSE410-ComputerGraphicsSessional/master/Assignment%203/before.bmp)


*Ray tracing output(**After shading**)*
![After shading](https://raw.githubusercontent.com/MzMahmud/CSE410-ComputerGraphicsSessional/master/Assignment%203/after.bmp)

Fractals
--------
This was not actually part of any assignment. I was loved the beauty of fractals so implemented a few fractals in OpenGL.

* Fractal Tree
* Koch Snowflake
* Dragon Curve

![Fractals](https://raw.githubusercontent.com/MzMahmud/CSE410-ComputerGraphicsSessional/master/Fractal/fractals.png)
