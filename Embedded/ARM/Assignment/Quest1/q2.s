        PRESERVE8
        TTL Quest2
        GLOBAL main
        AREA q2_Data, DATA, READWRITE
        ALIGN
Size    DCD 8
Array   DCD 14, 20, 6, 8, 5, 90, 3, 6
Ans     DCD 0x7FFFFFFF  

        AREA q2_Code, CODE, READONLY

main
        LDR R0, =Array    
        LDR R1, =Ans    
        LDR R2, =Size
        LDR R2, [R2]    

loop    
        LDR R3, [R0], #4
        LDR R4, [R1]     
        CMP R3, R4       
        MOVLT R4, R3    
        STR R4, [R1]     
        SUBS R2, R2, #1  
        BNE loop         

STOP    B STOP
        END
