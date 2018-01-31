#ifndef INC_FUNKCJE_SSP_H_
#define INC_FUNKCJE_SSP_H_

/* SPI read and write buffer size */
#define FIFOSIZE         8

/* SSP CR1 register */
#define SSPCR1_LBM      (1 << 0)
#define SSPCR1_SSE      (1 << 1)
#define SSPCR1_MS       (1 << 2)
#define SSPCR1_SOD      (1 << 3)

/* SSP Interrupt Mask Set/Clear register */
#define SSPIMSC_RORIM   (1 << 0)
#define SSPIMSC_RTIM    (1 << 1)
#define SSPIMSC_RXIM    (1 << 2)
#define SSPIMSC_TXIM    (1 << 3)

/* SSP Status register */
#define SSPSR_TFE       (1 << 0)
#define SSPSR_TNF       (1 << 1)
#define SSPSR_RNE       (1 << 2)
#define SSPSR_RFF       (1 << 3)
#define SSPSR_BSY       (1 << 4)


extern void SSP_init( void );
extern void SSP_Send( uint8_t *Buf, uint32_t Length );
#endif /* INC_FUNKCJE_SSP_H_ */
