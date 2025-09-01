; basic unit test of coreSim
.ORG 0x0000
.DAT 0x0C00
:start  CLS
        PSH #34			; push two values on stack
        PSH #85
        STM $0C00		; store them to data memory
        STI
        LDM $0C00		; load them back to stack
        LDI
        ADD				; add them
        OUT				; output sum
        END
