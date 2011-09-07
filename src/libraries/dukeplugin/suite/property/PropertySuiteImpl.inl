/*
 * PropertySuiteImpl.cpp
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 */

#include "PropertySuiteImpl.h"
#include <dukeplugin/suite/OfxPluginExceptions.h>
#include <sstream>

namespace openfx {
namespace plugin {

// defining nasty macros
#define throwIfUnknown                              \
    if(status==kOfxStatErrUnknown)                  \
        throw exception::unknown_object(property);

#define throwIfBadHandle                            \
    if(status==kOfxStatErrBadHandle)                \
        throw exception::bad_handle("");

#define throwIfBadIndex                             \
    if(status==kOfxStatErrBadIndex){                \
        std::ostringstream msg;                     \
        msg << index;                               \
        throw exception::bad_index(msg.str());      \
    }

#define throwIfBadCount                             \
    if(status==kOfxStatErrBadIndex){                \
        std::ostringstream msg;                     \
        msg << "bad count : ";                      \
        msg << count;                               \
        throw exception::bad_index(msg.str());      \
    }

int PropertyHelper::dimension(const char *property) const {
    int value;
    const OfxStatus status = m_Suite.getSuite().propGetDimension(m_Handle, property, &value);
    throwIfBadHandle;
    throwIfUnknown;
    return value;
}

bool PropertyHelper::reset(const char *property) {
    const OfxStatus status = m_Suite.getSuite().propReset(m_Handle, property);
    throwIfBadHandle;
    throwIfUnknown;
    return status == kOfxStatOK;
}

int PropertyHelper::getInt(const char *property, int index) const {
    int value;
    const OfxStatus status = m_Suite.getSuite().propGetInt(m_Handle, property, index, &value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadIndex;
    return value;
}

double PropertyHelper::getDouble(const char *property, int index) const {
    double value;
    const OfxStatus status = m_Suite.getSuite().propGetDouble(m_Handle, property, index, &value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadIndex;
    return value;
}

char* PropertyHelper::getString(const char *property, int index) const {
    char* value;
    const OfxStatus status = m_Suite.getSuite().propGetString(m_Handle, property, index, &value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadIndex;
    return value;
}

void* PropertyHelper::getPointer(const char *property, int index) const {
    void* value;
    const OfxStatus status = m_Suite.getSuite().propGetPointer(m_Handle, property, index, &value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadIndex;
    return value;
}

void PropertyHelper::getIntN(const char *property, int *value, int count) const {
    const OfxStatus status = m_Suite.getSuite().propGetIntN(m_Handle, property, count, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadCount;
}

void PropertyHelper::getDoubleN(const char *property, double *value, int count) const {
    const OfxStatus status = m_Suite.getSuite().propGetDoubleN(m_Handle, property, count, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadCount;
}

void PropertyHelper::getStringN(const char *property, char **value, int count) const {
    const OfxStatus status = m_Suite.getSuite().propGetStringN(m_Handle, property, count, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadCount;
}

void PropertyHelper::getPointerN(const char *property, void **value, int count) const {
    const OfxStatus status = m_Suite.getSuite().propGetPointerN(m_Handle, property, count, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadCount;
}

void PropertyHelper::setInt(const char *property, int value, int index) {
    const OfxStatus status = m_Suite.getSuite().propSetInt(m_Handle, property, index, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadIndex;
}

void PropertyHelper::setDouble(const char *property, double value, int index) {
    const OfxStatus status = m_Suite.getSuite().propSetDouble(m_Handle, property, index, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadIndex;
}

void PropertyHelper::setString(const char *property, const char *value, int index) {
    const OfxStatus status = m_Suite.getSuite().propSetString(m_Handle, property, index, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadIndex;
}

void PropertyHelper::setPointer(const char *property, void *value, int index) {
    const OfxStatus status = m_Suite.getSuite().propSetPointer(m_Handle, property, index, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadIndex;
}

void PropertyHelper::setIntN(const char *property, int *value, int count) {
    const OfxStatus status = m_Suite.getSuite().propSetIntN(m_Handle, property, count, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadCount;
}

void PropertyHelper::setDoubleN(const char *property, double *value, int count) {
    const OfxStatus status = m_Suite.getSuite().propSetDoubleN(m_Handle, property, count, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadCount;
}

void PropertyHelper::setStringN(const char *property, const char **value, int count) {
    const OfxStatus status = m_Suite.getSuite().propSetStringN(m_Handle, property, count, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadCount;
}

void PropertyHelper::setPointerN(const char *property, void **value, int count) {
    const OfxStatus status = m_Suite.getSuite().propSetPointerN(m_Handle, property, count, value);
    throwIfBadHandle;
    throwIfUnknown;
    throwIfBadCount;
}

// undefining nasty macros
#undef throwIfUnknown
#undef throwIfBadHandle
#undef throwIfBadIndex
#undef throwIfBadCount

} // namespace plugin
} // namespace openfx
