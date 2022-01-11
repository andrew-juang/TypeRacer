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
#### Connecting to the server
- The client will first prompt for a IP address or hostname to connect to. It will connect to the server, and prompt the user to choose between creating a new room or joining an existing room. Whichever the option is, the client will send the appropriate message to the server.

#### Gameplay
- Coming soon.

### External Libraries
- Ncurses
```
sudo apt get something
```

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
