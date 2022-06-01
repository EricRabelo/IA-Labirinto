#!/bin/bash
reset
g++ runMaze.cpp -o mazeExec -lGLU -lGL -lglut
#g++ -lGLU -lGL -lglut runMaze.cpp -o mazeExec
#g++ /System/Library/Frameworks/GLUT.framework/GLUT /System/Library/Frameworks/OpenGL.framework/OpenGL runMaze.cpp -o mazeExec

./mazeExec $1

rm mazeExec
