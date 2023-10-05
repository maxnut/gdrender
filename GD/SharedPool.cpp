#include "SharedPool.hpp"

BS::thread_pool& sharedPool() {
    static BS::thread_pool pool;
    return pool;
}