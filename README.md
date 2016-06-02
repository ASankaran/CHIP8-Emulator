CHIP8 Emulator
=======================================
Arnav Sankaran

Functionality
---------------------------------------
- Supports all 35 opcodes from the original CHIP8 standard. (https://en.wikipedia.org/wiki/CHIP-8#Opcode_table)
- Graphical interface and keyboard input are supported.

Keyboard Input
---------------------------------------
Keys are mapped as such:
```
Original               Emulator
1 2 3 C                1 2 3 4
4 5 6 D                Q W E R
7 8 9 E                A S D F
A 0 B F                Z X C V
```

Keyboard mapping and font provided by http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/.

Using the Emulator
---------------------------------------
- Place a 10 by 10 pixel black image named PixelBlack.bmp in the same folder as the executable.
- Place a 10 by 10 pixel white image named PixelWhite.bmp in the same folder as the executable.
- Run programs by passing them as a command line argument. `./<executable-name> <program-name>`

Known Issues
---------------------------------------
- Scoring in games such as pong and brix is not properly updated.