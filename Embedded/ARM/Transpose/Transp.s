        PRESERVE8
        TTL TEXT
        GLOBAL main
        AREA MYDATA, DATA, READWRITE
        ALIGN
ARRAY   DCD 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16  
DEST    DCD 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0          ; Destination 

        AREA MYCODE, CODE, READONLY
main
		; Load address 
        LDR R0, =ARRAY       
        LDR R1, =DEST       
        MOV R4, #0           ; Row index (i = 0)
        MOV R5, #0           ; Column index (j = 0)

ROW_LOOP
        CMP R4, #4           ; Check if all rows are processed
        BEQ END_PROGRAM      ; Exit if done
        MOV R5, #0           ; Reset column index for each row

COL_LOOP
        CMP R5, #4           ; Check if all columns are processed
        BEQ NEXT_ROW         ; Move to the next row if done

        ; Calculate source address: (i * 4 + j) * 4
        MOV R6, R4
        LDR R9, =4           ; Load multiplier value
        MUL R6, R6, R9       ; R6 = i * 4
        ADD R6, R6, R5       ; R6 = i * 4 + j
        LSL R6, R6, #2       ; R6 = (i * 4 + j) * 4
        ADD R6, R6, R0       ; R6 = &ARRAY[i][j]
        LDR R7, [R6]         ; Load ARRAY[i][j]

        ; Calculate destination address: (j * 4 + i) * 4
        MOV R8, R5
        MUL R8, R8, R9       ; R8 = j * 4
        ADD R8, R8, R4       ; R8 = j * 4 + i
        LSL R8, R8, #2       ; R8 = (j * 4 + i) * 4
        ADD R8, R8, R1       ; R8 = &DEST[j][i]
        STR R7, [R8]         ; Store value in transposed position

        ADD R5, R5, #1       ; Increment column index (j++)
        B COL_LOOP           ; Repeat column loop

NEXT_ROW
        ADD R4, R4, #1       ; Increment row index (i++)
        B ROW_LOOP           ; Repeat row loop

END_PROGRAM
		; Infinite loop to stop the program
        B END_PROGRAM        
        END
