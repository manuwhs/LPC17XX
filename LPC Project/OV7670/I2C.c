#include "LPC17xx.h"			/* LPC17xx Peripheral Registers */
#include "i2c.h"
#include "../UART/uart.h" 
char  *frase = "Un registro configurado con éxito\n\r";
volatile uint32_t I2CMasterState = I2CSTATE_IDLE;
volatile uint32_t I2CSlaveState = I2CSTATE_IDLE;

volatile uint8_t I2CMasterBuffer[Master_Buffer_BUFSIZE];
volatile uint8_t I2CSlaveBuffer[Slave_Buffer_BUFSIZE];
volatile uint32_t I2CReadLength;
volatile uint32_t I2CWriteLength;

volatile uint32_t RdIndex = 0;
volatile uint32_t WrIndex = 0;

volatile uint8_t I2C_mode = SCCB;

/* The next to variables are used to stablish a way of writting in the OV7670 registers without
stopping the whole program without modifying the current way too much*/
volatile uint8_t SCCB_WriteQueue[200];  // Buffer to put the writting information
volatile uint8_t numWriteQueue = 0;		// Number of write operations
/*****************************************************************************
** Function name:		I2C_IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with master mode only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C0_IRQHandler(void) {
	uint8_t StatValue;

	/* this handler deals with master read and master write only */
	StatValue = LPC_I2C0->I2STAT;
	switch ( StatValue )
	{
	case 0x08:
		/*
		 * A START condition has been transmitted.
		 * We now send the slave address and initialize
		 * the write buffer
		 * (we always start with a write after START+SLA)
		 */
		WrIndex = 0;
		RdIndex = 0;
		LPC_I2C0->I2DAT = I2CMasterBuffer[WrIndex++];
		// LPC_I2C0->I2CONCLR = I2CONSET_AAC;	 //This is done to give ACK when we read
		I2CMasterState = I2CSTATE_PENDING;
		LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);

		break;
	
	case 0x10:
		/*
		 * A repeated START condition has been transmitted.
		 * Now a second, read, transaction follows so we
		 * initialize the read buffer.
		 */
		RdIndex = 0;
		/* Send SLA with R bit set, */
		LPC_I2C0->I2DAT = I2CMasterBuffer[WrIndex++];
		//LPC_I2C0->I2CONSET = I2CONSET_AA;
		LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	break;
	
	case 0x18:
		/*
		 * SLA+W has been transmitted; ACK has been received.
		 * We now start writing bytes.
		 */
		LPC_I2C0->I2DAT = I2CMasterBuffer[WrIndex++];
		//LPC_I2C0->I2CONSET = I2CONSET_AA;
		LPC_I2C0->I2CONCLR = I2CONCLR_SIC;
		break;

	case 0x20:
		/*
		 * SLA+W has been transmitted; NOT ACK has been received.
		 * Send a stop condition to terminate the transaction
		 * and signal I2CEngine the transaction is aborted.
		 */
		if (I2C_mode == SCCB) {	/* In SCCB might not get ACK (We get Dont Care Bit) */
		   LPC_I2C0->I2DAT = I2CMasterBuffer[WrIndex++];  	
			//LPC_I2C0->I2CONSET = I2CONSET_AA;
		}
		else{
			LPC_I2C0->I2CONCLR = I2CONCLR_SIC;
			LPC_I2C0->I2CONSET = I2CONSET_STO;
			I2CMasterState = I2CSTATE_SLA_NACK;
		}
		LPC_I2C0->I2CONCLR = I2CONCLR_SIC;
		break;

	case 0x28:
		/*
		 * Data in I2DAT has been transmitted; ACK has been received.
		 * Continue sending more bytes as long as there are bytes to send
		 * and after this check if a read transaction should follow.
		 */
		if ( WrIndex < I2CWriteLength )
		{
			/* Keep writing as long as bytes avail */
			LPC_I2C0->I2DAT = I2CMasterBuffer[WrIndex++];
		}
		else
		{
			if ( I2CReadLength != 0 )
			{
				/* Send a Repeated START to initialize a read transaction */
				/* (handled in state 0x10)                                */
				LPC_I2C0->I2CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
			}
			else
			{
				I2CMasterState = I2CSTATE_ACK;
				LPC_I2C0->I2CONSET = I2CONSET_STO;      /* Set Stop flag */
			}
		}
		LPC_I2C0->I2CONCLR = I2CONCLR_SIC;
		break;

	case 0x30:
		/*
		 * Data byte in I2DAT has been transmitted; NOT ACK has been received
		 * Send a STOP condition to terminate the transaction and inform the
		 * I2CEngine that the transaction failed.
		 */
		if (I2C_mode == SCCB) {	/* In SCCB might not get ACK (We get Dont Care Bit) */
			if ( WrIndex < I2CWriteLength )
			{
				/* Keep writing as long as bytes avail */
				LPC_I2C0->I2DAT = I2CMasterBuffer[WrIndex++];
			}
			else{
				if ( I2CReadLength != 0 ){
					/* Send a Repeated START to initialize a read transaction */
					/* (handled in state 0x10)                                */
					LPC_I2C0->I2CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
				}
				else{
					I2CMasterState = I2CSTATE_ACK;
					LPC_I2C0->I2CONSET = I2CONSET_STO;      /* Set Stop flag */
				}
			}
		}
		else {
			LPC_I2C0->I2CONSET = I2CONSET_STO;
			I2CMasterState = I2CSTATE_NACK;
		}
		LPC_I2C0->I2CONCLR = I2CONCLR_SIC;
		break;


	case 0x38:
		/*
		 * Arbitration loss in SLA+R/W or Data bytes.
		 * This is a fatal condition, the transaction did not complete due
		 * to external reasons (e.g. hardware system failure).
		 * Inform the I2CEngine of this and cancel the transaction
		 * (this is automatically done by the I2C hardware)
		 */
		I2CMasterState = I2CSTATE_ARB_LOSS;
		LPC_I2C0->I2CONCLR = I2CONCLR_SIC;

		break;


	   // READING  ************************************************************************************



	case 0x40:
		/*
		 * SLA+R has been transmitted; ACK has been received.
		 * Initialize a read.
		 * Since a NOT ACK is sent after reading the last byte,
		 * we need to prepare a NOT ACK in case we only read 1 byte.
		 */
		if ( I2CReadLength == 1 )
		{
			/* last (and only) byte: send a NACK after data is received */
			LPC_I2C0->I2CONCLR = I2CONCLR_AAC;
		}
		else
		{
			/* more bytes to follow: send an ACK after data is received */
			LPC_I2C0->I2CONSET = I2CONSET_AA;
		}
		LPC_I2C0->I2CONCLR = I2CONCLR_SIC;
		break;

	case 0x48:
		/*
		 * SLA+R has been transmitted; NOT ACK has been received.
		 * Send a stop condition to terminate the transaction
		 * and signal I2CEngine the transaction is aborted.
		 */
		LPC_I2C0->I2CONSET = I2CONSET_STO;
		I2CMasterState = I2CSTATE_SLA_NACK;
		LPC_I2C0->I2CONCLR = I2CONCLR_SIC;
		break;

	case 0x50:
		/*
		 * Data byte has been received; ACK has been returned.
		 * Read the byte and check for more bytes to read.
		 * Send a NOT ACK after the last byte is received
		 */
		I2CSlaveBuffer[RdIndex++] = LPC_I2C0->I2DAT;
		if ( RdIndex < (I2CReadLength-1) )
		{
			/* lmore bytes to follow: send an ACK after data is received */
			LPC_I2C0->I2CONSET = I2CONSET_AA;
		}
		else
		{
			/* last byte: send a NACK after data is received */
			LPC_I2C0->I2CONCLR = I2CONCLR_AAC;
		}
		LPC_I2C0->I2CONCLR = I2CONCLR_SIC;
		break;
	
	case 0x58:
		/*
		 * Data byte has been received; NOT ACK has been returned.
		 * This is the last byte to read.
		 * Generate a STOP condition and flag the I2CEngine that the
		 * transaction is finished.
		 */
		I2CSlaveBuffer[RdIndex++] = LPC_I2C0->I2DAT;
		I2CMasterState = I2CSTATE_ACK;
		LPC_I2C0->I2CONSET = I2CONSET_STO;	/* Set Stop flag */
		LPC_I2C0->I2CONCLR = I2CONCLR_SIC;	/* Clear SI flag */
		break;

	
	default:
		LPC_I2C0->I2CONSET = I2CONSET_STO;
		tx_cadena_UART0("Situacion desconocida I2C\n\r");

		LPC_I2C0->I2CONCLR = I2CONCLR_SIC;
	break;
  }
  return;
}



 /****************   I2C  INIT  *************** */

void Init_I2C0( void ) {

  	// POWER IT UP 
 	LPC_SC->PCONP |= (1 << 7);
	 	
	// Set frecuency
	LPC_SC->PCLKSEL0 &= ~(3 << 14) ; /* Select CLK/4 = 25 MHz*/

  	// Set pins SDA0 on PIO0.27 and SCL0 on PIO0.28
	LPC_PINCON->PINSEL1 &= ~((0x03<<22)|(0x03<<24));	 /* We clear the bits */
  	LPC_PINCON->PINSEL1 |= ((0x01<<22)|(0x01<<24));	 /* Set them to '01' */
 
	 // We dont need to configure the pins as not pull up/down or opendrain coz they already
	 // are this way by default and cant be changed

  	// Set the control register to 0 to desconfigure it
  	LPC_I2C0->I2CONCLR |= (0x1F << 2);  /* 1111100 */

 	// SET SCL frecuency and duty when it's a master

  	LPC_PINCON->I2CPADCFG &= ~((0x1<<0)|(0x1<<2));   /* Set the Stardad Mode */
  	LPC_I2C0->I2SCLL   = I2SCLL_SCLL;		/* Number of CLK cycles SCL is '0' */
  	LPC_I2C0->I2SCLH   = I2SCLH_SCLH;		/* Number of CLK cycles SCL is '1' */

  // Enable interrupt and set it's priority
	NVIC_SetPriority(I2C0_IRQn,0x04); 	/* Priority of the interrupt*/
  	NVIC_EnableIRQ(I2C0_IRQn);		   /* I2C0 interrupt enable */

   // Activamos el dispositivo I2C
	LPC_I2C0->I2CONSET |= (1 << 6);

}

