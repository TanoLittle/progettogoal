				
				AREA asm_functions, CODE, READONLY				
                EXPORT  traduzione_morse
traduzione_morse
				; save current SP for a faster access 
				; to parameters in the stack
				MOV   r12, sp
				; save volatile registers
				STMFD sp!,{r2-r8,r10-r11,lr}				
				; extract argument 4 and 5 into R4 and R5

				; arguments 
				; R0 - Vett_in[]
				; R1 - Vett_out[]
				; R2 - valore in costruzione
				; R3 - cnt elementi nel valore attuale
				; R4 - valore letto
				; R5 - conta simboli
				; R6 - valori in tabella
				; R7 - pointer a tabella
				; R8 - dato_tabella
			
				MOV R5, #0	; conta simboli
				MOV R2, #0
				MOV R3, #0
				
newValue		LDRB R4, [R0], #1

				cmp R4, #4
				beq fine_lettura
				cmp R4, #2
				beq cambio_lettera
				cmp R4, #3
				beq spazio
				add r3, r3, #1 ; SE NON CARATTERE SPECIALE INC R3
				cmp R4, #1
				beq uno
				
zero			
				LSL r2,r2,#1  ; shift left <-- 0 
				b newValue
uno				
				LSL r2,r2,#1  
				ORR r2,r2,#1	; shift left <-- 1 
				b newValue		

cambio_lettera
				cmp r3, #1
				beq t_uno
				cmp r3, #2
				beq t_due
				cmp r3, #3
				beq t_tre
				cmp r3, #4
				beq t_quattro
				cmp r3, #5
				beq t_cinque
				

spazio			
				CMP R3, #0
				subne r0,r0,#1 ; 
				BNE cambio_lettera
				MOV R8, #" "
				STRB R8, [R1], #1
				b ripartenza

t_uno
				mov r6, #2   ; due valori in tabella
				ldr r7, =table1
				b cerca_dato		
				
t_due
				mov r6, #4   ; quattro valori in tabella
				ldr r7, =table2
				b cerca_dato		

t_tre
				mov r6, #8   ; otto valori in tabella
				ldr r7, =table3
				b cerca_dato		
t_quattro
				mov r6, #12   ; dodici valori in tabella
				ldr r7, =table4
				b cerca_dato		
t_cinque
				mov r6, #10   ; dieci valori in tabella
				ldr r7, =table5
				b cerca_dato		


cerca_dato
				LDRB R8, [R7], #1
				SUB r6, r6, #1
				CMP R8,R2
				beq trovato
				cmp r6, #0
				addne R7,R7, #1 ; skip value in the table 
				bne cerca_dato
ripartenza				
				add r5,r5,#1
				MOV R2, #0
				MOV R3, #0
				b newValue
				
trovato 
				LDRB R8, [R7]
				STRB R8, [R1], #1
				b ripartenza

fine_lettura
				CMP R3, #0
				subne r0,r0,#1 ; 
				BNE cambio_lettera
				MOV R0, R5
				; restore volatile registers
				LDMFD sp!,{r2-r8,r10-r11,pc}

; 2 Valori
table1
				DCB 0,"E"
				DCB 1,"T"

; 4 valori
table2			
				DCB 2_00, "I"
				DCB 2_01, "A"
				DCB 2_10, "N"
				DCB 2_11, "M"
; 8 valori
table3
				DCB 2_000, "S"
				DCB 2_001, "U"
				DCB 2_010, "R"
				DCB 2_011, "W"
				DCB 2_100, "D"
				DCB 2_101, "K"
				DCB 2_110, "G"
				DCB 2_111, "O"

;12 Valori
table4
				DCB 2_0000, "H"
				DCB 2_0001, "V"
				DCB 2_0010, "F"
				DCB 2_0100, "L"
				DCB 2_1000, "B"
				DCB 2_1010, "C"
				DCB 2_0111, "J"
				DCB 2_0110, "P"
				DCB 2_1101, "Q"
				DCB 2_1001, "X"
				DCB 2_1011, "Y"
				DCB 2_1100, "Z"
;10 valori
table5
				DCB 2_01111, "1"
				DCB 2_00111, "2"
				DCB 2_00011, "3"
				DCB 2_00001, "4"
				DCB 2_00000, "5"
				DCB 2_10000, "6"
				DCB 2_11000, "7"
				DCB 2_11100, "8"
				DCB 2_11110, "9"
				DCB 2_11111, "0"
				
				

				

				

                END
					
					