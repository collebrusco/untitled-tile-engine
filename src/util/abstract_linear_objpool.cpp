#include "abstract_linear_objpool.h"
#include <flgl/logger.h>
LOG_MODULE(ALOP);

template <typename T>
void abstract_linear_objpool<T>::loge(const char* msg) {
    LOG_ERR("%s", msg);
}

