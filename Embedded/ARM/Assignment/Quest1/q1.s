        PRESERVE8
        TTL Quest1
        GLOBAL main
        AREA q1_Data, DATA, READWRITE
        ALIGN
No1     DCD 35            
No2     DCD 38          
Ans     DCD 00            

        AREA q1_Code, CODE, READONLY

main
        LDR R0, =No1    
        LDR R1, [R0]      
        LDR R0, =No2    
        LDR R2, [R0]      
        CMP R1, R2        
        MOVGT R3, R1     
        MOVLE R3, R2      
        LDR R0, =Ans   
        STR R3, [R0]     

STOP    B STOP
        END
