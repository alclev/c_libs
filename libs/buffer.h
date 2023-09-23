#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include "syscalls.h"

typedef unsigned char byte;

/**
 * A struct representing a buffer.
 *
 * @param size The current size of the buffer.
 * @param capacity The maximum capacity of the buffer.
 * @param body A pointer to the memory allocated for the buffer.
 */
struct buff {
    size_t size;
    size_t capacity;
    void *body;
};
typedef struct buff Buffer;


/* function prototypes */
void *buff_init(int size); 
void buff_insert(Buffer *buff, void *add, size_t size, size_t index);
void buff_append(Buffer *buff, void *add, size_t size);
void buff_append_byte(Buffer *buff, byte add);
size_t buff_size(Buffer *buff);
size_t buff_capacity(Buffer *buff);
void *buff_body(Buffer *buff);
void buff_clear(Buffer *buff);
void buff_resize(Buffer *buff, size_t new_size);
void buff_free(Buffer *buff);
void buff_dump(Buffer *buff, int numbytes, int endianess);

#endif