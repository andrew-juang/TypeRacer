# ![TypeRacer](https://i.imgur.com/gUjuZST.gif)
Project Proposal by Aaron Visser, Andrew Juang, and Dylan Hu | Systems Period 4

## Description
TypeRacer is a multiplayer typing test racing game. You have to type a text as fast as you can to try and beat others who are typing the same text.

## Getting Started

* Clone the Repository
```
git clone https://github.com/Andrew1J/TypeRacer.git
```
* Change Directories
```
cd TypeRacer
```
* Compile using make
```
make
```
* Run Server then run client

`make server` and `make client` in separate windows

* Press escape if you want to escape the client and ctrl-c if you want to stop the server

### External Libraries
- ncurses
  - On debian: `sudo apt install libncurses5-dev libncursesw5-dev`
  - On other systems google: "install ncurses (platform)" and follow instructions that look somewhat trustworthy

## Timeline
- [x] Proposal (1/12)
- [x] Complete protocol implementation (1/16)
- [x] Client / Server initial connection (1/18)
- [x] Set up UI using ncurses (1/18)
- [ ] Finish game loop portion (1/22)
- [ ] Set up single room, multiple users (1/22)
- [ ] Better Menus / GUI / Text Selection (Whatever remaining time we have)
