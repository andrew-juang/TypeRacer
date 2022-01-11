# ![TypeRacer](https://i.imgur.com/gUjuZST.gif)
Project Proposal by Aaron Visser, Andrew Juang, and Dylan Hu | Systems Period 4

## Description
TypeRacer is a multiplayer typing test racing game. You have to type a text as fast as you can to try and beat others who are typing the same text.

## User Interface
### Menus
- (maybe later)

### Typing interface
<img width="550" alt="Mockup of Typing Interface" src="https://user-images.githubusercontent.com/43192121/148847406-95b9ef1b-d356-4044-ab8f-d57bddaf3302.png">

- The text to be typed will be displayed, likely in its entirety depending on total length. It will be in a darker color and font to signify that it has not been typed yet. Typed characters are lighter and bolded, and incorrect characters are highlighted in red.

- The instantaneous WPM (words per minute) and accuracy will be displayed.

#### Coming soon??
- The player will also be able to see the progress and speed of other players.


## Technical Design
### Client-server communication protocol
#### Rationale
- Since network communication via TCP in C is a bit funky, we need a way to make sure that the data that we're sending over the network is completely transmitted and readable on the other side of the connection. We are sending more than just plain text over the network, so we need to be able to reliably tell the difference between many different types of messages.

#### Protocol Details
- Coming soon.


### Server side
#### Accepting connections
- After startup, the server will bind to a port and being to listen for incoming connections. Upon acceptance of the connection, it will wait for a message from the client that will signify whether or not to create a new room (fork a subserver) or join an existing room (how?).

#### Generating text
- The server will generate the text to be out of an array of pointers to possible text string to be typed by all players. It will send this to all clients. (how?)

#### Relaying players' progress
- Coming soon.


### Client side
The client is responsible for connecting to the server, recieving the text to type, displaying text and other players' progress on the screen, recieving input, and sending progress messages to the server.

#### Connecting to the server
On execution the client will prompt the user for an IP address or hostname to connect to. The client will then connect to the server specified using a socket. On connection to the server, the client will recieve a message packet from the server containing the text to type. The client will display the text on the screen while the server waits for others to connect. As others connect to the server, the client will recieve notification of these other clients and show their username/progress on the screen (which will be 0% as the game hasn't started yet).

#### Starting the race
When the server sends a race start message packet, the client will display a countdown timer until the race starts.

#### During the race
During the race, the client will have to be able to respond to keyboard input as well as incoming messages relayed by the server. ncurses provides a function to get the next character `getch()`, which can be set to a non-blocking buffered mode. The socket used to connect to the server will also be set to non-blocking mode.

During the game loop, the client will continuously check for keyboard input using `getch()`, and if recieved process the input and redraw the screen. It will then call `recv()` and if any data is recieved, process and redraw the screen.

(more info later)


### External Libraries
- ncurses
  - On debian: `sudo apt install libncurses5-dev libncursesw5-dev`
  - On other systems google: "install ncurses (platform)"

### Responsibilities
- Aaron: Does stuff 
- Andrew: Does stuff
- Dylan: Does stuff

### Data Structures / Algorithms Used
- ASLDKJfklsdjfl;ajkfaskjfas

## Timeline
- [x] Proposal (1/12)
- [ ] Integrate client / server from past HW (1/13)
- [ ] TypeRacer for single person (1/18)
- [ ] Set up UI using ncurses (1/18)
- [ ] Set up single room, multiple users (1/20)
- [ ] Multiple rooms, multiple users (1/22)
- [ ] Menus / GUI (Whatever remaining time we have)
