# Stack CPU
Custom stack-based CPU design 

StackCPU is a custom CPU design that utilizes a stack-based memory model instead of the usual memory models seen in processors such as 6502 or Z80.  The StackCPU design definition has a limited memory size (4K bytes) to simplify the implementation of any custom hardware designs using the specification.  StackCPU was undertaken mostly as an exercise in virtual CPU design that would be implemented via a simulator and perhaps utilizing custom assembler tools.

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

```
              memory
                |
                |
      program--CPU--stack
                |
                |
         program counter (PC)


                   +------------+
    SP      0x0FFF |   Stack    |  mem top
                   |            |
            0x0F00 |------------|
                   |    Data    |
                   |            |
    DR      0x0C00 |------------|
                   |            |
                   |            |
                   |            |
                   |  Program   |
                   |            |
                   |            |
    PC      0x0000 |            |  mem bottom
                   +------------+
```

![model](https://github.com/dervish77/StackCPU/blob/main/docs/StackCPU-Machine-Model.png?raw=true)

### Register Model 

(all 16 bits)
```
 PC         program counter - grows up from <bottom of mem>
 SP         stack pointer - grows down from <top of mem>

 DR         data register (hidden) - points to <mem above top of program>
 AC         accumulator (hidden) - used for math operations

 INP        input register - external input
 OUT        output register - external output 
 PRT        print register - external output (used by print instructions)
```

### Instruction Model

```
 OPERATION
 OPERATION <direct operand>               #dd or "c"
 OPERATION <memory address>               $hhhh
 OPERATION <label>                        &label
```

### Architecture Diagram

![architecture](https://github.com/dervish77/StackCPU/blob/master/docs/StackCPU-Architecture.png?raw=true)

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
                                                            push 1 -> S[0]
                                                          else, 
                                                            push 0 -> S[0]

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

INP           inputs to top of stack                      <inp> -> S[0]

OUT           outputs top of stack, stack is popped       S[0] -> <out>

PRT           prints top of stack, stack is popped        S[0] -> <prt>

PRD           prints current stack depth                  depth of S -> <prt>

(Note, <print> is std output interface)
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
    // adds numbers from 1 to 5, prints sum

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
        PRT
        END
```

#### Add Numbers from 1 to 10 in a Loop

```
    // adds numbers from 1 to 10 in a loop, prints sum

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
        PRT              // print sum
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



