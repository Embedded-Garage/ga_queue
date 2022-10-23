#include <stdint.h>
#include <string.h>

#include "eg_queue_cfg.h"
#include "eg_queue.h"

/** MA Queue context structure */
typedef struct
{
    uint8_t *items_buf; /**< Pointer to buffer to store items */
    uint32_t item_size; /**< Size of one item in queue */
    uint32_t items_no;  /**< Number of items in queue */
    uint32_t head;      /**< Head item in queue */
    uint32_t tail;      /**< tail item in queue */
    uint32_t used;      /**< items counter */
    uint32_t max_used;  /**< max items in queue */
} eg_queue_ctx_s;

/** Static assertion of EG_QUEUE_OBJ_SIZE parameter */
typedef char assertion_on_eg_queue_obj_size[(sizeof(eg_queue_ctx_s) == EG_QUEUE_OBJ_SIZE) * 2 - 1];

eg_queue_error_e eg_queue_init(eg_queue_t queue, uint8_t *items_buf, uint32_t buf_size, uint32_t item_size, uint32_t items_no)
{
    eg_queue_ctx_s *ctx = (eg_queue_ctx_s *)queue;

    if ((item_size * items_no) > buf_size)
    {
        return EG_QUEUE_ERR_BUFFER_SIZE;
    }

    memset(ctx, 0, sizeof(eg_queue_ctx_s));

    ctx->items_buf = items_buf;
    ctx->item_size = item_size;
    ctx->items_no = items_no;

    return EG_QUEUE_OK;
}

eg_queue_error_e eg_queue_put(eg_queue_t queue, void *item, uint32_t item_size)
{
    eg_queue_ctx_s *ctx = (eg_queue_ctx_s *)queue;
    uint8_t *target_item;

    if (ctx->used >= ctx->items_no)
    {
        return EG_QUEUE_ERR_FULL;
    }
    if (item_size > ctx->item_size)
    {
        return EG_QUEUE_ERR_ITEM_SIZE;
    }

    target_item = &ctx->items_buf[ctx->head * ctx->item_size];
    memcpy(target_item, item, item_size);
    memset(&target_item[item_size], 0, ctx->item_size - item_size);

    CRITICAL_SECTION_ENTER();
    ctx->head++;
    ctx->head %= ctx->items_no;
    ctx->used++;
    CRITICAL_SECTION_EXIT();

    if (ctx->used > ctx->max_used)
    {
        ctx->max_used = ctx->used;
    }

    return EG_QUEUE_OK;
}

eg_queue_error_e eg_queue_get_copy(eg_queue_t queue, void *buf, uint32_t buf_size)
{
    eg_queue_ctx_s *ctx = (eg_queue_ctx_s *)queue;
    uint8_t *source_item;
    if (buf_size < ctx->item_size)
    {
        return EG_QUEUE_ERR_ITEM_SIZE;
    }
    if (0u == ctx->used)
    {
        return EG_QUEUE_ERR_EMPTY;
    }

    source_item = &ctx->items_buf[ctx->tail * ctx->item_size];

    memcpy(buf, source_item, buf_size);

    CRITICAL_SECTION_ENTER();
    ctx->tail++;
    ctx->tail %= ctx->items_no;
    ctx->used--;
    CRITICAL_SECTION_EXIT();

    return EG_QUEUE_OK;
}

eg_queue_error_e eg_queue_get_last(eg_queue_t queue, void **ptr)
{
    eg_queue_ctx_s *ctx = (eg_queue_ctx_s *)queue;

    if (0u == ctx->used)
    {
        return EG_QUEUE_ERR_EMPTY;
    }

    *ptr = &ctx->items_buf[ctx->tail * ctx->item_size];

    return EG_QUEUE_OK;
}

eg_queue_error_e eg_queue_release_last(eg_queue_t queue)
{
    eg_queue_ctx_s *ctx = (eg_queue_ctx_s *)queue;
    uint8_t *source_item;

    if (0u == ctx->used)
    {
        return EG_QUEUE_ERR_EMPTY;
    }

    CRITICAL_SECTION_ENTER();
    ctx->tail++;
    ctx->tail %= ctx->items_no;
    ctx->used--;
    CRITICAL_SECTION_EXIT();

    return EG_QUEUE_OK;
}

uint32_t eg_queue_get_max_used(eg_queue_t queue)
{
    eg_queue_ctx_s *ctx = (eg_queue_ctx_s *)queue;

    return ctx->max_used;
}
