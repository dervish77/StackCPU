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
