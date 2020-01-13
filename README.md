<!--
  Tower of Hanoi Instructions

  Author: Jun Jung
  Student Number: 292961
  Email: jun.jung@tuni.fi
-->

# Tower of Hanoi Instructions

This document contains instructions of playing the game 'Tower of Hanoi'.


## 1. Objectives

There are three pegs where user can place disks.

The first peg is called source where all disks are located at the beginning of the game.
The number of disks are different depening on the difficulty of the game; easy difficulty starts with 4 disks, middle does with 6, and difficult does with 8.

The second peg is called auxiliary where disks can stay temporarily.
It is not a destination, thus it doesn mean winning if all disks are moved to auxiliary peg from source.

The last peg is called target where all disks have to be located to win the game.

Therefore, objectives of the game is to move a set of disk from source peg to target.


## 2. Rules

These are basic rules of the game.
All actions against the rules will be considered as invalid movement.

1. Only one disk can be moved at a time.

2. Each move consists of taking the upper disk from one of the stacks and placing it on top of another stack or on an empty rod.

3. No larger disk may be placed on top of a smaller disk.

Rule source: [Wikipedia: Tower of Hanoi](https://en.wikipedia.org/wiki/Tower_of_Hanoi)


## 3. Game Play

A game starts by clicking the 'Start' button.
The difficulty level of the game is set to easy as default, but it is changeable before starting.

Once it starts, the disks can be moved to from a peg to another by using [three different methods](#4-moving-disk-methods).
The movement of disks is restricted by [game rules](#2-rules).

The game can be paused or continued by clicking the 'Pause'/'Continue' button which is originally used for starting.

When all disks at the source peg have moved to the target in order of size of disks from the smallest to the biggest, the game ends.

Without closing the program, a new game can start after reseting the previous game.


## 4. Moving Disk Methods

There are three methods implemented for moving disks from a peg to another.
All movements are available only when a game is playing, that is, users cannot move disks when a game is paused or reseted or finished.

1. Move button method
    * A single disk can be moved at a time by clicking a move button.
    * Six buttons are available.
    * Each button moves the top disk of a peg.
    * Invalid movement does not drive any movement.

2. Key command method
    * A single disk can be moved at a time by pressing a combination of two keboard commands.
    * Only keys 'A', 'B' and 'C' are acceptable.
    * Two key commands are required for departure and destination.
        * Departure: The peg which a disk moves from.
        * Destination: The peg which a disk moves to.
    * After setting a departure point, the system is waiting for a destination for three seconds.
      If the next key is not input within three seconds, it requests from first key.
    * This will move from the top disk of the departure peg.
    * Invalid movement does not drive any movement.

3. Drag and drop method
    * A single disk can be moved at a time by dragging and dropping.
    * Only the top disk of a peg can be movable.
    * The disk needs to be located properly to the desiring peg.
    * Invalid movement will take the selected disk back to its original position.


## 5. Graphic User Interface

```text
							 Log:
 Best Record:		Difficulty: 			+-----------------------+
 +-----------+			    +-------+		|			|
 |   0 |   0 |		* Easy      | Start |		|			|
 +-----------+			    +-------+		|			|
			o Middle    | Reset |		|			|
 Play Time:			    +-------+		|			|
 +-----------+		o Difficult | Close |		|			|
 |   0 |   0 |			    +-------+		|			|
 +-----------+	  +--------+ +--------+ +--------+	|			|
		  |  A->B  | |  B->A  | |  C->A  |	|			|
		  +--------+ +--------+ +--------+	|			|
		  |  A->C  | |  B->C  | |  C->B  |	|			|
		  +--------+ +--------+ +--------+	+-----------------------+

		 _			 _			 _
		| |			| |			| |
		| |			| |			| |
	       _|_|_			| |			| |
	     _|_____|_			| |			| |
	   _|_________|_		| |			| |
	 _|_____________|_		| |			| |
	|_________________|		|_|			|_|
	    A(Source)		    B(Auxiliary)	     C(Target)
```

1. Best record  - The best result (time) of the games played so far; minutes and seconds.
		  When a user completes a game with faster play time than best record, 
		  it will be updated with new record.

2. Play time    - The play time of a game since it has started.
		  The time will not last when a game is paused.
		  Reset button will reset the play time as well.

3. Difficulty   - The difficulty level of the game.
		  Three different difficulties are offered by this game; easy, middle and difficult.
		  The default difficulty is set as easy.
		  Difficulty can be set before a game starts.
		  Once it has started, difficulty is only changeable after reseting the game.
		  Changing difficulty will also change the number of disks immediately.

4. Start button - A button to start the game.
		  When a game starts, the play time also counts time.
		  Once it starts, the button changes to pause button that can puase the game.
		  When the game is paused, the button changes to continue button to resume the game.

5. Reset button - A button to reset the ongoing game.
		  Once it is clicked, all disks move to source peg and play timer and log will be reset.
		  
6. Close button - A button to quit the program.
		  This will earse game data such as best record, log and game status.

6. Move buttons - Buttons to move disks from a peg to another.
		  For example, the button 'A->B' will move the top disk on the source peg to auxiliary.
		  Invalid movement does not drive any movement.
		  The buttons are only available when a game is running.

7. Log browser  - A text browser to display the history of movements;
		  it contains information which pegs a disk moves from and to.


