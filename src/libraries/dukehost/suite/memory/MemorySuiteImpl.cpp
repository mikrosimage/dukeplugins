/*
 * MemorySuiteImpl.cpp
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 */

#include "MemorySuiteImpl.h"

#include <cstdlib>

#ifndef __APPLE__
#include <malloc.h>
#endif

namespace openfx {
namespace host {

OfxStatus memoryAlloc(void *handle, size_t nBytes, void **allocatedData) {
    *allocatedData = malloc(nBytes);
    return kOfxStatOK;
}

OfxStatus memoryFree(void *allocatedData) {
    free(allocatedData);
    return kOfxStatOK;
}

} // namespace host
} // namespace openfx
