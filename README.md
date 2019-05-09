# Fortuna 2048
Implementation of the game 2048 for the La Fortuna board, built for the Computer Systems 2 undergraduate module at the University of Southampton.

By Bradley Garrod (bjg1g17) & Samuel Collins (sdc1g17)

## Acknowledgements
- Steve Gunn for much of the display logic (`lcd.c`)
- Peter Dannegger (danni@specs.de) for the ruota code (used for handling input).
- Frank Vahid, Bailey Miller, and Tony Givargis for [RIOS](http://www.cs.ucr.edu/~vahid/rios/)
- Gabriele Cirulli for creating [2048](https://github.com/gabrielecirulli/2048)
- Thomas Barth for [RGB565 Color Picker](http://www.barth-dev.de/online/rgb565-color-picker/)

## Running the Game

Running the game requires the following:

* GNU make
* dfu-programmer
* avr-libc

Connect the La Fortuna board to your computer, and turn it on. Press the reset button at the top of the device.

Calling `make` will compile the c program, and then flash the game onto the La Fortuna device.

## How to Play
When you turn on your La Fortuna, you will be greeted by the 2048 logo. To begin the game, press the center button.

Use the directional buttons to slide the tiles within the grid. If 2 tiles with the same value collide during a slide, they will combine to make a tile of twice the value.

The goal is to produce a 2048 tile within the grid. The game is lost whenever you cannot make a move any longer. (Currently, the game will just continue past this point until you can no longer make a move)

Your score is incremented by the value produced by a combination of tiles.

## Screenshots

<img src="readme-images/title-screen.jpg" width="400"/>
<img src="readme-images/new-game.jpg" width="400"/>
<img src="readme-images/mature-game.jpg" width="400"/>
<img src="readme-images/game-over.jpg" width="400"/>
