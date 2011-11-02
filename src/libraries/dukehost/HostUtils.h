/*
 * HostUtils.h
 *
 *  Created on: 13 avr. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef HOSTUTILS_H_
#define HOSTUTILS_H_

#include "openfx/ofxCorePlugin.h"

namespace openfx {
namespace host {

/**
 * perform an action on the plugin and throws according to the returned OfxStatus
 */
OfxStatus perform(const OfxPlugin* pPlugin, const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs);

/**
 * same as above but never throws
 */
OfxStatus performNoThrow(const OfxPlugin* pPlugin, const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs);

/**
 * throws a std::runtime_error exception if OfxStatus is anything but
 * kOfxStatOK, kOfxStatReplyYes, kOfxStatReplyNo, kOfxStatReplyDefault
 *
 * In particular a std::bad_alloc exceptions is thrown in case of kOfxStatErrMemory
 *
 * TODO : right now we are throwing in case of kOfxStatusFailed, maybe it's a bit too much ?
 */
OfxStatus throwSuiteStatusException(OfxStatus stat);

/**
 * A convenient function to map OfxStatus to strings
 */
const char* mapStatusToString(OfxStatus stat);

} // namespace host
} // namespace openfx

#endif /* HOSTUTILS_H_ */
