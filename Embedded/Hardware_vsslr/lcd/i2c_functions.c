// -----------------------------------------------------------------------------------------------------
// Tarun Raj Singh - (B21CS076)                                                        Date : 20/04/2024
// -----------------------------------------------------------------------------------------------------
// I2C Communication Protocol using Bare Metal Programming --> Here I am writing the driver functions 
// that will ease the use of I2C protocol 
// -----------------------------------------------------------------------------------------------------
// Here I am using Pins :       (PB8 --> SCL)        and           (PB9 --> SDA)
// -----------------------------------------------------------------------------------------------------




#include "i2c_functions.h"
#include "rccConfig.h"



void I2C_Config (void)
{
	// -------------------------------------------------------------------------------------------
	// Configuring RCC Enable clock for GPIO and I2C
	// -------------------------------------------------------------------------------------------
	RCC->APB1ENR |= (1<<21);  									// Enable I2C CLOCK
	RCC->AHB1ENR |= (1<<1);   									// Enable GPIOB CLOCK
	// -------------------------------------------------------------------------------------------
	
	
	
	// -------------------------------------------------------------------------------------------
	// Configure the I2C PINs for ALternate Functions
	// -------------------------------------------------------------------------------------------
	GPIOB->MODER |= (2<<16) | (2<<18);  				// Alternate Function mode for PB8 & PB9
	GPIOB->OTYPER |= (1<<8) | (1<<9);  					// Selecting Open Drain Mode for Both the PINS
	GPIOB->OSPEEDR |= (3<<16) | (3<<18);  				// Selecting High Speed for Both PINS
	GPIOB->PUPDR |= (1<<16) | (1<<18);  				// Pul Up Mode because I2C operate in Pul-up state
	GPIOB->AFR[1] |= (4<<0) | (4<<4); 	 				// Selecting AF4 for PB8 & PB9
	// -------------------------------------------------------------------------------------------
	
	
	
	// -------------------------------------------------------------------------------------------
	// Reseting CR1 and Setting PCLK1 info in CR2 and configuring the Clock Control Registers
	// -------------------------------------------------------------------------------------------
	I2C1->CR1 |= (1<<15);                       					// Reseting CR1 (SWRST bit)
	I2C1->CR1 &= ~(1<<15);											// Reseting CR1
	I2C1->CR2 |= (45<<0);  											// PCLK1 FREQUENCY in MHz
	I2C1->CCR = 225<<0;  											// Check calculation in Report
	I2C1->TRISE = 46;  												// Check Report for calculations
	// -------------------------------------------------------------------------------------------
	I2C1->CR1 |= (1<<0);  											// Enable I2C
	
}




void I2C_Start (void)
{
	// -------------------------------------------------------------------------------------------
	// Enabling the ACK bit and Enable the start bit SB
	// -------------------------------------------------------------------------------------------
	I2C1->CR1 |= (1<<10);  											// Enable the ACK
	I2C1->CR1 |= (1<<8);  											// Generate START
	while (!(I2C1->SR1 & (1<<0)));  								// Wait fror SB(Start Bit) bit to set
	// -------------------------------------------------------------------------------------------
}




void I2C_Write (uint8_t data)
{
	// -------------------------------------------------------------------------------------------
	// Writing the DATA into Data Register if Transmission Register Empty
	// -------------------------------------------------------------------------------------------
	while (!(I2C1->SR1 & (1<<7)));  						// Waiting for TXE bit --> Transmit Register Empty
	I2C1->DR = data;										// Write the Data into DR (Data Register)
	while (!(I2C1->SR1 & (1<<2)));  						// Waiting for BTF bit to set
	// -------------------------------------------------------------------------------------------
}




void I2C_Address (uint8_t Address)
{	
	// -------------------------------------------------------------------------------------------
	// Sending Slave Address & the update is reflected in ADDR bit, finally clear the ADDR
	// -------------------------------------------------------------------------------------------
	I2C1->DR = Address;  									// Sending Address by putting it in Data Register
	while (!(I2C1->SR1 & (1<<1)));  						// Waiting till address is transmitted
	uint8_t temp = I2C1->SR1 | I2C1->SR2;  					// Clear the ADDR by reading SR1 & SR2 (Manual)
	// -------------------------------------------------------------------------------------------
}
	



void I2C_Stop (void)
{
	// -------------------------------------------------------------------------------------------
	// Stop the current message transmission by enabling the stop bit in CR1
	// -------------------------------------------------------------------------------------------
	I2C1->CR1 |= (1<<9);  											// Stop I2C
	// -------------------------------------------------------------------------------------------
}




void I2C_WriteMulti (uint8_t *data, uint8_t size)
{	
	// -------------------------------------------------------------------------------------------
	// For Writing More Bytes (More Data Frames are required), iteratively send like above
	// -------------------------------------------------------------------------------------------
	while (!(I2C1->SR1 & (1<<7)));             	// Waiting for TXE bit --> Transmit Register Empty
	while (size)
	{
		while (!(I2C1->SR1 & (1<<7)));  					// Waiting for TXE bit --> Transmit Register Empty 
		I2C1->DR = (uint32_t )*data++;  					// Write the Data into DR (Data Register) (OAT)
		size--;																		// Decrement Remaining Size
	}
	while (!(I2C1->SR1 & (1<<2))); 	 						// Waiting for BTF bit to set
	// -------------------------------------------------------------------------------------------
}





void I2C_Read (uint8_t Address, uint8_t *buffer, uint8_t size)
{
	int remaining = size;                       // Size of Data to be Read (In Bytes or Dataframes)
	
	
	if (size == 1)
	{
		// ----------------------------------------------------------------------------------------------
		// If only 1 dataframe need to be read (Simply send address and recive ack, wait for Data & Read)
		// ----------------------------------------------------------------------------------------------	
		I2C1->DR = Address;  											// Send the address for the Requested Device
		while (!(I2C1->SR1 & (1<<1)));  					// Waiting till address is transmitted
		I2C1->CR1 &= ~(1<<10);  									// Clear the ACK bit recieved
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  	 	// Clear the ADDR by reading SR1 & SR2 ( Manual EV6 )
		I2C1->CR1 |= (1<<9);  										// Stop I2C --> Because Now the other device will send
		while (!(I2C1->SR1 & (1<<6)));  					// Wait till the data is recieved (RxNE-->1)
		buffer[size-remaining] = I2C1->DR;  			// Read the data from the DR -> DATA REGISTER
		// ----------------------------------------------------------------------------------------------
	}
		
	else 
	{
		// ----------------------------------------------------------------------------------------------
		// If Data consists of more than 2 frames (We need to do seperate task by sending ACK of the data)
		// ----------------------------------------------------------------------------------------------	
		I2C1->DR = Address;  											// Send the address for the Requested Device
		while (!(I2C1->SR1 & (1<<1)));  					// Waiting till address is transmitted
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  		// Clear the ADDR by reading SR1 & SR2 ( Manual EV6 )
		while (remaining>2)                       // Reading till the second last dataframe (BYTE)
		{
			while (!(I2C1->SR1 & (1<<6)));  				// Wait till the data is recieved (RxNE-->1)
			buffer[size-remaining] = I2C1->DR;  		// Read the data from the DR -> DATA REGISTER			
			I2C1->CR1 |= 1<<10;  										// Set the ACK bit to Acknowledge the data received
			remaining--;
		}
		// ----------------------------------------------------------------------------------------------	
		
		
		
		// ----------------------------------------------------------------------------------------------
		// For Reading the second last DataFrame ACK is transferred to NACK (Acc to protocol)
		// ----------------------------------------------------------------------------------------------
		while (!(I2C1->SR1 & (1<<6)));  					// Wait till the data is recieved (RxNE-->1)
		buffer[size-remaining] = I2C1->DR;  			// Read the data from the DR -> DATA REGISTER	
		I2C1->CR1 &= ~(1<<10);  									// Send NACK (or disbale the ACK bit) 
		I2C1->CR1 |= (1<<9);  										// Stop I2C
		remaining--;
		// ----------------------------------------------------------------------------------------------
		
		
		// ----------------------------------------------------------------------------------------------
		// Reading the last DataFrame but no ACK is required this time
		// ----------------------------------------------------------------------------------------------
		while (!(I2C1->SR1 & (1<<6)));  					// Wait till the data is recieved (RxNE-->1)
		buffer[size-remaining] = I2C1->DR;  			// Read the data from the DR -> DATA REGISTER	
		// ----------------------------------------------------------------------------------------------
	}	
}













// ----------------------------------------------------------------------------------------------
// Display Function for showing the recieved data on Serial Terminal Monitor
// ----------------------------------------------------------------------------------------------
void DisplayConfig(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHB1ENR |= (1<<0); 
	GPIOA->MODER |= (1<<21);  
	GPIOA->MODER |= (1<<19);  
	GPIOA->AFR[1] |= (7<<4);  
	GPIOA->AFR[1] |= (7<<8);
	USART1->CR1 = 0x00;  
	USART1->CR1 |= (1<<13);  	
	USART1->CR1 &= ~(1<<12); 
	USART1->CR2 =0x00;	
	USART1->BRR	 =0x249F; 	
	USART1->CR1 |= (1<<2); 
	USART1->CR1 |= (1<<3);  
}
// ----------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------
// Display Function for printing 1 Character to the serial Terminal
// ----------------------------------------------------------------------------------------------
void Display_SendChar(uint8_t c)
{
	USART1->DR = c; 
	while (!(USART1->SR & (1<<7)));  
}
// ----------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------
// Display Function for printing multiple Character to the serial Terminal
// ----------------------------------------------------------------------------------------------
void Display_SendString (char *string)
{
	while (*string) Display_SendChar(*string++);
}
// ----------------------------------------------------------------------------------------------













// ----------------------------------------------------------------------------------------------
// Timer configuration function for generating delays required for the program execution
// ----------------------------------------------------------------------------------------------
void TIM6Config (void)
{
	RCC->APB1ENR |= (1<<4);  													// Enable the RCC for TIM6 (General Timer)
	TIM6->PSC = 180-1;  																// 90MHz/90 = 1 MHz ~~ 1 uS delay
	TIM6->ARR = 0xffff;  															// MAX ARR value to generate any delay
	TIM6->CR1 |= (1<<0); 															// Enable the Timer Counter
	while (!(TIM6->SR & (1<<0)));  										// UIF: Update interrupt flag.
}
// ----------------------------------------------------------------------------------------------




// ----------------------------------------------------------------------------------------------
// Timer function for generating delays required in microseconds
// ----------------------------------------------------------------------------------------------
void Delay_us (uint16_t us)
{
	TIM6->CNT = 0;
	while (TIM6->CNT < us);
}
// ----------------------------------------------------------------------------------------------




// ----------------------------------------------------------------------------------------------
// Timer function for generating delays required in milliseconds
// ----------------------------------------------------------------------------------------------
void Delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		Delay_us (1000); 
	}
}
// ----------------------------------------------------------------------------------------------