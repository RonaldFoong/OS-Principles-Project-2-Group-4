#include "chunk_size_of.h"
#include <stddef.h>

size_t chunk_size_of(size_t bytes) {
    if (bytes <= 32) {
        return 32;
    } 
    if (bytes <= 64) {
        return 64;
    }
    if (bytes <= 128) {
        return 128;
    }
    if (bytes <= 256) {
        return 256;
    }
    if (bytes <= 512) {
        return 512;
    }
    return 0;
}