/** 
 * abstract_freelist_objpool.h
 * created 07/22/24 by frank collebrusco 
 */
#ifndef ABSTRACT_FREELIST_OBJPOOL_H
#define ABSTRACT_FREELIST_OBJPOOL_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define IS_POW2(num) ( !(num & (num-1)) )

/**
 * this structure stores a pool of objects inheriting from the same base.
 * they are stored in place, each object 'slot' is of a fixed size.
 * then an iterator is provided to loop through and get base class ptrs.
 * subclasses can be of varying size as long as they're less than the slot size
 * now, this is weird. we determine if a slot is being used by looking at the last byte
 * since we want slots to be a power of 2, slot_size MUST BE A POWER OF 2 MINUS ONE
 * which is silly but hey. This is enforced with a static assert so don't worry
 */
template <typename Base, uint16_t slot_size, uint16_t slots>
struct abstract_freelist_objpool {
    abstract_freelist_objpool() {
        static_assert(IS_POW2(slot_size + 1));
        static_assert(slot_size >= 3);
        static_assert(slots > 1);
        data = (char*)malloc(size_bytes());
        memset(data, 0, size_bytes());
        top = 0;
    }
    ~abstract_freelist_objpool() {free(data);}

    inline uint32_t size_bytes() const {return (slot_size * slots) + slots;}
    inline Base* at(uint16_t i) {
        if (i >= slots) return 0; 
        return &data[i * (slot_size + 1)];
    }
    inline bool is_valid(uint16_t i) {
        return (i < slots) && data[(i * (slot_size + 1)) + slot_size];
    }
    inline void set_valid(uint16_t i, bool v) {
        if (!(i < slots)) return;
        data[(i * (slot_size + 1)) + slot_size] = v;
    }

    template <typename Sub, typename ...Args>
    void push(Args ...args) {
        static_assert(sizeof(Sub) <= slot_size);
        uint16_t i;
        if (freelist.top) {
            i = freelist.data[--freelist.top];
        } else {
            i = top++;
        }
        set_valid(i, true);
        new (at(i)) Sub(args...);
    }

    void remove(Base* entry) {
        uintptr_t off = (uintptr_t)entry - (uintptr_t)data;
        uint16_t i = off / (slot_size + 1);
        if (i > slots) {printf("[ERROR afop]: remove: i = %d out of bounds", i);}
        entry->~Base();
        set_valid(i, false);
        freelist.data[freelist[top++]];
    }

    class afop_it;
    afop_it begin() {return afop_it(this);}
    afop_it end() {return afop_it(this, -1);}

    class afop_it {
    friend class abstract_freelist_objpool;
        abstract_freelist_objpool& afop;
        uint16_t i{0};
        afop_it(abstract_freelist_objpool* home, uint16_t _i = 0) : i(_i) {}
    public:
        inline bool operator==(afop_it const& other) const {return this->i == other->i;}
        inline bool operator!=(afop_it const& other) const {return this->i != other->i;}
        inline afop_it const& operator++() {
            if (i >= slots) return *this;
            do {
                i++;
                if (i >= slots) {
                    i = -1; return *this;
                }
            } while (afop.data[i_to_ptr(i) - 1] == 0);
        }
        inline Base* operator*() const {return i == -1 ? 0 : (Base*)(afop.data + i_to_ptr(i));}
    };

private:
    char* data;
    uint16_t top;
    struct {
        uint16_t data[slots];
        uint16_t top;
    } freelist = {0};
};


#endif /* ABSTRACT_FREELIST_OBJPOOL_H */
