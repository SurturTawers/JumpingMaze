# JumpingMaze
Implementation of a "Jumping Maze"(?) (Laberinto Saltarín).\
\
Basically, it creates a labyrinth with dimensions, starting cell, target cell and jump value entered by console, then it tries to find the target cell jumping around "jump value" cells at a time, vertically or horizontally.

## Algorithm
It has a jumpy node that has knowledge of the top, down, left and right cells that it can jump to, it decides where to jump given the euclidean distance that it has to the target cell after "jumping".\
\
Also, there's a matrix with all the cells that the node has visited, this way it can decide if the labyrinth can be completed.\
\
If the jumpy node can not jump "forward" because it has already visited all other posible cells or it cannot jump because of dimension constraints, it has the ability to "jump backwards" which means it can go back a jump and try another cell that it has not yet jumped to.
