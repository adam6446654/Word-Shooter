Word Shooter - 2D Alphabet Puzzle Game

A 2D word-based puzzle game developed in C++ using OpenGL. This project is based on the skeleton code of a Cookie Crush-style game originally written by Sibt ul Hussain. It was created as part of a Programming Fundamentals course assignment.

Overview

Word Shooter is a grid-based game where players shoot alphabets onto a board to form valid English words. When a valid word is formed horizontally, vertically, or diagonally, the letters are removed, and points are awarded. The game runs on a timer, players have to form as many words as possible before time runs out.

Features

- Grid-based gameplay with a 22x31 tile board
- Alphabet shooting mechanic with mouse interaction
- Real-time word detection using a built-in dictionary
- Burst and scoring system based on word formation
- Timer-based gameplay with game-over logic

Getting Started

Prerequisites:
This game requires OpenGL, GLUT, GLEW, and g++ to be installed on your system.

Linux (Ubuntu) Installation:

1. Run the provided install-libraries.sh script:
   chmod +x install-libraries.sh
   ./install-libraries.sh

2. If libglew1.6-dev is not available, install an older version:
   sudo apt-get install libglew1.5-dev

Compilation:

To compile the project, run:
   make

To run the game:
   ./word-shooter

Controls

- Left Mouse Click: Shoots the current alphabet to the clicked position on the board.

Gameplay Rules

- The board starts with the first two rows filled with random alphabets.
- The player shoots alphabets from a launcher at the bottom of the screen.
- Alphabets must remain within board boundaries.
- Words can be formed in horizontal, vertical, or diagonal directions.
- All valid word sequences are detected and removed.
- The longest possible match is preferred for scoring.
- Each removed alphabet grants one point.
- The game lasts for two minutes, after which a "Game Over" message is displayed.

Technical Details

- The canvas size is 930x660 pixels, divided into 30x30 pixel tiles.
- Alphabet drawing is handled using provided utility functions.
- All rendering is performed within the Display() function, using glutPostRedisplay() for redraws.
- The game dictionary contains over 370,000 English words.

Project Structure

util.h / util.cpp       - Drawing utilities and canvas setup
word-shooter.cpp        - Game logic and main functions
dictionary.txt          - Word list for word detection
Makefile                - Build configuration
install-libraries.sh    - Dependency installer
README.md               - Project documentation

Credits

This project is based on code by:
Author: Sibt ul Hussain
File: cookie-crush.cpp
Description: Basic 2D game of Cookie Crush
