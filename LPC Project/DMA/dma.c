
#include "LPC17xx.h"                   
#include "../DAC/dac.h"
#include "dma.h"
#include "../UART/uart.h"

volatile uint32_t DMATCCount = 0;
volatile uint32_t DMAErrCount = 0;
volatile uint32_t DACDMA0Done = 0;
volatile uint32_t DACDMA1Done = 0;

volatile uint32_t Src_DAC_DMA = 0;
volatile uint32_t Dst_DAC_DMA = 0;

	LLIO onda;
	LLIO audio_clip[10];	 // Max 2^12*(10+1) = 45056 samples of audio

// char cado2[10];
void DMA_IRQHandler(void) {
 // This interrupt will be trigerred whenever the tranferSize reaches 0 (end of a Burst of FFF-1 words max)
  uint32_t regVal;
  // tx_cadena_UART0("X");

/*  conversor_IaS(LPC_GPDMACH0->DMACCControl&0xFFF, cado2);
  tx_cadena_UART0(cado2);			
  tx_cadena_UART0("|");  
 */	
  regVal = LPC_GPDMA->DMACIntStat;
  if ( regVal )
  {
	DMATCCount++;
	LPC_GPDMA->DMACIntTCClear = regVal;
	if ( regVal & 0x01 )
	{
	  DACDMA0Done = 1;
	}
	else if ( regVal & 0x02 )
	{
	  DACDMA1Done = 1;
	}
  } 

  regVal = LPC_GPDMA->DMACIntErrStat;
  if ( regVal )
  {
	DMAErrCount++;
	LPC_GPDMA->DMACIntErrClr = regVal;
  }
  return;

}

/******************************************************************************
* Function Name  : DMA_Init
* Description    : It Configures the DMA
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DMA_Init( void ) {
  /* Enable CLOCK into GPDMA controller */
  LPC_SC->PCONP |= (1 << 29);

  /* sync enabled */
  LPC_GPDMA->DMACSync = 0xFFFF;

  LPC_SC->DMAREQSEL = 0x0000;

  LPC_GPDMA->DMACIntTCClear = 0x03;			 // Clear channels 0 and 1
  LPC_GPDMA->DMACIntErrClr = 0x03;			 // Clear channels 0 and 1

  LPC_GPDMA->DMACConfig = 0x01;	/* Enable DMA channels, little endian */
  // while ( !(LPC_GPDMA->DMACConfig & 0x01) );

  NVIC_EnableIRQ(DMA_IRQn);				// Enable DMA interrupct


  return;
}

/******************************************************************************
* Function Name  : DMA_DAC_Channel_Init
* Description    : Configures a DMA channel to reproduce a given signal from an array given by the
				   global variable Dst_DAC_DMA into the DAC.
				   The signal reproduces in a loop for ever.
				   It a Memory to Pheripherial transaction, word-long (32 bits), burst = 1, N = buffer size
				   Interrupt Enabled after every transaction.
				   To Stop the sound change the DMACCLLI to 0
* Input          : - ChannelNum: Channel for the transaction (Actually only configured for channel 0)
* Output         : None
* Return         : None
* Attention		 : It assumes that the buffer where the signal is stored is smaller than the maximum number 
				   of transactions a single LILO element can hold (4095)
*******************************************************************************/

/* Operacion Memory to Pheripherial */
uint32_t DMA_DAC_Channel_Init( uint32_t ChannelNum ){
  if ( ChannelNum == 0 ) {
	DACDMA0Done = 0;			/* Clear DMA channel 0 terminal count. */
	

 
  Src_DAC_DMA =(uint32_t) &buffer_DAC;
  Dst_DAC_DMA = (uint32_t) &(LPC_DAC->DACR);		// This is the value of the DACR -> 0x4008 C000

	Create_DAC_LLIO();		// Create the LLIO that calls itself so that the sound never stops

	LPC_GPDMA->DMACIntTCClear = 0x01;	  // Clear channel 0 requests
	LPC_GPDMA->DMACIntErrClr = 0x01;	  // Clear channel 0 errors
	LPC_GPDMACH0->DMACCControl = 0;		  // Initialice registers to default 0 to configure afterwards
	LPC_GPDMACH0->DMACCConfig = 0;   
	LPC_GPDMACH0->DMACCLLI = (uint32_t)&onda;;	 // We give it a LLIO that calls itsenlf in an infinite loop

	  /* Ch0 set for M2P transfer from mempry to DAC. */

	  LPC_GPDMACH0->DMACCSrcAddr = Src_DAC_DMA;
	  LPC_GPDMACH0->DMACCDestAddr = Dst_DAC_DMA;
	  /* The burst size is set to 1, source and dest transfer width is
	  32 bits(word), src addr increment by 1. dst addr unchange. Terminal 
	  Count Int enable */
	 LPC_GPDMACH0->DMACCControl 	|= samples_per_cicle  // Number of total transactions 
	 								|(0x00<<12) 	// Source Burst Size = 1 transaction
									|(0x00<<15)	// Destiny Burst Size = 1 transaction
									|(0x02<<18)	// Source Wordsize = 32 bits
									|(0x02<<21)	// Destiny Wordsize = 32 bits
									|(1<<26)	    // Source Address increases with every transaction
									|(0<<27)		// Destiny Address NOT increases with every transaction
									|(0x80000000);		// Activa Terminal Count Int  1<<31 da warrings

 		/* Set TC int mask, channel enabled, src peripheral is memory(0x0), 
		dst peripheral is DAC(9), and transfer type is the M2P(1) */
	   
	    LPC_GPDMACH0->DMACCConfig 	|=(1<<14)	 // Dont MASK error on channel
	    						  	|(1<<15)	 // Dont MASK Terminal Count Interrupt on channel
									|(DMA_DAC<<6)	// Destiny Pheripheral = DAC	(request signal = DAC)
									|(0x1<<11)	    // Memory to pheripheral transaction
									|(0x01<<0);		 // CHANNEL ENABLE
		
	  	return(1);
   }
  
  else{
	return ( 0 );
  }
}
/******************************************************************************
* Function Name  : Create_DAC_LLIO
* Description    : Creates the LLIO structure for the DAC signal
* Input          : None
* Output         : None
* Return         : None
* Attention		 : It assumes that the buffer where the signal is stored is smaller than the maximum number 
				   of transactions a single LILO element can hold (4095)
*******************************************************************************/
void Create_DAC_LLIO(void){
	onda.source = Src_DAC_DMA;
 	onda.destination = Dst_DAC_DMA;
	onda.next = (uint32_t)&onda;
	onda.control = 					 samples_per_cicle  // Number of total transactions 
	 								|(0x00<<12) 	// Source Burst Size = 1 transaction
									|(0x00<<15)	// Destiny Burst Size = 1 transaction
									|(0x02<<18)	// Source Wordsize = 32 bits
									|(0x02<<21)	// Destiny Wordsize = 32 bits
									|(1<<26)	    // Source Address increases with every transaction
									|(0<<27)		// Destiny Address NOT increases with every transaction
									|(0x80000000);		// Activa Terminal Count Int  1<<31 da warrings
}
/******************************************************************************
* Function Name  : DMA_DAC_audio_Channel_Init
* Description    : Configures a DMA channel to reproduce a given signal from an array given as input
				   The signal reproduces only once.
				   It a Memory to Pheripherial transaction, byte-long (7 bits), burst = 1, N = buffer size
				   Interrupt Enabled after every transaction.
				   The buffer size can be any size you want.
* Input          : - ChannelNum: Channel for the transaction (Actually only configured for channel 0)
				  - audio: uint8_t array with the sound we want to reproduce.
				  - audio_length: Length of the array
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
uint8_t DMA_DAC_audio_Channel_Init( uint32_t ChannelNum,const uint8_t *audio,uint32_t audio_length){

  if ( ChannelNum == 0 ) {
	DACDMA0Done = 0;			/* Clear DMA channel 0 terminal count. */
	
  	Src_DAC_DMA =(uint32_t) audio;					
  	Dst_DAC_DMA = (uint32_t) &(LPC_DAC->DACR);		// This is the value of the DACR -> 0x4008 C000
   	Dst_DAC_DMA +=1 ;			 	// We add 1 so that the byte we transmit gets in the bits [15-8] of DACR

	Create_DAC_audio_LLIO(audio,audio_length);		// Create the LLIO that calls itself so that the sound never stops

	LPC_GPDMA->DMACIntTCClear = 0x01;	  // Clear channel 0 requests
	LPC_GPDMA->DMACIntErrClr = 0x01;	  // Clear channel 0 errors
	LPC_GPDMACH0->DMACCControl = 0;		  // Initialice registers to default 0 to configure afterwards
	LPC_GPDMACH0->DMACCConfig = 0;   
	LPC_GPDMACH0->DMACCLLI = (uint32_t)&audio_clip[0];	 // We give it a LLIO that calls itsenlf in an infinite loop

	  /* Ch0 set for M2P transfer from mempry to DAC. */

	  LPC_GPDMACH0->DMACCSrcAddr = Src_DAC_DMA;
	  LPC_GPDMACH0->DMACCDestAddr = Dst_DAC_DMA;
	  /* The burst size is set to 1, source and dest transfer width is
	  16 bits(half-word), src addr increment by 1. dst addr unchange. Terminal 
	  Count Int enable */

	  // The initial number will be 1 so that then it enters directly the LLIO structure
	  // where the whole sound is placed
	 LPC_GPDMACH0->DMACCControl 	|= 1  // Number of total transactions 
	 								|(0x00<<12) 	// Source Burst Size = 1 transaction
									|(0x00<<15)	// Destiny Burst Size = 1 transaction
									|(0x00<<18)	// Source Wordsize = 8 bits
									|(0x00<<21)	// Destiny Wordsize = 8 bits
									|(1<<26)	    // Source Address increases with every transaction
									|(0<<27)		// Destiny Address NOT increases with every transaction
									|(0x80000000);		// Activa Terminal Count Int  1<<31 da warrings

 		/* Set TC int mask, channel enabled, src peripheral is memory(0x0), 
		dst peripheral is DAC(9), and transfer type is the M2P(1) */
	   
	    LPC_GPDMACH0->DMACCConfig 	|=(1<<14)	 // Dont MASK error on channel
	    						  	|(1<<15)	 // Dont MASK Terminal Count Interrupt on channel
									|(DMA_DAC<<6)	// Destiny Pheripheral = DAC	(request signal = DAC)
									|(0x1<<11)	    // Memory to pheripheral transaction
									|(0x01<<0);		 // CHANNEL ENABLE
		
	  	return(1);
   }
  
  else{
	return ( 0 );
  }
}
/******************************************************************************
* Function Name  : Create_DAC_audio_LLIO
* Description    : Creates the LLIO structure for the DAC audio signal
* Input          :- audio: uint8_t array with the sound we want to reproduce.
				  - audio_length: Length of the array
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Create_DAC_audio_LLIO(const uint8_t *audio,uint32_t audio_length){
	 // The sound is entirely put here, the first transaction will be of 1 byte so that it enters directly this
	 // LLIO structure

	uint8_t i = 0;
	int num_LLIO = audio_length/(0xFFF);		//4095

	// num_LLIO -= 1;			// We delete 1 coz the first 4095 samples are given in the initialization

	int rest = 	audio_length%(0xFFF);
	if (num_LLIO > 0){
		for (i = 0; i <	num_LLIO; i++) {
			audio_clip[i].source =(uint32_t) (audio + i*(0xFFF)); 
	 		audio_clip[i].destination = Dst_DAC_DMA;
			audio_clip[i].next =(uint32_t) &audio_clip[i+1];
			audio_clip[i].control = 	(0xFFF)  // Number of total transactions 
		 								|(0x00<<12) 	// Source Burst Size = 1 transaction
										|(0x00<<15)	// Destiny Burst Size = 1 transaction
										|(0x00<<18)	// Source Wordsize = 8 bits
										|(0x00<<21)	// Destiny Wordsize = 8 bits
										|(1<<26)	    // Source Address increases with every transaction
										|(0<<27)		// Destiny Address NOT increases with every transaction
										|(0x80000000);		// Activa Terminal Count Int  1<<31 da warrings
			}

		}
	  // Now the last LLIO
		audio_clip[i].source =(uint32_t) (audio + i*(0xFFF));  // +1 coz the first 4095 are already given
 		audio_clip[i].destination = Dst_DAC_DMA;
		audio_clip[i].next = 0;
							// (uint32_t) &audio_clip[0];		// SOUND BUCLE !!!!!!!!!!!!!!!!!!!!!!!!!!!

		audio_clip[i].control = 	rest  // Number of total transactions 
	 								|(0x00<<12) 	// Source Burst Size = 1 transaction
									|(0x00<<15)	// Destiny Burst Size = 1 transaction
									|(0x00<<18)	// Source Wordsize = 8 bits
									|(0x00<<21)	// Destiny Wordsize = 8 bits
									|(1<<26)	    // Source Address increases with every transaction
									|(0<<27)		// Destiny Address NOT increases with every transaction
									|(0x80000000);		// Activa Terminal Count Int  1<<31 da warrings


}
/******************************************************************************
**                            End Of File
******************************************************************************/
