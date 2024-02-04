; lc3 program for the halt subroutine (TRAP 0x25)

.ORIG xFD70

; save R1 and R2
ST  R1, save_r1
ST  R0, save_r0

; clear bit 15 of the master control register
LDI R1, MCR
LD  R0, MASK
AND R0, R1, R0
STI R0, MCR

; restore R1 and R0
LD R1, save_r1
LD R0, save_r0
RET

; memory
MCR		.FILL xFFFE
MASK	.FILL x7FFF
save_r0 .BLKW #1
save_r1 .BLKW #1
.END