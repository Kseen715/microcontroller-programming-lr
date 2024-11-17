$MOD51

	jmp start	

org 0bh							    ; Прерывание таймера по переполнению
	clr tcon.4					    ; Запретить работу таймера T/C0
	reti						    ; Возврат из прерывания

org 20h

start:
	clr c						    ; Очистить счетчик
	mov tmod,#01h				    ; Установить режим 1 для таймера T/C0
	setb ie.7					    ; Разрешить прерывания (общее разрешение)
	setb ie.1					    ; Разрешить прерывания по переполнению 
                                    ; счетчика

	mov p2,#0h					    ; Подать низкий сигнал на все пины порта 2
                                    ; (светодиоды выключены)
	
main_loop:	
	mov r0,#20					    ; Таймер на 1с (S loop) #20
	mov r1,#10					    ; Таймер на 0,5с (K loop) #10
	mov r3,#80					    ; Таймер на 120с (T loop) #80

twomin_t:
	
	mov a,r0
	mov r4,a
	mov p2,#00100000b
second_t:
	mov TL0, #low(not(50000-1))     ; 50 ms (50000)
	mov TH0, #high(not(50000-1))    ;
	setb tcon.4					    ; Включить таймер T/C0

wait_1:
	jnb tcon.5, wait_1			    ; Цикл до переполнения
	djnz r4,second_t

	mov a,r1
	mov r4,a
	anl p2,#0h
halfsec_t:
	mov TL0, #low(not(50000-1))     ; 50 ms (50000)
	mov TH0, #high(not(50000-1))    ;
	setb tcon.4					    ; Включить таймер T/C0

wait_2:
	jnb tcon.5, wait_2			    ; Цикл до переполнения
	djnz r4,halfsec_t

	djnz r3,twomin_t
	
	jmp main_loop				    ; Основной цикл

END