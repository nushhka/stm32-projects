// -----------------------------------------------------------------------------------------------------
// Tarun Raj Singh - (B21CS076)                                                        Date : 20/04/2024
// -----------------------------------------------------------------------------------------------------
// I2C Communication Protocol using Bare Metal Programming --> Here I am writing the trial code to test 
// the I2C functions written in the driver file
// -----------------------------------------------------------------------------------------------------
// Here I am using MPU6050 Sensor, for the communication [ I am using the driver functions written by me
// to communicate with an I2C device, i.e, the MPU6050 sensor ]
// -----------------------------------------------------------------------------------------------------





// Including the Driver file which is written using bare metal programming for this assignment
// -----------------------------------------------------------------------------------------------------
#include "stdio.h"
#include "rccConfig.h"
#include "i2c_functions.h"
// -----------------------------------------------------------------------------------------------------



// -----------------------------------------------------------------------------------------------------
// Driver Functions to Configure and Interact with MPU 6050 using the I2C drivers made before
// -----------------------------------------------------------------------------------------------------
void MPU6050_Read_Temp(void);
void MPU_Write (uint8_t Address, uint8_t Reg, uint8_t Data);
void MPU_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size);
void MPU6050_Init (void);
void MPU6050_Read_Gyro(void);
void MPU6050_Read_Accel(void);
// -----------------------------------------------------------------------------------------------------





// -----------------------------------------------------------------------------------------------------
// Driver Functions to Configure and Interact with I2C External Display LCD using the I2C drivers 
// -----------------------------------------------------------------------------------------------------
void DISPLAY_WRITE_COMMAND(uint8_t Address, uint8_t cmd);
void DISPLAY_WRITE_DATA(uint8_t Address, uint8_t data);
void lcd_init(void);
void lcd_send_string (char *str);
void lcd_clear (void);
void lcd_put_cur(int row, int col);
// -----------------------------------------------------------------------------------------------------





// -----------------------------------------------------------------------------------------------------
// Defining the basic register values to initiate the I2C device ( MPU6050 sensor )
// -----------------------------------------------------------------------------------------------------
#define MPU6050_ADDR 0xD0 																				// Selecting address of sensor = 0xD0
#define SMPLRT_DIV_REG 0x19																				// Referred datasheet for sensor
#define GYRO_CONFIG_REG 0x1B																			// Referred datasheet for sensor
#define ACCEL_CONFIG_REG 0x1C																			// Referred datasheet for sensor
#define ACCEL_XOUT_H_REG 0x43																			// Referred datasheet for sensor
#define TEMP_OUT_H_REG 0x41																				// Referred datasheet for sensor
#define TEMP_OUT_L_REG 0x42																				// Referred datasheet for sensor
#define GYRO_XOUT_H_REG 0x3B																			// Referred datasheet for sensor
#define PWR_MGMT_1_REG 0x6B																				// Referred datasheet for sensor
#define WHO_AM_I_REG 0x75																					// Referred datasheet for sensor

static int16_t Accel_X_RAW = 0;																		// Variables to keep track of response
static int16_t Accel_Y_RAW = 0;																		// Variables to keep track of response
static int16_t Accel_Z_RAW = 0;																		// Variables to keep track of response

static int16_t Gyro_X_RAW = 0;																		// Variables to keep track of response
static int16_t Gyro_Y_RAW = 0;																		// Variables to keep track of response
static int16_t Gyro_Z_RAW = 0;																		// Variables to keep track of response

static float Ax, Ay, Az, Gx, Gy, Gz, temperature;									// Variables to keep track of response

																				          // Variables to keep track of response
// -----------------------------------------------------------------------------------------------------





// -----------------------------------------------------------------------------------------------------
// Defining the Write function using our developed driver I2C functions 
// -----------------------------------------------------------------------------------------------------
void MPU_Write (uint8_t Address, uint8_t Reg, uint8_t Data)
{
	I2C_Start ();																										// Generate the start condition
	I2C_Address (Address);																					// Send the Address to the device
	I2C_Write (Reg);																								// Send the Address of the register
	I2C_Write (Data);																								// Write the Data to that Register
	I2C_Stop ();																										// Generate the stop condition
}
// -----------------------------------------------------------------------------------------------------





// -----------------------------------------------------------------------------------------------------
// Defining the Read function using our developed driver I2C functions 
// -----------------------------------------------------------------------------------------------------
void MPU_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
	I2C_Start ();																										// Generate the start condition
	I2C_Address (Address);																					// Send the Address to the device
	I2C_Write (Reg);																								// Send the Address of the register
	I2C_Start ();  																									// Another Start to Enable Read
	I2C_Read (Address+0x01, buffer, size);											    // Reading data ( Read mode : Adress ++)
	I2C_Stop ();																										// Generate the stop condition
}
// -----------------------------------------------------------------------------------------------------




// -------------------------------------------------------------------------------------------------------------------
// Defining the Initialization function for the sensor to Switch ON the sensor and configure registers 
// -------------------------------------------------------------------------------------------------------------------

void MPU6050_Init (void)
{
	uint8_t check;
	uint8_t Data;

	MPU_Read (MPU6050_ADDR,WHO_AM_I_REG, &check, 1);              	// Check device ID from WHO_AM_I Register 
	if (check == 104)  																							// 0x68 is returned if ADDR matches Response
	{
		MPU_Write (MPU6050_ADDR, PWR_MGMT_1_REG, 0);									// Writing 0 to Power Management Reg to wake sensor
		MPU_Write(MPU6050_ADDR, SMPLRT_DIV_REG, 0x07);								// Writing 0x07 to set Data Rate = 1 KHz
		MPU_Write(MPU6050_ADDR, ACCEL_CONFIG_REG, 0x00);							// Initialize the Acceleration config Reg
		MPU_Write(MPU6050_ADDR, GYRO_CONFIG_REG, 0x00);								// Initialize the Gyroscope config Reg
	}
}
// ------------------------------------------------------------------------------------------------------------------





// -------------------------------------------------------------------------------------------------------------------
// Defining function to Read the Gyroscope Values from the sensor using I2C Protocol
// -------------------------------------------------------------------------------------------------------------------
void MPU6050_Read_Gyro(void)
{
	uint8_t Rx_data[6];																							// Register to store the Read values
	MPU_Read (MPU6050_ADDR, GYRO_XOUT_H_REG, Rx_data, 6);						// Reading 6 Bytes from the GYRO_XOUT_H_REG index
	Gx = (int16_t)(Rx_data[0] << 8 | Rx_data [1]);									// Making the Gyro X values using 0th and 1st Bytes
	Gy = (int16_t)(Rx_data[2] << 8 | Rx_data [3]);									// Making the Gyro Y values using 2nd and 3rd Bytes
	Gz = (int16_t)(Rx_data[4] << 8 | Rx_data [5]);									// Making the Gyro Z values using 4th and 5th Bytes
}
// -------------------------------------------------------------------------------------------------------------------





// -------------------------------------------------------------------------------------------------------------------
// Defining function to Read the Acceleration values from the sensor using I2C Protocol
// -------------------------------------------------------------------------------------------------------------------
void MPU6050_Read_Accel(void)
{
	uint8_t Rx_data[6];																							// Register to store the Read values
	MPU_Read (MPU6050_ADDR, ACCEL_XOUT_H_REG, Rx_data, 6);				 	// Reading 6 Bytes from the ACCEL_XOUT_H_REG index
	Accel_X_RAW = (int16_t)(Rx_data[0] << 8 | Rx_data [1]);					// Making the Accel X values using 0th and 1st Bytes
	Accel_Y_RAW = (int16_t)(Rx_data[2] << 8 | Rx_data [3]);					// Making the Accel Y values using 2nd and 3rd Bytes
	Accel_Z_RAW = (int16_t)(Rx_data[4] << 8 | Rx_data [5]);					// Making the Accel Z values using 4th and 5th Bytes
	Ax = Accel_X_RAW/16384.0;																				// Converting the Raw Ax value in units of g
	Ay = Accel_Y_RAW/16384.0;																				// Converting the Raw Ay value in units of g
	Az = Accel_Z_RAW/16384.0;																				// Converting the Raw Az value in units of g
}
// -------------------------------------------------------------------------------------------------------------------





// -------------------------------------------------------------------------------------------------------------------
// Defining function to Read the Temperature values from the sensor using I2C Protocol
// -------------------------------------------------------------------------------------------------------------------
void MPU6050_Read_Temp(void)
{
	uint8_t Rx_data_L;																							// Register to store the Read values
	uint8_t Rx_data_H;																							// Register to store the Read values
	float rawTemp;																									// Register to store the Read values
	MPU_Read (MPU6050_ADDR, TEMP_OUT_L_REG, &Rx_data_L, 1);					// Read Lower 8 bits from Temperature register
	MPU_Read (MPU6050_ADDR, TEMP_OUT_H_REG, &Rx_data_H, 1);					// Read Upper 8 bits from Temperature register
	rawTemp = (int16_t)(Rx_data_H << 8 | Rx_data_L );								// Concatenating to form the 16 bit data
	if(Rx_data_H >= 128){
		int16_t twos_complement = (int16_t)(~(uint16_t)rawTemp) + 1;	// converting to degree celcius
		temperature = (twos_complement/340)*(-1) + 36.53;
	}
	else{
		int16_t twos_complement = (int16_t)(~(uint16_t)rawTemp) + 1;	// converting to degree celcius
		temperature = (twos_complement/340) + 36.53;
	}
}
// -------------------------------------------------------------------------------------------------------------------






// -------------------------------------------------------------------------------------------------------------------
// Defining function Send a Command Code to Configure LCD using I2C Drivers made from scratch
// -------------------------------------------------------------------------------------------------------------------
void DISPLAY_WRITE_COMMAND(uint8_t Address, uint8_t cmd)
{	
	I2C_Start();                                                                  // Send the Start Codnition
	I2C_Address (Address);																												// Send the Address
	char data_u, data_l;
	uint8_t data_t[4];																														// Maaking the 32-bit command structure
	data_u = (cmd&0xf0);																													// Extracting upper 4-bits of cmd
	data_l = ((cmd<<4)&0xf0);																											// Extracting lower 4-bits of cmd
	data_t[0] = data_u|0x0C;  																										// en=1, rs=0
	data_t[1] = data_u|0x08;  																										// en=0, rs=0
	data_t[2] = data_l|0x0C;  																										// en=1, rs=0
	data_t[3] = data_l|0x08;  																										// en=0, rs=0
	I2C_WriteMulti((uint8_t *) data_t,4);																					// Writing the cmd to LCD using I2C
	I2C_Stop ();																																	// Genrating Stop Codnition
}
// -------------------------------------------------------------------------------------------------------------------





// -------------------------------------------------------------------------------------------------------------------
// Defining function Send a Character Data to Configure LCD using I2C Drivers made from scratch
// -------------------------------------------------------------------------------------------------------------------
void DISPLAY_WRITE_DATA(uint8_t Address, uint8_t data)
{	
	I2C_Start();                                                                  // Send the Start Codnition
	I2C_Address (Address);																												// Send the Address
	char data_u, data_l;
	uint8_t data_t[4];																														// Maaking the 32-bit data structure
	data_u = (data&0xf0);																													// Extracting upper 4-bits of data
	data_l = ((data<<4)&0xf0);																										// Extracting lower 4-bits of data
	data_t[0] = data_u|0x0D;  																										// en=1, rs=1
	data_t[1] = data_u|0x09;  																										// en=0, rs=1
	data_t[2] = data_l|0x0D;  																										// en=1, rs=1
	data_t[3] = data_l|0x09;  																										// en=0, rs=1
	I2C_WriteMulti((uint8_t *) data_t,4);																					// Writing the cmd to LCD using I2C
	I2C_Stop ();																																	// Genrating Stop Codnition
}
// -------------------------------------------------------------------------------------------------------------------




// -------------------------------------------------------------------------------------------------------------------
// Defining function to initialize the LCD using I2C Drivers made from scratch
// -------------------------------------------------------------------------------------------------------------------
void lcd_init(void)
{
	Delay_ms(50);  
	DISPLAY_WRITE_COMMAND (0x4E,0x30);												// Reset Condition 
	Delay_ms(5);  
	DISPLAY_WRITE_COMMAND (0x4E,0x30);												// Reset Condition 
	Delay_ms(1);  
	DISPLAY_WRITE_COMMAND (0x4E,0x30);												// Reset Condition 
	Delay_ms(10);
	DISPLAY_WRITE_COMMAND (0x4E,0x20); 												// Select 4 - bit mode (P4 - P7)
	Delay_ms(10);
	DISPLAY_WRITE_COMMAND (0x4E,0x28); 												// Set DL=0 (4 bit mode), N=1(2 line) F = 0 (5x8 chars)
	Delay_ms(1);
	DISPLAY_WRITE_COMMAND (0x4E,0x08);                        // Display ON/OFF control --> D=0,C=0,B=0 ---> OFF
	Delay_ms(1);
	DISPLAY_WRITE_COMMAND (0x4E,0x01);  											// clear display
	Delay_ms(1);
	Delay_ms(1);
	DISPLAY_WRITE_COMMAND (0x4E,0x06); 												// Entry mode set --> I/D=1 (increment cursor) & S=0 (no shift)
	Delay_ms(1);
	DISPLAY_WRITE_COMMAND (0x4E,0x0C); 												// Display on/off control --> D = 1, C and B = 0. 
}
// -------------------------------------------------------------------------------------------------------------------



// -------------------------------------------------------------------------------------------------------------------
// Defining function send a string to the LCD using I2C Drivers made from scratch
// -------------------------------------------------------------------------------------------------------------------
void lcd_send_string (char *str)
{
	while (*str) DISPLAY_WRITE_DATA (0x4E,*str++);
}
// -------------------------------------------------------------------------------------------------------------------




// -------------------------------------------------------------------------------------------------------------------
// Defining function to clear the LCD Screen
// -------------------------------------------------------------------------------------------------------------------
void lcd_clear (void)
{
	DISPLAY_WRITE_COMMAND (0x4E,0x80);
	for (int i=0; i<80; i++)
	{
		DISPLAY_WRITE_DATA (0x4E,' ');
	}
}
// -------------------------------------------------------------------------------------------------------------------



// -------------------------------------------------------------------------------------------------------------------
// Defining function to put the curson at a specific location on the screen
// -------------------------------------------------------------------------------------------------------------------
void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
        case 2:
        	col |= 0xD4;
        	break;
    }
    DISPLAY_WRITE_COMMAND (0x4E,col);
}
// -------------------------------------------------------------------------------------------------------------------



int main ()
{
	// char buffer[50] ={0};
	// char Ax_arr[13];
	// char Ay_arr[13];
	// char Az_arr[13];
	SysClockConfig ();
	TIM6Config ();
	I2C_Config ();	
	DisplayConfig();
	Display_SendString("Hello Running Program...\n\r");
	// MPU6050_Init ();
	lcd_init();
	//lcd_send_string("B21CS0479739");
	
	
	while (1)
	{
		// MPU6050_Read_Accel ();
		// //MPU6050_Read_Gyro();
		// sprintf(Ax_arr,"Ax : %.3f",Ax);
		// sprintf(Ay_arr,"Ay : %.3f",Ay);
		// sprintf(Az_arr,"Az : %.3f",Az);
		 lcd_put_cur(0,0);
		 lcd_send_string("LAB COMPLETE");
		 lcd_put_cur(1,0);
		 lcd_send_string("SUCCESS");
		 //lcd_put_cur(2,0);
		 //lcd_send_string(Az_arr);
		// //MPU6050_Read_Temp();
		// Delay_ms (500);
		//sprintf(buffer,"%s %f %s %f %s %f \n\r","Ax : ",Ax," || Ay : ", Ay, " || Az : ",Az);
		//sprintf(buffer,"%s %f %s %f %s %f \n\r","Ax : ",Gx," || Ay : ", Gy, " || Az : ",Gz);
		//sprintf(buffer,"%s %f %s \n\r","Temperature : ",temperature,"  C");
    //Display_SendString(buffer);
	}
}

