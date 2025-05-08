    PRESERVE8
    TTL TEXT
    GLOBAL main
    AREA MYDATA, DATA, READWRITE
    ALIGN
ARRAY   DCD 1, 2, 3, 4,    ; Input 4x4 matrix stored row-wise
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
DEST    DCD 0, 0, 0, 0,    ; Output matrix initialized to 0
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    AREA MYCODE, CODE, READONLY
main
    LDR R0, =ARRAY          ; R0 points to the source matrix
    LDR R1, =DEST           ; R1 points to the destination matrix
    MOV R4, #4              ; Matrix size (4x4)

    MOV R5, #0              ; Row index (i)
ROW_LOOP
    CMP R5, R4              ; Check if row index < 4
    BGE STOP                ; Exit if all rows processed

    MOV R6, #0              ; Column index (j)
COL_LOOP
    CMP R6, R4              ; Check if column index < 4
    BGE NEXT_ROW            ; Move to the next row if all columns processed

    ; Calculate source index: i * 4 + j
    MOV R2, R5              ; R2 = i
    MUL R2, R2, R4          ; R2 = i * 4
    ADD R2, R2, R6          ; R2 = i * 4 + j
    LDR R3, [R0, R2, LSL #2]; Load ARRAY[i][j] into R3

    ; Calculate destination index: j * 4 + i
    MOV R2, R6              ; R2 = j
    MUL R2, R2, R4          ; R2 = j * 4
    ADD R2, R2, R5          ; R2 = j * 4 + i
    STR R3, [R1, R2, LSL #2]; Store into DEST[j][i]

    ADD R6, R6, #1          ; Increment column index (j)
    B COL_LOOP              ; Repeat for next column

NEXT_ROW
    ADD R5, R5, #1          ; Increment row index (i)
    B ROW_LOOP              ; Repeat for next row

STOP
    B STOP                  ; Infinite loop to stop execution
    END
