

#define __DMA_H

/* Second half of the second RAM is used for GPDMA 
operation. */


#define DMA_DAC			7

#define DATA_LENGTH		0x400 
#define DMA_SIZE		0x400

/* DMA mode */
#define M2M				0x00
#define M2P				0x01
#define P2M				0x02
#define P2P				0x03

typedef	struct {
  	uint32_t source;
	uint32_t destination;
	uint32_t next;
  	uint32_t control;  // DMACCXCRegister
}LLIO;

extern void DMA_IRQHandler( void ); 
extern void DMA_Init( void );
extern uint32_t DMA_DAC_Channel_Init( uint32_t ChannelNum );
void Create_DAC_LLIO(void);

uint8_t DMA_DAC_audio_Channel_Init( uint32_t ChannelNum,const uint8_t *audio,uint32_t audio_length);
void Create_DAC_audio_LLIO(const uint8_t *audio,uint32_t audio_length);

extern volatile uint32_t DMATCCount;
extern volatile uint32_t DMAErrCount;

/****************************************************************************
**                            End Of File
****************************************************************************/
