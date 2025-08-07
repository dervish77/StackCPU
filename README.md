# Stack CPU
Custom stack-based CPU design 

StackCPU is a custom CPU design that utilizes a stack-based memory model instead of the usual memory models seen in processors such as 6502 or Z80.  The StackCPU project was undertaken mostly as an exercise in virtual CPU design that could be implemented via a simulator and utilize custom assembler tools.  The StackCPU design definition has a limited memory size (4K bytes) to simplify any hardware implementations using the specification.  The StackCPU design should be simple enough to be implemented in hardware via an FPGA or perhaps even using discrete TTL devices.

The stack-based design for StackCPU uses the stack for nearly all operations.  Thus data is constantly moved on and off the stack during normal operation.  Some instructions use a combination of the stack and the accumulator register (i.e. math and logic operations).  The data memory region is used to store and retrieve temporary variables when stack operations might result in those values being lost or destroyed.  While the accumulator register (AC) is used for arithmetic and logic operations, it is not directly accessible via any machine instructions.  Likewise the data register (DR) is used to perform transfers to and from the data memory region, the register itself is not directly accessible via any machine instructions.

The StackCPU design also includes registers for getting input from the outside world (IR) and (SR) and sending output to outside world (OR) and (PR).  The IR and OR registers are used to input and output numerical values directly, or use them to read/write control signals for external devices.  The SR and PR registers are used to input and output ascii characters to a serial terminal via an RS232 port.

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
    // prints Hello

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
        PSH #13          // CR
        PRT
        PSH #10          // LF
        PRT
        END
```

#### Add Numbers from 1 to 5

```
    // adds numbers from 1 to 5, outputs sum

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
    // adds numbers from 1 to 10 in a loop, outputs sum

        CLS
        PSH #0
        STM $0C00        // clear sum in memory
        NOP
        PSH #0           // push "last" value on stack
        NOP
        PSH #1
iloop:  PSH #1           // incr value
        ADD
        CPE #10
        BRN &iloop       // loop to push next incr value on stack
        NOP
aloop:  LDM $0C00
        ADD
        STM $0C00        // store sum to memory
        CPE #0
        BRN &aloop       // loop to add next num to sum
        NOP
        LDM $0C00
        OUT              // output sum
        END
```


## Detailed Design

### Hardware

#### TBD Implementation

The TBD Implementation is TBD.


### Software

#### Simulator

The StackCPU Simulator is TBD.

#### Assembler

The StackCPU Assembler is TBD.








