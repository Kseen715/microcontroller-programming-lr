$MOD51

	jmp start	

org 0bh							; timer overflow interrupt
	clr tcon.4					; forbid count of timer T/C0
	reti						; return from interrupt

org 20h

start:
	clr c						; clear counter
	mov tmod,#01h				; set 1st mode for timer T/C0
	setb ie.7					; main interrept allow
	setb ie.1					; timer OF interrupt allow

	mov p2,#0h					; set full port p2 to LOW
	
main_loop:	
	mov r0,#1					; 1s timer (S loop) #20
	mov r1,#1					; 0,5s timer (K loop) #10
	mov r3,#5					; 120s timer (T loop) #80

twomin_t:
	
	mov a,r0
	mov r4,a
	mov p2,#00100000b
second_t:
	mov TL0, #low(not(50000-1)) ; 50 ms (50000)
	mov TH0, #high(not(50000-1));
	setb tcon.4					; turn on timer T/C0

wait_1:
	jnb tcon.5, wait_1			; loop until oveflow
	djnz r4,second_t

	mov a,r1
	mov r4,a
	anl p2,#0h
halfsec_t:
	mov TL0, #low(not(50000-1)) ; 50 ms (50000)
	mov TH0, #high(not(50000-1));
	setb tcon.4					; turn on timer T/C0

wait_2:
	jnb tcon.5, wait_2			; loop until oveflow
	djnz r4,halfsec_t

	djnz r3,twomin_t
	
	jmp main_loop				; main loop

END