#ifndef DEV_H_
#define DEV_H_

/* @todo : this should not contain art_config, device configuration \
 * should also be mapped here */
struct dev_config{
	volatile uint16_t * timerValue;
	volatile uint8_t * rxBuff;
	volatile uint8_t * txBuff;
	volatile uint8_t * statusReg;
	struct art_config* artPtr;
};

void Dev_timerInit(void);
void Dev_setGpioIn(void);
void Dev_setGpioOut(void);
void Dev_enRxTimer(void);
inline void Dev_enTxTimer(void);
void Dev_updateRxTimerValue(uint16_t );
void Dev_denGpioInterrupt(void);
void Dev_setPin(uint8_t );
uint8_t Dev_getPinAvg(void);
uint8_t Dev_calcParity(void);
void Dev_sendParity(void);
void Dev_denTxTimer(void);
void Dev_denRxTimer(void);
void Dev_enGpioInterrupt(void);
void Dev_init(struct dev_config* );
void Dev_raiseTxComplInt(void);
void Dev_raiseRxComplInt(void);
#endif
