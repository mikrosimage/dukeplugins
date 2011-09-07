/*
 * MemorySuiteImpl.h
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 *
 * A very basic (and useless ?) implementation of the MemorySuite
 * It simply redirects to malloc and free C functions.
 *
 * It can serve as an example to implement your own memorySuite
 */

#ifndef MEMORYSUITEIMPL_H_
#define MEMORYSUITEIMPL_H_

#include <openfx/ofxCorePlugin.h>
#include <openfx/suite/ofxMemory.h>

namespace openfx {
namespace host {

OfxStatus memoryAlloc(void *handle, size_t nBytes, void **allocatedData);
OfxStatus memoryFree(void *allocatedData);

struct MemorySuiteImpl : public OfxMemorySuiteV1 {
public:
    MemorySuiteImpl() {
        memoryAlloc = &::openfx::host::memoryAlloc;
        memoryFree = &::openfx::host::memoryFree;
    }

    inline OfxStatus _MemoryAlloc(void *handle, size_t nBytes, void **allocatedData) {
        return (*memoryAlloc)(handle, nBytes, allocatedData);
    }
    inline OfxStatus _MemoryFree(void *allocatedData) {
        return (*memoryFree)(allocatedData);
    }
};

} // namespace host
} // namespace openfx

#endif /* MEMORYSUITEIMPL_H_ */
