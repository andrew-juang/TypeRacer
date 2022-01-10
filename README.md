# TypeRacer
By Aaron Visser, Andrew Juang, and Dylan Hu | Systems Period 4

## Description
TypeRacer is a multiplayer typing test racing game. You have to type a text as fast as you can to try and beat others who are typing the same text.

## User Interface
### Menus
(maybe later)

### Typing interface
<img width="400" alt="c_typeracer_mockup" src="https://user-images.githubusercontent.com/43192121/148847406-95b9ef1b-d356-4044-ab8f-d57bddaf3302.png">

The text to be typed will be displayed, likely in its entirety depending on total length. It will be in a darker color and font to signify that it has not been typed yet. Typed characters are lighter and bolded, and incorrect characters are highlighted in red.

The instantaneous WPM (words per minute) and accuracy will be displayed.

#### Coming soon??
The player will also be able to see the progress and speed of other players.


## Technical Design
### Server side
After accepting connections through sockets (TODO: expand), the server will generate the text to be typed by all players. It will send this to all clients. 

### Client side
TODO

## Timeline
## Timeline
- [x] Proposal 1/12
- [ ] stuff
- [ ] more stuff
- [ ] 
