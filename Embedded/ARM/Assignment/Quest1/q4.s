        PRESERVE8
        TTL Quest4
        GLOBAL main
        AREA q4_Data, DATA, READWRITE
        ALIGN
String   DCB "racecar", 0  
Ans      DCD 0           

        AREA q4_Code, CODE, READONLY

main
        LDR R0, =String     
        MOV R1, R0       

get_end
        LDRB R2, [R1], #1
        CMP R2, #0       
        BNE get_end     
        SUB R1, R1, #2  

check_pal
        CMP R0, R1       
        BGE is_pal 

        LDRB R2, [R0], #1
        LDRB R3, [R1], #-1
        CMP R2, R3       
        BNE not_pal
        B check_pal

is_pali
        MOV R4, #1       
        B store_result   

not_pal
        MOV R4, #0       

store_ans
        LDR R5, =Ans
        STR R4, [R5]     

STOP    B STOP
        END
