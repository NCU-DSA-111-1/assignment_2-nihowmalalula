# shogidala

-------------------------------------
# shogidala

This is assignment 2，enter the number or char as program say，let's paly some shogi !

# File structure
SHOGIDALA/
         |——boardreset.h      # the header file for reset the board
         |        
         |——main.c            # the main function
         |
         |——new_game_file.txt # txt file for saving new game
         |
         |——old_game_file.txt # txt file for reading old game
         |
         |——Assignment_2      # my report
         |
         |——a.out             # the exe file
         |
         |——README.           # this file

## Compile & Run
# Compile
gcc main.c -lev
# Run(load old file)
./a.out –l old_game_file_name
# Run(play new file)
./a.out –n –s new_game_file_name
```
