; Basic lc3 program.

; Semi-colons are used to create comments.

.ORIG x3000 ; Program begins at x3000.

	LEA R0, prompt1
	PUTS
	
	HALT ; The program is done, all objectives met.
	
prompt1	.STRINGZ "Hello World!"	; String variable.
	
.END   