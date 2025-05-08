        PRESERVE8
        TTL Quest3
        GLOBAL main
        AREA q3_Data, DATA, READWRITE
        ALIGN
Array   DCD 32, 80, 6, 77, 55  
Ans     DCD 0

        AREA q3_Code, CODE, READONLY

main
        LDR R0, =Array    
        LDR R1, =Ans 
        MOV R2, #5      
        MOV R3, #7      

loop    
        LDR R4, [R0], #4
        SDIV R5, R4, R3  
        MUL R6, R5, R3  
        CMP R6, R4       
        MOVEQ R7, R4    
        STR R7, [R1]     

        SUBS R2, R2, #1  
        BNE loop         

STOP    B STOP
        END
