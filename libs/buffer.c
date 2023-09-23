#include "buffer.h"

/* cannot be 0 */
/**
 * Initializes a buffer with default values.
 *
 * This function initializes a buffer with default values based on the data type.
 * The default values are determined as follows:
 * - For integer types, the default value is 0.
 * - For floating-point types, the default value is 0.0.
 * - For boolean types, the default value is false.
 * - For character types, the default value is '\0'.
 *
 * @param buffer The buffer to be initialized.
 * @param size The size of the buffer in bytes.
 *
 * @returns None
 */
void *buff_init(int size){
    Buffer *buff = sec_malloc(sizeof(Buffer));
    buff->size = 0;
    buff->capacity = size;
    buff->body = sec_malloc(size);
    return buff;
}

/**
 * Inserts a value into a buffer at a specified index.
 *
 * @param buffer The buffer to insert the value into.
 * @param index The index at which to insert the value.
 * @param value The value to be inserted.
 *
 * @returns None
 */
void buff_insert(Buffer *buff, void *add, size_t size, size_t index){
    if(index + size > buff->capacity){
        buff_resize(buff, index + size);
    }
    memmove(buff->body + index, add, size);
    int diff = (ssize_t) index - (ssize_t) buff->size;
    if(diff > 0){ /* there is a gap */
        print(STDERR_FILENO, "Error: buffer insert gap\n");
        exit(EXIT_FAILURE);
    }else if(diff < 0){
        print(STDERR_FILENO, "Error: buffer insert overlap\n");
        exit(EXIT_FAILURE);
    }else{ 
        buff->size += size;
    }
}

/**
 * Appends the contents of one buffer to another buffer.
 *
 * @param dest The destination buffer to append to.
 * @param src The source buffer to append from.
 * @param dest_size The size of the destination buffer.
 * @param src_size The size of the source buffer.
 *
 * @returns None
 */
void buff_append(Buffer *buff, void *add, size_t size){
    if(buff->size + size > buff->capacity){
        buff_resize(buff, buff->size + size);
    }
    memmove(buff->body + buff->size, add, size);
    buff->size += size;
}

/**
 * Appends a byte to a buffer.
 *
 * @param buffer The buffer to append the byte to.
 * @param byte The byte to append.
 *
 * @returns None
 */
void buff_append_byte(Buffer *buff, byte add){
    if(buff->size + 1 > buff->capacity){
        buff_resize(buff, buff->size + 1);
    }
    memmove(buff->body + buff->size, &add, 1);
    buff->size += 1;
}

/**
 * Calculates the size of the buffer required for a given operation.
 *
 * @param None
 *
 * @returns The size of the buffer required.
 */
size_t buff_size(Buffer *buff){
    return buff->size;
}

/**
 * Returns the capacity of the buffer.
 *
 * @returns The capacity of the buffer.
 */
size_t buff_capacity(Buffer *buff){
    return buff->capacity;
}

/**
 * Returns the content of the buffer.
 *
 * @returns The content of the buffer.
 */
void *buff_body(Buffer *buff){
    return buff->body;
}

/**
 * Clears the contents of a buffer.
 *
 * @param buffer The buffer to be cleared.
 *
 * @returns None
 */
void buff_clear(Buffer *buff){
    memset(buff->body, 0, buff->size);
    buff->size = 0;
}

/* uses sec_realloc so buff body is new pointer */
/**
 * Resizes the buffer to the specified size.
 *
 * @param size The new size of the buffer.
 *
 * @returns None
 */
void buff_resize(Buffer *buff, size_t new_size){
    buff->body = sec_realloc(buff->body, buff->capacity, new_size);
    buff->capacity = new_size;
}

/**
 * Frees the memory allocated for a buffer.
 *
 * @param buff A pointer to the buffer to be freed.
 *
 * @returns None
 */
void buff_free(Buffer *buff){
    free(buff->body);
    free(buff);
}

/* specify the endianess OF THE SYSTEM */
/**
 * Dumps the contents of a buffer to the console.
 *
 * @param buffer The buffer to be dumped.
 *
 * @returns None
 */
void buff_dump(Buffer *buff, int numbytes, int endianess){
    if(endianess == LITTLE_ENDIAN){
        for(int i = 0; i < numbytes; i ++){
            byte chunk = *(((byte *) buff->body) + i);
            for(int b = 7; b >= 0; b --){
                (chunk & (1 << b)) ? print(STDOUT_FILENO, "1") : print(STDOUT_FILENO, "0");
            }
            print(STDOUT_FILENO, " ");
        }
        println(STDOUT_FILENO, "");
    }else{
        for(int i = numbytes - 1; i >= 0; i --){
            byte chunk = *(((byte *) buff->body) + i);
            for(int b = 7; b >= 0; b --){
                (chunk & (1 << b)) ? print(STDOUT_FILENO, "1") : print(STDOUT_FILENO, "0");
            }
            print(STDOUT_FILENO, " ");
        }
        println(STDOUT_FILENO, "");
    }
}
