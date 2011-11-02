/*
 * HostUtils.cpp
 *
 *  Created on: 13 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include "HostUtils.h"
#include <stdexcept>
#include <cassert>

namespace openfx {
namespace host {

OfxStatus performNoThrow(const OfxPlugin* pPlugin, const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs) {
    assert(pPlugin);
    return pPlugin->mainEntry(action, handle, inArgs, outArgs);
}

OfxStatus perform(const OfxPlugin* pPlugin, const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs) {
    return throwSuiteStatusException(performNoThrow(pPlugin, action, handle, inArgs, outArgs));
}

OfxStatus throwSuiteStatusException(const OfxStatus stat) {
    switch (stat) {
        case kOfxStatOK:
        case kOfxStatReplyYes:
        case kOfxStatReplyNo:
        case kOfxStatReplyDefault:
            break;
        case kOfxStatErrMemory:
            throw std::bad_alloc();
        default:
            throw std::runtime_error(mapStatusToString(stat));
    }
    return stat;
}

const char* mapStatusToString(OfxStatus stat) {
    switch (stat) {
        case kOfxStatOK:
            return "kOfxStatOK";
        case kOfxStatFailed:
            return "kOfxStatFailed";
        case kOfxStatErrFatal:
            return "kOfxStatErrFatal";
        case kOfxStatErrUnknown:
            return "kOfxStatErrUnknown";
        case kOfxStatErrMissingHostFeature:
            return "kOfxStatErrMissingHostFeature";
        case kOfxStatErrUnsupported:
            return "kOfxStatErrUnsupported";
        case kOfxStatErrExists:
            return "kOfxStatErrExists";
        case kOfxStatErrFormat:
            return "kOfxStatErrFormat";
        case kOfxStatErrMemory:
            return "kOfxStatErrMemory";
        case kOfxStatErrBadHandle:
            return "kOfxStatErrBadHandle";
        case kOfxStatErrBadIndex:
            return "kOfxStatErrBadIndex";
        case kOfxStatErrValue:
            return "kOfxStatErrValue";
        case kOfxStatReplyYes:
            return "kOfxStatReplyYes";
        case kOfxStatReplyNo:
            return "kOfxStatReplyNo";
        case kOfxStatReplyDefault:
            return "kOfxStatReplyDefault";
    }
    return "UNKNOWN STATUS CODE";
}

} // namespace host
} // namespace openfx

