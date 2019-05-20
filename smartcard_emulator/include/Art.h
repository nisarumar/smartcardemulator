#ifndef ART_H_
#define ART_H_

struct art_config{
	volatile uint16_t etu;
	volatile uint8_t statusReg;
	volatile uint8_t rxBuffer;
	volatile uint8_t rxBitIdx;
	volatile uint8_t txBuffer;
	volatile uint8_t txBitIdx;
	struct fifo_t *txFifo;
	struct fifo_t *rxFifo;
	/* todo: add dev_config here */
};

typedef enum{
	TRANSMITTER = 0,
	RECEIVER
}art_mod;

/* @brief mode : Rx or Tx */
#define ART_STATUS_REG_MODE				UINT8_C(0)
/* @brief Rx in progress */
#define ART_STATUS_REG_RX				UINT8_C(1)
/* @brief Tx in progress */
#define ART_STATUS_REG_TX				UINT8_C(2)
/* @brief Parity Error */
#define ART_STATUS_REG_PARITY_ERR		UINT8_C(3)
/* @brief RX value in the buffer */
#define ART_STATUS_REG_RX_COMP			UINT8_C(4)
/* @brief TX value in the buffer */
#define ART_STATUS_REG_TX_COMP			UINT8_C(5)
/* @brief RX FIFO is Full */
#define ART_STATUS_REG_RXFIFO_FULL		UINT8_C(6)
/* @brief TX FIFO is Full */
#define ART_STATUS_REG_TXFIFO_FULL		UINT8_C(7)
/* @brief Expected status value for Tx */
#define ART_STATUS_TX_READY				UINT8_C(0)
/* @brief Expected status value for Rx */
#define ART_STATUS_RX_READY				UINT8_C(1)

#define ART_NOK							UINT8_C(1)
#define ART_OK							UINT8_C(0)
#define ART_LOW							UINT8_C(0)
#define ART_HIGH						UINT8_C(1)
#define ART_BYTE						UINT8_C(8)


void Art_duplexMode(struct art_config* , art_mod);

void Art_gpioInterrupt(struct art_config* );

void Art_rxTimerInterrupt(struct art_config* );

void Art_txTimerInterrupt(struct art_config* );

uint8_t Art_txByteStart(struct art_config* );

uint8_t Art_rxByteStart(struct art_config* );

#endif
