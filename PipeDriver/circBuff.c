#include <circBuff.h>
//инициализация буфера
STATUS CircBuffInit(void* buf, uint16_t size, size_t cellSize, CircBuff_t *cb)
{
	cb->size = size;
	cb->cellSize = cellSize;
	cb->buf = buf;
	CircBuffReset(cb);
	return cb->buf ? OK : PARAM_ERR;
}
//сброс буфера?
STATUS CircBuffReset(CircBuff_t *cb)
{
	if (cb->buf == NULL) return PARAM_ERR; //если буфера нет - ошибка
	
	cb->head = 0;
	cb->tail = 0
	return OK;
}

STATUS CircBuffBytePut(const uint8_t data, CircBuff_t *cb)
{
	if(cb->buf == NULL) return PARAM_ERR;
	
	cb->buf[cb->head++] = data;
	if (cb->head >= cb->size)//если голова вышла за пределы буфера
		cb->head = 0;		 //перещаем ее на начало буфера
	return OK;
}

STATUS CircBuffCellPut(const void *data, CircBuff_t *cb);
{
    return CircBuffDataPut(data, 1, cb);
}

STATUS CircBuffDataPut(const void *data, uint16_t len, CircBuff_t *cb)
{
    if (cb->buf == NULL) return PARAM_ERR;
    if (len > cb->size)
        return OVERFLOW;
	
    const char *input = data; // recast pointer
    // INPUT data index start address
    size_t startAddress = 0;
    // available space in the end of buffer
    size_t space = cb->size - cb->head;
    if (len > space)
	{ // if len > available space
        // copy data to available space
        memcpy(&cb->buf[cb->head*cb->cellSize], &input[startAddress * cb->cellSize], space * cb->cellSize);
        // next writing will start from 0
        cb->head = 0;
        // new start address = space length
        startAddress = space;
        // new length = len-space
        len -= space;
    }
    // copy all the data to the buf storage
    memcpy(&cb->buf[cb->head*cb->cellSize], &input[startAddress * cb->cellSize], len * cb->cellSize);
    // shift to the next head
    cb->head += len;
    if (cb->head >= cb->size)
        cb->head = 0;
    return RINGBUF_OK;
}

/**
 * @brief Read next byte from the buffer
 *
 * @param[out] data Data byte from the buffer
 * @param[in] cb #RINGBUF_t structure instance
 * @return #STATUS enum
 */
STATUS CircBuffByteRead(uint8_t *data, CircBuff_t *cb)
{
    if (cb->buf == NULL) return PARAM_ERR;
	
    STATUS st = CircBuffByteWatch(data, cb);
    if (st != OK)
        return st;
    cb->tail++;
    if (cb->tail >= cb->size)
        cb->tail = 0;
    return st;
}

/**
 * @brief Read 1 cell from buf
 * @param[out] data Data cell from the buffer
 * @param[in] cb #RINGBUF_t structure instance
 * @return #STATUS enum
 */
STATUS CircBuffCellRead(void *data, CircBuff_t *cb)
{
    return CircBuffDataRead(data, 1, cb);
}

/**
 * @brief Read some next data from the buffer
 *
 * @param[out] data Data from the buffer
 * @param[in] len Length of data to be read [bytes]
 * @param[in] cb #RINGBUF_t structure instance
 * @return #STATUS enum
 */
STATUS CircBuffDataRead(void *data, uint16_t len, CircBuff_t *cb)
{
    if (cb->buf == NULL) return PARAM_ERR;
	
    // read data
    STATUS st = CircBuffDataWatch(data, len, cb);
    if (st != OK)
        return st;
    // shift to the next head
    cb->tail += len;
    if (cb->tail >= cb->size)
        cb->tail = 0;
    return st;
}

/**
 * @brief Watch current byte in buf
 * @note Reads data without shifting in the buffer
 *
 * @param[out] data Pointer to data byte got from buffer
 * @param[in] cb #RINGBUF_t structure instance
 * @return #STATUS enum
 */
STATUS CircBuffByteWatch(uint8_t *data, CircBuff_t *cb)
{
    if (data == NULL) return PARAM_ERR;
    *data = cb->buf[cb->tail];
    return OK;
}

/**
 * @brief Watch 1 cell from buf
 * @note Reads data without shifting in the buffer
 *
 * @param[out] data Pointer to data cell got from buffer
 * @param[in] #RINGBUF_t structure instance
 * @return #STATUS enum
 */
STATUS CircBuffCellWatch(void *data, CircBuff_t *cb)
{
    return CircBuffDataWatch(data, 1, cb);
}

/**
 * @brief Watch current data in the buf
 * @note Reads data without shifting in the buffer
 *
 * @param[out] data Data from buffer
 * @param[in] len Length of data to be read [bytes]
 * @param[in] cb #RINGBUF_t structure instance
 * @return #STATUS enum
 */
STATUS CircBuffDataWatch(void *data, uint16_t len, CircBuff_t *cb)
{
    if (data == NULL)
        return PARAM_ERR;
    if (len > cb->size)
        return OVERFLOW;
	
    // OUTPUT data index start address
    uint16_t startAddress = 0;
    // available space in the end of buffer
    uint16_t space = cb->size - cb->tail;
    uint16_t loc_tail = cb->tail;
    if (len > space)
	{ // if len > available space
        // recast pointer to uint8_t
        // copy data from available space
        memcpy(&data[startAddress * cb->cellSize], &cb->buf[loc_tail * cb->cellSize], space * cb->cellSize);
        // next reading will start from 0
        loc_tail = 0;
        // new start address - space length
        startAddress = space;
        // new length - len-space
        len -= space;
    }
    // copy all the data from the buf storage
    memcpy(&data[startAddress * cb->cellSize], &cb->buf[loc_tail * cb->cellSize], len * cb->cellSize);
    return OK;
}






