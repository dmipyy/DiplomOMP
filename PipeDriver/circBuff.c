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

STATUS STATUS CircBuffCellPut(const void *data, CircBuff_t *cb); {
    return CircBuffDataPut(data, 1, rb);
}


RINGBUF_STATUS RingBuf_DataPut(const void *data, u16_t len, RINGBUF_t *rb) {
    if (rb->buf == NULL) return RINGBUF_PARAM_ERR;
    if (len > rb->size)
        return RINGBUF_OVERFLOW;
    const char *input = data; // recast pointer
    // INPUT data index start address
    size_t s_addr = 0;
    // available space in the end of buffer
    size_t space = rb->size - rb->head;
    if (len > space) { // if len > available space
        // copy data to available space
        memcpy(&rb->buf[rb->head*rb->cell_size], &input[s_addr * rb->cell_size], space * rb->cell_size);
        // next writing will start from 0
        rb->head = 0;
        // new start address = space length
        s_addr = space;
        // new length = len-space
        len -= space;
    }
    // copy all the data to the buf storage
    memcpy(&rb->buf[rb->head*rb->cell_size], &input[s_addr * rb->cell_size], len * rb->cell_size);
    // shift to the next head
    rb->head += len;
    if (rb->head >= rb->size)
        rb->head = 0;
    return RINGBUF_OK;
}

/**
 * @brief Read next byte from the buffer
 *
 * @param[out] data Data byte from the buffer
 * @param[in] rb #RINGBUF_t structure instance
 * @return #RINGBUF_STATUS enum
 */
RINGBUF_STATUS RingBuf_ByteRead(u8_t *data, RINGBUF_t *rb) {
    if (rb->buf == NULL) return RINGBUF_PARAM_ERR;
    RINGBUF_STATUS st = RingBuf_ByteWatch(data, rb);
    if (st != RINGBUF_OK)
        return st;
    rb->tail++;
    if (rb->tail >= rb->size)
        rb->tail = 0;
    return st;
}

/**
 * @brief Read 1 cell from buf
 * @param[out] data Data cell from the buffer
 * @param[in] rb #RINGBUF_t structure instance
 * @return #RINGBUF_STATUS enum
 */
RINGBUF_STATUS RingBuf_CellRead(void *data, RINGBUF_t *rb) {
    return RingBuf_DataRead(data, 1, rb);
}

/**
 * @brief Read some next data from the buffer
 *
 * @param[out] data Data from the buffer
 * @param[in] len Length of data to be read [bytes]
 * @param[in] rb #RINGBUF_t structure instance
 * @return #RINGBUF_STATUS enum
 */
RINGBUF_STATUS RingBuf_DataRead(void *data, u16_t len, RINGBUF_t *rb) {
    if (rb->buf == NULL) return RINGBUF_PARAM_ERR;
    // read data
    RINGBUF_STATUS st = RingBuf_DataWatch(data, len, rb);
    if (st != RINGBUF_OK)
        return st;
    // shift to the next head
    rb->tail += len;
    if (rb->tail >= rb->size)
        rb->tail = 0;
    return st;
}

/**
 * @brief Watch current byte in buf
 * @note Reads data without shifting in the buffer
 *
 * @param[out] data Pointer to data byte got from buffer
 * @param[in] rb #RINGBUF_t structure instance
 * @return #RINGBUF_STATUS enum
 */
RINGBUF_STATUS RingBuf_ByteWatch(u8_t *data, RINGBUF_t *rb) {
    if (data == NULL) return RINGBUF_PARAM_ERR;
    *data = rb->buf[rb->tail];
    return RINGBUF_OK;
}

/**
 * @brief Watch 1 cell from buf
 * @note Reads data without shifting in the buffer
 *
 * @param[out] data Pointer to data cell got from buffer
 * @param[in] #RINGBUF_t structure instance
 * @return #RINGBUF_STATUS enum
 */
RINGBUF_STATUS RingBuf_CellWatch(void *data, RINGBUF_t *rb) {
    return RingBuf_DataWatch(data, 1, rb);
}

/**
 * @brief Watch current data in the buf
 * @note Reads data without shifting in the buffer
 *
 * @param[out] data Data from buffer
 * @param[in] len Length of data to be read [bytes]
 * @param[in] rb #RINGBUF_t structure instance
 * @return #RINGBUF_STATUS enum
 */
RINGBUF_STATUS RingBuf_DataWatch(void *data, u16_t len, RINGBUF_t *rb) {
    if (data == NULL)
        return RINGBUF_PARAM_ERR;
    if (len > rb->size)
        return RINGBUF_OVERFLOW;
    // OUTPUT data index start address
    u16_t s_addr = 0;
    // available space in the end of buffer
    u16_t space = rb->size - rb->tail;
    u16_t loc_tail = rb->tail;
    if (len > space) { // if len > available space
        // recast pointer to u8_t
        // copy data from available space
        memcpy(&data[s_addr * rb->cell_size], &rb->buf[loc_tail * rb->cell_size], space * rb->cell_size);
        // next reading will start from 0
        loc_tail = 0;
        // new start address - space length
        s_addr = space;
        // new length - len-space
        len -= space;
    }
    // copy all the data from the buf storage
    memcpy(&data[s_addr * rb->cell_size], &rb->buf[loc_tail * rb->cell_size], len * rb->cell_size);
    return RINGBUF_OK;
}






