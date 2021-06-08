# Button operations:
Left, Right, Down. Move the block with correspond direction.
Space. Rotete the block clockwise.

# Score depend on number of line cleared:
One line is 100 points.
Two lines are 200 points.
Three lines are 600 points.
Four lines are 1200 points.

# Build Environment:
Visual studio 2019.

# Concepts:
Divide the system into three parts.

Model is responsible for logical operation of game.
View is responsible for drawing the game result.
Control is responsible for detecting keyboard state.

Model class is Tetris.
View class is Drawer.
Control class is KeyObserver.

Intermediate layer between model and view with observer pattern.
Intermediate layer between model and control with command pattern.
View and control are independent each other.
