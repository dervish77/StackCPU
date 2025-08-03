# Stack CPU
Custom stack-based CPU design 

The StackCPU design is TBD.  

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
    SP      0x0FFF |   Stack    |
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
    PC      0x0000 |            |
	                  +------------+
```

![model](https://github.com/dervish77/StackCPU/blob/master/docs/StackCPU-Machine-Model.png?raw=true)

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

TBD

### Examples

#### TBD

TBD


## Detailed Design

### Hardware

#### TBD

The TBD is TBD.

### Software

#### Simulator

The TBD coordinates TBD.

#### Assembler

The TBD coordinates TBD.












