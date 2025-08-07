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
