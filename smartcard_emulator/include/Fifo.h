#ifndef FIFO_H_
#define FIFO_H_

struct fifo_t{
	uint8_t * buff;
	uint8_t rd;
	uint8_t wr;
	uint8_t sz;
	uint8_t fl;	/*fill level*/
};

#define MK_FIFO(name, size)		struct fifo_t fifo_##name =\
													{NULL,0,0,0,0};\
								struct fifo_t * name = \
													& fifo_##name;\
								static uint8_t fifo_buff_##name[size];\

#define FIFO_OK					UINT8_C(0)
#define FIFO_NOK				UINT8_C(1)				

uint8_t Fifo_fillLevel(struct fifo_t*);
uint8_t Fifo_write(struct fifo_t*, uint8_t);
uint8_t Fifo_read(struct fifo_t* handl, uint8_t * data);


#endif
