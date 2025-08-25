; Adds numbers from 1 to 10 in a loop, outputs sum
.ORG 0x0000
.DAT 0x0C00
.EQU 0x0C00 %sum
:start  CLS
        PSH #0
        STM $0C00        ; clear sum in memory (use direct address ref)
        NOP
        PSH #0           ; push "last" value on stack
        NOP
; create list of numbers on stack
        PSH #1
:iloop  POP              ; pop the "next" number
		PSA              ; put it back on stack
		PSA              ; push a copy
		PSH #1           ; incr value
        ADD              ; add replaces top 2 stack values with sum
        CPE #10
        BRN &iloop       ; loop to push next incr value on stack
        NOP
; sum the numbers on the stack
:aloop  LDM %sum         ; use variable ref
        ADD
        STM %sum         ; store sum to memory
        CPE #0
        BRN &aloop       ; loop to add next num to sum
        NOP
; output results
        LDM $0C00
        OUT              ; output sum
        END
