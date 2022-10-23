#ifndef _EG_QUEUE_H_
#define _EG_QUEUE_H_

#include <stdint.h>
/**
 * A constant that allows the user to statically allocate the memory needed for the eg_queue_t object.
 */
#define EG_QUEUE_OBJ_SIZE (24u + 1 * sizeof(void *))

/**
 * Queue object. User should allocate memory for this object and use as queue context on own application.
 */
typedef uint8_t eg_queue_t[EG_QUEUE_OBJ_SIZE];

/**
 * MA Queue error codes.
 */
typedef enum
{
    EG_QUEUE_OK,
    EG_QUEUE_ERR_FULL,
    EG_QUEUE_ERR_EMPTY,
    EG_QUEUE_ERR_BUFFER_SIZE,
    EG_QUEUE_ERR_ITEM_SIZE
} eg_queue_error_e;

/**
 * MA Queue initialisation function.
 *
 * @param queue MA Queue object.
 * @param items_buf pointer to buffer allocated by user. The buffer should be of the size of item_size * items_cnt.
 * @param buf_size Allocated buffer size. Used only for parameters validation.
 * @param item_size Size of one queue item
 * @param items_no Max number of items in queue
 * @return eg_queue_error_e Initialisation result.
 */
extern eg_queue_error_e eg_queue_init(eg_queue_t queue, uint8_t *items_buf, uint32_t buf_size, uint32_t item_size, uint32_t items_no);

/**
 * MA Queue put function.
 *
 * @param queue MA Queue object.
 * @param item pointer to item to put in queue.
 * @param item_size pointed item size.
 * @return eg_queue_error_e Operation result.
 */
extern eg_queue_error_e eg_queue_put(eg_queue_t queue, void *item, uint32_t item_size);

/**
 * MA Queue get copy of last item function.
 * The function copies the last element from the queue to the indicated buffer, and then removes it from the queue.
 *
 * @param queue MA Queue object.
 * @param buf item destination buffer.
 * @param buf_size destination buffer size.
 * @return eg_queue_error_e Operation result.
 */
extern eg_queue_error_e eg_queue_get_copy(eg_queue_t queue, void *buf, uint32_t buf_size);

/**
 * MA Queue get last item function.
 * The function passes a pointer to the last item in the queue. After use, call eg_queue_release_last().
 *
 * @param queue MA Queue object.
 * @param ptr destination pointer.
 * @return eg_queue_error_e Operation result.
 */
extern eg_queue_error_e eg_queue_get_last(eg_queue_t queue, void **ptr);

/**
 * MA Queue release last item in the queue function.
 * The function removes the last item from the queue without reading it.
 * The function should be used with the eg_queue_get_last () function after reading the contents of the buffer.
 *
 * @param queue MA Queue object.
 * @return eg_queue_error_e
 */
extern eg_queue_error_e eg_queue_release_last(eg_queue_t queue);

/**
 * MA Queue get max items in the queue function.
 * The function returns the maximum number of items in the queue. For the purpose of performance research.
 *
 * @param queue MA Queue object.
 * @return uint32_t max items in the queue.
 */
extern uint32_t eg_queue_get_max_used(eg_queue_t queue);

#endif /* _EG_QUEUE_H_ */
