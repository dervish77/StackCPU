# Stack CPU
Custom stack-based CPU design 

StackCPU is a custom CPU design that utilizes a stack-based memory model instead of the usual memory models seen in processors such as 6502 or Z80.  The StackCPU project was undertaken mostly as an exercise in virtual CPU design that could be implemented via a simulator and utilize custom assembler tools.  The StackCPU design definition has a limited memory size (4K bytes) to simplify any hardware implementations using the specification.  The StackCPU design should be simple enough to be implemented in hardware via an FPGA or perhaps even using discrete TTL devices.

The stack-based design for StackCPU uses the stack for nearly all operations.  Thus data is constantly moved on and off the stack during normal operation.  Some instructions use a combination of the stack and the accumulator register (i.e. math and logic operations).  The data memory region is used to store and retrieve temporary variables when stack operations might result in those values being lost or destroyed.  While the accumulator register (AC) is used for arithmetic and logic operations, it is not directly accessible via any machine instructions.  Likewise the data register (DR) is used to perform transfers to and from the data memory region, the register itself is not directly accessible via any machine instructions.

The StackCPU design includes registers for getting input from the outside world (IR) and (SR) and sending output to outside world (OR) and (PR).  The IR and OR registers are used to input and output of numerical values directly, or use them to read/write control signals for external devices.  The SR and PR registers are used to input and output ascii characters from/to a serial terminal via an RS232 port.

The StackCPU design's memory model is limited to 4K bytes, thus the external address bus is 12 bits.  Additional memory could be accessed by using 1 to 4 OR register pins as additional address pins.  The memory model is further divided into three sections - program, data, and stack.  By default, the program section starts at address 0x0000 and is 3K bytes in size.  By default, the data section starts at address 0x0C00 and is 768 bytes in size.  The stack section starts at address 0x0F00 and is 256 bytes in size.  Note that the program and data sections are flexible with respect to their division if more or less program space is required (of course by also adjusting the space allocated to the data space). The stack section, however, is fixed and cannot be expanded or contracted.

All of the StackCPU design's internal registers are 16 bits.  But the external data bus is only 8 bits, so when reading data from memory into any internal register, that data will typically only occupy the lower half of the internal register (unless a specific instruction takes care of loading both halves of the internal register).  The memory interface also includes a few control lines such as memory clock, output enable and write select.


Repo contents:

* docs/          - system design docs, block diagrams, etc
* examples/      - example code using the Stack CPU
* hw/            - hardware design files
* ref/           - reference documents for integrated packages
* src/           - source code for the Stack CPU tools


## Requirements

* StackCPU **shall** support TBD.


## High Level Design

### Machine Model

![model](https://github.com/dervish77/StackCPU/blob/main/docs/StackCPU-Machine-Model.png?raw=true)

### Register Model 

(all 16 bits)
```
 PC         program counter - grows up from <bottom of mem>
 SP         stack pointer - grows down from <top of mem>

 DR         data register (hidden) - points to <mem above top of program>
 AC         accumulator (hidden) - used for math operations

 IR         input register - external input
 OR         output register - external output 
 PR         print register - external output (used by print instructions)
 SR         serial register - external input (serial data input)
```

### Instruction Model

```
 OPERATION
 OPERATION <direct operand>               #dd or "c"
 OPERATION <memory address>               $hhhh
 OPERATION <label>                        &label
```

### Architecture Diagram

![architecture](https://github.com/dervish77/StackCPU/blob/main/docs/StackCPU-Architecture.png?raw=true)

### Instructions

```
(Note, unless otherwise noted all instruction end with PC + 1 -> PC)

CLS           clear the stack                             <mem top> -> SP

END           end of program (aka HALT)                   PC -> PC

NOP           no operation                                no state change

RST           reset cpu                                   0 -> AC
                                                          0 -> DR
                                                          <mem top> -> SP
                                                          0 -> PC

LDM <mem>     loads data from memory to top of stack      <mem> -> DR
                                                          M[DR] -> S[0]

LDI           increments DR, load data mem to TOS         DR + 1 -> DR
                                                          M[DR] -> S[0]

STM <mem>     stores data from top of stack to memory     <mem> -> DR
                                                          S[0] -> M[DR]

STI           increments DR, stores TOS to data mem       DR + 1 -> DR
                                                          S[0] -> M[DR]

PSH <do>      push direct data to top of stack            <do> -> AC
                                                          push AC -> S[0]

POP           pops top of stack                           pop S[0] -> AC
                                                          0 -> AC

ADD           adds top two stack values                   S[1] -> AC
                                                          AC = AC + S[0]
                                                          push AC -> S[0]

SUB           subtracts top two stack values              S[1] -> AC
                                                          AC = AC - S[0]
                                                          push AC -> S[0]

NEG           negates top of stack                        0 -> AC
                                                          AC = AC - S[0]
                                                          push AC -> S[0]

AND <do>      AND top of stack with data                  S[0] -> AC               
                                                          AC & <do> -> AC
							  push AC -> S[0]

ORR <do>      OR top of stack with data                   S[0] -> AC               
                                                          AC | <do> -> AC
							  push AC -> S[0]

INV           Invert top of stack                         S[0] -> AC
                                                          invert AC -> AC
							  push AC -> S[0]

CPE <do>      compare if top of stack is equal            <do> -> AC
                                                          if S[0] equal AC,
                                                            push 0 -> S[0]
                                                          else, 
                                                            push 1 -> S[0]

CNE <do>      compare if top of stack is not equal        <do> -> AC
                                                          if S[0] not equal AC,
                                                            push 0 -> S[0]
                                                          else, 
                                                            push 1 -> S[0]

BRZ <label>   branch if top of stack is zero              pop S[0] -> AC
                                                          if AC equal to 0, 
                                                            <label> -> PC
                                                          else,
                                                            PC + 1 -> PC

BRN <label>   branch if top of stack is not zero          pop S[0] -> AC
                                                          if AC not equal to 0,
                                                            <label> -> PC
                                                          else, 
                                                            PC + 1 -> PC

BRU <label>   branch unconditionally                      <label> -> PC


Special instructions

INP           inputs I/O to top of stack                  IR -> S[0]

OUT           outputs top stack to I/O, stack is popped   S[0] -> OR

PRT           prints top of stack, stack is popped        S[0] -> PR

SER           inputs serial to top of stack               SR -> S[0]

(Note, PR is serial output interface, SR is serial input interface)
```

[Instruction op-code details](https://github.com/dervish77/StackCPU/blob/master/docs/StackCPU-Instruction-Op-Codes.pdf)

### Examples

#### Hello

```
; prints Hello
.ORG 0x0000
.DAT 0x0C00
        CLS
        PSH "H"
        PRT
        PSH "E"
        PRT
        PSH "L"
        PRT
        PSH "L"
        PRT
        PSH "O"
        PRT
        PSH #13          ; CR
        PRT
        PSH #10          ; LF
        PRT
        END
```

#### Add Numbers from 1 to 5

```
; Adds numbers from 1 to 5, outputs sum
.ORG 0x0000
.DAT 0x0C00
        CLS
        PSH #1
        PSH #2
        ADD
        PSH #3
        ADD
        PSH #4
        ADD
        PSH #5
        ADD
        OUT
        END
```

#### Add Numbers from 1 to 10 in a Loop

```
; Adds numbers from 1 to 10 in a loop, outputs sum
.ORG 0x0000
.DAT 0x0C00
        CLS
        PSH #0
        STM $0C00        ; clear sum in memory
        NOP
        PSH #0           ; push "last" value on stack
        NOP
        PSH #1
iloop:  PSH #1           ; incr value
        ADD
        CPE #10
        BRN &iloop       ; loop to push next incr value on stack
        NOP
aloop:  LDM $0C00
        ADD
        STM $0C00        ; store sum to memory
        CPE #0
        BRN &aloop       ; loop to add next num to sum
        NOP
        LDM $0C00
        OUT              ; output sum
        END
```


## Detailed Design

### Hardware

#### Logic Blocks

##### Fetch Logic

TBD

##### Instruction Decode Logic

TBD

##### ALU Logic

TBD

##### Control SM Logic

TBD

#### Implementations

##### Emulated Implementation

The Emulated Implementation is a simulation of the StackCPU running on a Pi Pico microcontroller.  Basically this is a version of the Core Simulator noted below running on a Pi Pico.

##### FPGA Implementation

The FPGA Implementation is a HW design of the StackCPU device implemented on a TBD FPGA.

##### Discrete Implementation

The Discrete Implementation is a HW design of the StackCPU device implemented using discrete logic devices such as standard 7400 series TTL devices.


### Software

#### Simulator

The StackCPU Simulator is a simulation of the StackCPU implemented in C/C++ and Python that can be executed on Windows (via Cygwin) or Linux environments.  The simulator is divided into three parts - UI Simulator, Core Simulator, and Memory Simulator.  The UI Simulator wraps around the Core Simulator to provide a view into and control over the internals of the StackCPU device.  The Core Simulator runs the core machine model that executes StackCPU machine code.  The Memory Simulator provides an emulation of the system memory that is accessed by the StackCPU device during runtime.

![simulator](https://github.com/dervish77/StackCPU/blob/main/docs/StackCPU-SW-Simulator.png?raw=true)

##### UI Simulator

The UI simulator is a CLI interface that enables the loading of binary images into the Memory simulator, access and control over registers in the Core simulator, and control over execution of code stored in memory.  The host interface of the UI simulator is provided in two parts -- command line arguments and the CLI itself.

Command line arguments:
```
stacksim [-f filename][-m <mode>][-h][-v][-d filename]

-f filename            - load memory with data from "filename" (default is "file.bin")
-d filename            - output debug data to log "filename" (default is off)
-m <mode>              - enter <mode> on startup
                           where <mode> is 0 for idle, 1 for halt (default), 2 for run, 3 for single step
-h                     - display command arguments
-v                     - display version
```

The CLI commands:
```
l filename             - load binary "file" into memory simulator
s filename             - save memory simulator to binary "file"
d filename             - dump memory to "hex file"

r hhhh                 - read memory at addess hhhh
w hhhh dd              - write dd to memory at address hhhh
b ssss eeee            - dump memory block from ssss to eeee
f ssss eeee dd         - fill memory block from ssss to eeee with dd

g                      - go - i.e. enter "run" mode
h                      - halt - i.e. enter "halt" mode
j hhhh                 - jump to address hhhh and reset into "run" mode
k hhhh                 - jump to address hhhh and begin "single step" mode
n                      - single step to next instruction

x cc                   - read register "cc" 
y cc dddd              - write dddd to register "cc"
z                      - dump contents of all registers

t rate                 - set clock tick to rate

?                      - display CLI help

q                      - quit the simulator
```

##### Core Simulator

The Core simulator emulates the internals of the StackCPU device.  It provides interfaces for connecting to the Memory simulator (i.e. address, data, and control).  It also provides debug interfaces for connecting with the UI simulator (i.e. control over operating modes, read/write of registers, etc).

##### Memory Simulator

The Memory simulator emulates the system memory of the StackCPU system.  It provides interfaces for connecting to the Core simulator (i.e. address, data, and control).  It also provides debug interfaces for connecting with the UI simulator (i.e. loading memory from binary files, saving memory to binary files, and read/write of locations in memory).

#### Assembler Tools

The StackCPU Assembler is a set of tools (implemented in C) for compiling StackCPU assembly source code into machine code that can be executed by a StackCPU simulator or a StackCPU HW implementation.  This set of tools includes an assembler, an archiver, and a linker.

![assembler](https://github.com/dervish77/StackCPU/blob/main/docs/StackCPU-SW-Assembler.png?raw=true)

##### Assembler

##### Archiver

##### Linker







