#ifndef circBuff_H
#define circBuff_H

#include "libsCircBuff.h"

typedef struct CircBuff_t {
	uint8_t *buf;
	volatile size_t tail;
	volatile size_t head;
	volatile size_t size;
	volatile size_t cellSize;
} CircBuff_t;

typedef enum STATUS{
	OK,
	ERR,
	PARAM_ERR,
	OVERFLOW,
} STATUS;

STATUS CircBuffInit(void* buf, uint16_t size, size_t cellSize, CircBuff_t *cb);
STATUS CircBuffReset(CircBuff_t *cb);
STATUS CircBuffAvailable(uint16 *len, CircBuff_t *cb);

STATUS CircBuffBytePut(const uint8_t data, CircBuff_t *cb);
STATUS CircBuffCellPut(const void *data, CircBuff_t *cb);
STATUS CircBuffDataPut(const void *data, uint16_t len, CircBuff_t *cb);

STATUS CircBuffByteRead(uint8_t *data, CircBuff_t *cb);
STATUS CircBuffCellRead(void *data, CircBuff_t *cb);
STATUS CircBuffDataRead(void *data, uint16_t len, CircBuff_t *cb);

STATUS CircBuffByteWatch(uint8_t *data, CircBuff_t *cb);
STATUS CircBuffCellWatch(void *data, CircBuff_t *cb);
STATUS CircBuffDataWatch(void *data, uint16_t len, CircBuff_t *cb);

#endif

