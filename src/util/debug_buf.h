/** 
 * debug_buf.h
 * created 07/30/24 by frank collebrusco 
 * need to put one of these in one of my libs. use it every time
 */
#ifndef DEBUG_BUF_H
#define DEBUG_BUF_H
#include <string.h>
#include <stdint.h>
#include <stdio.h>
template <typename T, int l2size>
struct debug_buf {
static_assert(l2size < 12);
    debug_buf() {memset(buf, 0, sizeof(T) * (1<<l2size)); top = 0;}
    void push(T v) {buf[(top)&((1<<l2size)-1)] = v; top++;}
    bool ready() const {/*printf("%d, %p, %p\n", top, (void*)((uintptr_t)(~((1<<l2size)-1))), (void*)((uintptr_t)((1<<l2size)-1)));*/
                 return (top&(~((1<<l2size)-1))) && !(top&((1<<l2size)-1));}
    T get() {T v=0; for (uint16_t i = 0; i < (1<<l2size); i++) {v += buf[i];} return v / (static_cast<T>(1<<l2size));}
private:
    uint16_t top{0};
    T buf[(1<<l2size)];
};

#endif /* DEBUG_BUF_H */
