# vigilant-chip8
Simple chip8 emulator in c++

## Run with
```
./Vigilant-chip8 [path to chip8 program]
```
## Debug mode allows you to step through the emulation.
```
./Vigilant-chip8 --debugger [path to chip8 program]
```
### Debugger commands
- ``` step ```, ``` s ``` - Steps through one instruction cycle
- ``` print [memory address] ``` - prints value in memory at address
- ``` print V(register index) ``` - prints V registers at index
- ``` print I ``` - prints I register
- ``` print PC ``` - prints PC register
- ``` print timer ``` - prints timer register
- ``` print sound ``` - prints sound timer register
- ``` print cpu ``` - prints state of all chip8 registers
- ``` continue ``` - stops debug mode and continues execution of the program until it terminates
- ``` exit ``` - stops execution of the chip8 program and closes the emulator

## Cool pictures
![Preview](http://i.imgur.com/qv9BhMa.png "Space Invaders!")
![Preview](http://i.imgur.com/l9u89xx.png "C8")

# Dependencies
* must have sfml 2.3 installed
