
# Linux Based File Explorer


A fully functional File Explorer Application, with a restricted feature set.

## Steps to Run
- Open the terminal in Linux where `main.cpp` is located.
- Run `g++ main.cpp` on terminal
- Run `./a.out` on terminal

## Some Assumptions

- Terminal Size should be :Minimum Width-**90** ,Minimum Height:**10**
- If window is resized **vertically**,then it will auto-adjust the content by pressing any **arrow key** 
- For any **Space** serparated File/Directory name, `\` should be used before space.For Example if file name is `Advance AOS.txt` then you should give input `Advance\ AOS`.
- Deletion/Rename of any directories which are on above level of current directories is NOT ALLOWED.For Example Current Directory is C and path is A/B/C. So Deletion/Rename of A,B are NOT ALLOWED.

## Standards Considered
- Directory from where Application is launched is Considered as **root(home)**.
- Application in NORMAL MODE can't go in above level folder of that root(home).



| Symbol             | Meaning                                                                |
| ----------------- | ------------------------------------------------------------------ |
| ~/ and / | Root(home) of application where it started |
| ./ | Current Directory |
| ../ | Parent Directory of Current Directory |



## Features 
- Changes will be reflected in NORMAL MODE file list also as you execute any command from COMMAND MODE.
- Directories will show **Total Size** including it's subfiles as well as folders.
- Scrolling can be done by `k` and `l` keys ,irrespective of cursor position. 

## Demo Image

![demo](https://user-images.githubusercontent.com/43428382/140296262-674a3b7a-621e-4832-9ef8-704385d5b468.png)
