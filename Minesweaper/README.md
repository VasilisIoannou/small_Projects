# Terminal Minesweeper (Windows)

A classic Minesweeper clone built in C++ specifically for the Windows console. This version features a fully navigable menu system, multiple difficulty levels, game persistence (save/load), and high-score tracking.

## Features

- **Three Difficulty Levels:** * **Easy:** 10x10 grid, 20 bombs.
    
    - **Medium:** 20x20 grid, 60 bombs.
        
    - **Hard:** 30x30 grid, 120 bombs.
        
- **Save & Load System:** Supports up to 3 separate save slots so you can pause your session and return later.
    
- **Statistics:** Tracks your best completion times for each difficulty level.
    
- **Recursive Digging:** Classic "chain-reaction" clearing when you click on an empty space.
    
- **Color-Coded Interface:** Visual cues for flags, cleared areas, and hidden tiles using Windows Console attributes.
    

---

## How to Play

### Controls

|Key|Action|
|---|---|
|**WASD / Arrow Keys**|Move the cursor|
|**Spacebar**|Toggle between **DIG** and **FLAG** mode|
|**Enter**|Perform the selected action on the current tile|
|**ESC**|Save the current game and return to the Main Menu|
### Objective

Clear the entire board without hitting a mine. Use the numbers revealed on the board to determine where mines are hidden. If you suspect a tile contains a mine, toggle to **FLAG** mode to mark it.

---

## Requirements & Compilation

### Requirements

- **Operating System:** Windows (uses `<windows.h>`)
    
- **Compiler:** A C++ compiler (like MinGW/GCC or MSVC)
    

### Compilation (via G++)

Open your terminal in the project directory and run:
g++ -o minesweeper.exe main.cpp

##  File Structure

The game automatically generates data files to manage your progress:

- `SaveFile_X.dat`: Stores game state for slots 1, 2, or 3.
- `scores.dat`: Stores your best times for the statistics menu.
- `timer.dat`: Tracks elapsed time for active sessions.
    

---

## Notes

- **Screen Size:** Since the game uses specific coordinate offsets, ensure your terminal window is large enough (especially for **Hard** mode) to avoid visual glitches.
    
- **Terminal Compatibility:** This version is designed specifically for the Windows Command Prompt or PowerShell.
    

---

**Happy Digging! 💣**
