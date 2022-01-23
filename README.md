# ![TypeRacer](https://i.imgur.com/gUjuZST.gif)
Project by Aaron Visser, Andrew Juang, and Dylan Hu | Systems Period 4

## Description
TypeRacer is a multiplayer typing test racing game. You have to type a text as fast as you can to try and beat others who are typing the same text.

## Getting Started

### External Libraries Installation
- ncurses
  - On debian: `sudo apt install libncurses5-dev libncursesw5-dev`
  - On other systems google: "install ncurses (platform)" and follow instructions that look somewhat trustworthy

### Run the Game
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

## Technical Design
### Client-server communication protocol
#### Rationale
- Since network communication via TCP in C is a bit funky, we need a way to make sure that the data that we're sending over the network is completely transmitted and readable on the other side of the connection. We are sending more than just plain text over the network, so we need to be able to reliably tell the difference between many different types of messages.

#### Protocol Details
- Each packet starts with `-=-=-=-=-=-` and ends with `-+-+-+-+-+-`. The first byte after the beginning is the type of the packet. The rest of the packet is data. We will implement functions for every type of packet to send/recieve over a socket to be used in both the client and server. For packing `int`s and others into portable formats, we will probably use [the htons family of functions](https://linux.die.net/man/3/htons). Packets will be represented using structs to make our lives easier as we can pass them around easily.

##### Packet Types
0. On connection username packet <br>  This packet is the first packet sent from connecting clients to the server. It contains the username that the client has chosen. The first two (2) bytes contain the length of the username (`unsigned int`). The rest contains the username.
2. New player joined packet <br>  This packet is sent to already connected clients when new clients join the server. It contains the username of the client that has joined. For new clients, if there are already joined clients, the server will just send a bunch of these to the new client.
3. Typing text packet <br>  This packet contains the text that the clients will type. The first two (2) bytes contain the length of the text (`unsigned int`). The rest of the packet is the text.
4. Race countdown start packet <br>  This packet signifies the start of a countdown until which the race will start. It contains the number of seconds until race start.
5. Race start packet <br>  This packet signifies the start of the race (in case clients are delayed somehow). It contains no data.
6. Progress packet <br>  This packet contains the username, percentage progress, and current WPM of the transmitting client. The first two (2) bytes contain the length of the username. Next is the username, and afterwards is two (2) bytes of the percentage progress, and finally two (2) bytes of the current WPM. All types are `unsigned int`s.


### Server side
#### Accepting connections
- After startup, the server will bind to a port and being to listen for incoming connections. 

#### Generating text
- The server will generate the text to be out of an array of pointers to possible text string to be typed by all players. It will send this to all clients. (how?)

#### Relaying players' progress
- While the race is ongoing, the server will `poll()` all of the client's sockets. This uses an array of `struct pollfd`. If anything happens, the server will just go through all of the sockets and relay the message along.


### Client side
The client is responsible for connecting to the server, recieving the text to type, displaying text and other players' progress on the screen, recieving input, and sending progress messages to the server. Keeping track of other players can be done with a linked list of structs, or just a regular array.

#### Connecting to the server
- On execution the client will prompt the user for an IP address or hostname to connect to as well as a username for the player. The client will then connect to the server specified using a socket, then it will send a message packet to the server containing the username selected. On connection to the server, the client will recieve a message packet from the server containing the text to type and the usernames of the other players, if applicable. The client will display the text on the screen while the server waits for others to connect. As others connect to the server, the client will recieve notification of these other clients and show their username/progress on the screen (which will be 0% as the game hasn't started yet).

#### Starting the race
- When the server sends a race start message packet, the client will display a countdown timer until the race starts. When the client recieves the race start message, it will move to the main race loop.

#### During the race
- During the race, the client will have to be able to respond to keyboard input as well as incoming messages relayed by the server. ncurses provides a function to get the next character `getch()`, which can be set to a non-blocking buffered mode. The socket used to connect to the server will also be set to non-blocking mode.

- During the game loop, the client will continuously check for keyboard input using `getch()`, and if recieved process the input and redraw the screen. It will then call `recv()` and if any data is recieved, process and redraw the screen. A sleep of a small amount will be used in order to not absolutely eat up all the CPU cycles.

- To write text on the screen, ncurses provides a bunch of print functions, which can be used to print text into the terminal in arbitrary positions. ncurses also provides `attron()` and `attroff()` to change the styling of the text.

#### Calculating errors and WPM
- Whenever a character is typed, the client will increment a total character counter. If the character typed is incorrect, a incorrect character counter will be incremented. WPM is basically something like: 

![WPM formula](https://latex.codecogs.com/png.image?%5Cdpi%7B110%7D%20%5Cbg_white%20%5Cfrac%7B%5B(%5Cfrac%7B%5Ctext%7BCharacters%20Typed%7D%7D%7B5%7D)-%5Ctext%7BErrors%7D%5D%7D%7B%5Ctext%7BTime%7D%7D)

#### Sending progress messages
- After every word (every time the user passes a white space in the original text), the client will send the most recent progress and WPM in a packet to the server.


## Timeline
- [x] Proposal (1/12)
- [x] Complete protocol implementation (1/16)
- [x] Client / Server initial connection (1/18)
- [x] Set up UI using ncurses (1/18)
- [ ] Finish game loop portion (1/22)
- [ ] Set up single room, multiple users (1/22)
- [ ] Better Menus / GUI / Text Selection (Whatever remaining time we have)
