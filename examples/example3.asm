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
