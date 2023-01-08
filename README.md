# OpenGL_Shapes

This project was a baby step of learning a little about OpenGL. I saw this project in a course at my university (I was not in the class) and decided to do it myself in OpenGL and C. The idea is to make a psuedo MSPaint-like program that allows the user to draw shapes on the screen and edit the shapes as well. There must also be an undo/redo function. The control of the program is to have two modes: insert and edit mode. The user can switch between these modes with space. In either mode, the user should be able to undo and redo actions they have taken in the past. Below is a sample image of what the program looks like.

![](/SampleImage.png)

## Insert Mode

This is the default mode of the program. Here the user can press `t` to select triangle (this is the default). With triangle selected, the user can click once to start drawing and then move the mouse around to preview the triangle. They can click once more to place the triangle onto the scene, so to speak. Similarly, `r` selects to draw a rectangle and the behavior for drawing the shape is the same as before.

When the user is not drawing out a shape, they can either undo/redo with `z` or `y`, respectively, or they can switch to edit mode with `space`.

## Edit Mode

During edit mode, the user can select a shape by clicking on it once. Then the shape will move around with the mouse. Clicking once more will place the shape down onto the scene. However, while the shape is selected they can press `delete` to delete the shape off the screen.

When the user is not moving a shape around, they can either undo/redo with `z` or `y`, respectively, or they can switch to insert mode with `space`.
