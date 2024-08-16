/** 
 * c_abstract.h
 * created 08/03/24 by frank collebrusco 
 */
#ifndef C_ABSTRACT_H
#define C_ABSTRACT_H
#include <stddef.h>
#include <type_traits>
#include "util/misc.h"

template <class Base, size_t max_size>
struct c_abstract {
static_assert(sizeof(Base) <= max_size);
static_assert(IS_POW2(max_size));

    inline c_abstract() {memset(data, 0x00, max_size);}
    virtual ~c_abstract() = default;

    NO_COPY_OR_MOVE(c_abstract);

    Base& get() {
        return (*((Base*)data));
    }

    template <typename Sub>
    Sub& downcast() {
static_assert(sizeof(Sub) <= max_size);
static_assert(std::is_base_of<Base, Sub>::value);
        return (*((Sub*)((Base*)data)));
    }

    template <typename Sub, typename ...Args>
    Sub& emplace(Args... args) {
static_assert(sizeof(Sub) <= max_size);
static_assert(std::is_base_of<Base, Sub>::value);
        return *(new (data) Sub(args...));
    }

    void destroy() {
        this->get().~Base();
    }

    template <typename Sub, typename ...Args>
    Sub& replace(Args... args) {
        this->destroy();
        return this->emplace(args...);
    }

private:
    alignas(Base) char data[max_size];
};

#endif /* C_ABSTRACT_H */
