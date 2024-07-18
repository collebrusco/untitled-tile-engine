/** 
 * abstract_linear_objpool.h
 * created 07/17/24 by frank collebrusco 
 * 
 * push differently sized subclasses stored in place to a stack of memory
 * iterate through them via ptr to base class
 * there is no removing elements
 */
#ifndef ABSTRACT_LINEAR_OBJPOOL_H
#define ABSTRACT_LINEAR_OBJPOOL_H
#include <stdlib.h>
#include <cstring>

#include <stdio.h>
template <typename Base>
class abstract_linear_objpool {
public:
    abstract_linear_objpool(unsigned int _size) : size(_size) {
        buf = (char*)malloc(size); 
        memset(buf, 0, size); 
        top = 0;
    }
    ~abstract_linear_objpool() {
        this->destroy();
        free(buf);
    }

    template <typename Sub, typename ...Args>
    void push(Args... args) {
        buf_entry_t* base = (buf_entry_t*)(buf + top);
        uint16_t jump = sizeof(Sub) + sizeof(base->next_size);
        top += jump;
        if (top > size) {
            loge("overflow"); return;
        }
        base->next_size = (uint16_t)jump;
        new (&base->entry) Sub(args...);
    }

    void clear() {
        this->destroy();
        memset(buf, 0, size);
    }

    class alop_it {
        friend class abstract_linear_objpool;
        unsigned int i;
        abstract_linear_objpool& alop;
        inline alop_it(abstract_linear_objpool* home, unsigned int idx = 0) : i(idx), alop(*home) {}
    public:
        inline Base* operator*() {return (Base*) ( &(( (abstract_linear_objpool::buf_entry_t*)(alop.buf + i) )->entry) );}
        inline bool operator==(alop_it const& other) const {return other.i == this->i;}
        inline bool operator!=(alop_it const& other) const {return other.i != this->i;}
        alop_it& operator++() {
            i += (((abstract_linear_objpool::buf_entry_t*)(alop.buf + i))->next_size);
            if ((((abstract_linear_objpool::buf_entry_t*)(alop.buf + i))->entry) == 0)
                i = -1;
            return *this;
        }
    };

    inline alop_it begin() {return alop_it(this);}
    inline alop_it end() {return alop_it(this, -1);}

private:
    void destroy() {
        top = 0;
        for (Base* obj : *this) {
            obj->~Base();
        }
    }
    char* buf;
    unsigned int top;
    unsigned int size;
    struct __attribute__((packed)) buf_entry_t {
        uint16_t next_size;
        uint32_t entry;
    };
    static void loge(const char* msg);
};

#endif /* ABSTRACT_LINEAR_OBJPOOL_H */
