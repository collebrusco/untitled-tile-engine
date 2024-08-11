/** 
 * misc.h
 * created 08/03/24 by frank collebrusco 
 */
#ifndef MISC_HEADER_H
#define MISC_HEADER_H

#define IS_POW2(num) ( !((num) & ((num)-1)) )

#define NO_COPY(class_type) \
    class_type(class_type const& other) = delete; \
    class_type& operator=(class_type const& other) = delete; \

#define NO_MOVE(class_type) \
    class_type(class_type && other) = delete; \
    class_type& operator=(class_type && other) = delete;

#define NO_COPY_OR_MOVE(class_type) \
    class_type(class_type const& other) = delete; \
    class_type(class_type && other) = delete; \
    class_type& operator=(class_type const& other) = delete; \
    class_type& operator=(class_type && other) = delete;

#define ASSERT_AGGREGATE(type) static_assert(std::is_aggregate<type>::value)

#endif /* MISC_HEADER_H */
