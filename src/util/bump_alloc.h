/** 
 * bump_alloc.h
 * created 07/14/24 by frank collebrusco 
 */
#ifndef BUMP_ALLOC_H
#define BUMP_ALLOC_H
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

template <size_t bufsize>
class stack_bumpalloc_t {
public:
    inline stack_bumpalloc_t() {memset(data, 0, bufsize); top = 0;}

    inline void reset() {top = 0;}
    void* alloc(size_t bytes) {
        if ((top + bytes) > bufsize) return 0;
        void* block = (void*)(data + top);
        top += bytes;
        return block;
    }
private:
    char data[bufsize];
    uint32_t top;
};

template <size_t bufsize>
class heap_bumpalloc_t {
public:
    inline heap_bumpalloc_t() {
        data = (char*)malloc(bufsize);
        memset(data, 0, bufsize); top = 0;
    }
    ~heap_bumpalloc_t() {free(data);}

    inline void reset() {top = 0;}
    void* alloc(size_t bytes) {
        if ((top + bytes) > bufsize) return 0;
        void* block = (void*)(data + top);
        top += bytes;
        return block;
    }
private:
    char *data;
    uint32_t top;
};

#endif /* BUMP_ALLOC_H */