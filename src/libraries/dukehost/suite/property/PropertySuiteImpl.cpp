/*
 * PropertySuiteImpl.cpp
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 */

#include "PropertySuiteImpl.h"
#include "PropertySet.h"

namespace openfx {
namespace host {

PropertySuiteV1Impl::PropertySuiteV1Impl() {
    propGetDimension = &::openfx::host::propGetDimension;
    propGetDouble = &::openfx::host::propGetDouble;
    propGetDoubleN = &::openfx::host::propGetDoubleN;
    propGetInt = &::openfx::host::propGetInt;
    propGetIntN = &::openfx::host::propGetIntN;
    propGetPointer = &::openfx::host::propGetPointer;
    propGetPointerN = &::openfx::host::propGetPointerN;
    propGetString = &::openfx::host::propGetString;
    propGetStringN = &::openfx::host::propGetStringN;
    propReset = &::openfx::host::propReset;
    propSetDouble = &::openfx::host::propSetDouble;
    propSetDoubleN = &::openfx::host::propSetDoubleN;
    propSetInt = &::openfx::host::propSetInt;
    propSetIntN = &::openfx::host::propSetIntN;
    propSetPointer = &::openfx::host::propSetPointer;
    propSetPointerN = &::openfx::host::propSetPointerN;
    propSetString = &::openfx::host::propSetString;
    propSetStringN = &::openfx::host::propSetStringN;
}

inline Property* getProperty(OfxPropertySetHandle pHandle, const char *property) {
    PropertySet *pProperties = dynamic_cast<PropertySet*> (pHandle);
    assert(pProperties);
    const PropertySet::MAP_TYPE &map = pProperties->map;
    const PropertySet::MAP_TYPE::const_iterator result = map.find(property);
    if (result == map.end())
        return NULL;
    return result->second;
}

#define DeclarePropertyAndReturnIfUnknown                                       \
    if(properties == NULL)                                                      \
        return kOfxStatErrBadHandle;                                            \
    Property * const pProperty = getProperty(properties, property);             \
    if(pProperty==NULL)                                                         \
        return kOfxStatErrUnknown;

#define CastToIntAndReturnIfBadHandle                      		                \
	if(pProperty->type != eInt)										            \
		return kOfxStatErrBadHandle;									        \
	IntProperty* intProperty = static_cast<IntProperty* >(pProperty);

#define CastToDoubleAndReturnIfBadHandle                      		            \
	if(pProperty->type != eDouble)										        \
		return kOfxStatErrBadHandle;									        \
	DoubleProperty* doubleProperty = static_cast<DoubleProperty* >(pProperty);

#define CastToStringAndReturnIfBadHandle                      		            \
	if(pProperty->type != eString)										        \
		return kOfxStatErrBadHandle;									        \
	StringProperty* stringProperty = static_cast<StringProperty* >(pProperty);

#define CastToPointerAndReturnIfBadHandle                      		            \
	if(pProperty->type != ePointer)										        \
		return kOfxStatErrBadHandle;									        \
	PointerProperty* pointerProperty = static_cast<PointerProperty* >(pProperty);

OfxStatus propReset(OfxPropertySetHandle properties, const char *property) {
    DeclarePropertyAndReturnIfUnknown
    pProperty->reset();
    return kOfxStatOK;
}

OfxStatus propGetDimension(OfxPropertySetHandle properties, const char *property, int *count) {
    DeclarePropertyAndReturnIfUnknown
    *count = pProperty->size();
    return kOfxStatOK;
}

OfxStatus propGetDouble(OfxPropertySetHandle properties, const char *property, int index, double *value) {
    DeclarePropertyAndReturnIfUnknown
    CastToDoubleAndReturnIfBadHandle
    if ((int) doubleProperty->value.size() - 1 < index)
        return kOfxStatErrBadIndex;
    *value = doubleProperty->value.at(index);
    return kOfxStatOK;
}

OfxStatus propGetDoubleN(OfxPropertySetHandle properties, const char *property, int count, double *value) {
    DeclarePropertyAndReturnIfUnknown
    CastToDoubleAndReturnIfBadHandle
    if ((int) doubleProperty->value.size() < count)
        return kOfxStatErrBadIndex;
    for (int i = 0; i < count; ++i)
        value[i] = doubleProperty->value.at(i);
    return kOfxStatOK;
}

OfxStatus propGetInt(OfxPropertySetHandle properties, const char *property, int index, int *value) {
    DeclarePropertyAndReturnIfUnknown
    CastToIntAndReturnIfBadHandle
    if ((int) intProperty->value.size() - 1 < index)
        return kOfxStatErrBadIndex;
    *value = intProperty->value.at(index);
    return kOfxStatOK;
}

OfxStatus propGetIntN(OfxPropertySetHandle properties, const char *property, int count, int *value) {
    DeclarePropertyAndReturnIfUnknown
    CastToIntAndReturnIfBadHandle
    if ((int) intProperty->value.size() < count)
        return kOfxStatErrBadIndex;
    for (int i = 0; i < count; ++i)
        value[i] = intProperty->value.at(i);
    return kOfxStatOK;
}

OfxStatus propGetPointer(OfxPropertySetHandle properties, const char *property, int index, void **value) {
    DeclarePropertyAndReturnIfUnknown
    CastToPointerAndReturnIfBadHandle
    if ((int) pointerProperty->value.size() - 1 < index)
        return kOfxStatErrBadIndex;
    *value = pointerProperty->value.at(index);
    return kOfxStatOK;
}

OfxStatus propGetPointerN(OfxPropertySetHandle properties, const char *property, int count, void **value) {
    DeclarePropertyAndReturnIfUnknown
    CastToPointerAndReturnIfBadHandle
    if ((int) pointerProperty->value.size() < count)
        return kOfxStatErrBadIndex;
    for (int i = 0; i < count; ++i)
        value[i] = pointerProperty->value.at(i);
    return kOfxStatOK;
}

OfxStatus propGetString(OfxPropertySetHandle properties, const char *property, int index, char **value) {
    DeclarePropertyAndReturnIfUnknown
    CastToStringAndReturnIfBadHandle
    if ((int) stringProperty->value.size() - 1 < index)
        return kOfxStatErrBadIndex;
    //FIXME API should be char* const* value
    *value = const_cast<char*> (stringProperty->value.at(index).c_str());
    return kOfxStatOK;
}

OfxStatus propGetStringN(OfxPropertySetHandle properties, const char *property, int count, char **value) {
    DeclarePropertyAndReturnIfUnknown
    CastToStringAndReturnIfBadHandle
    if ((int) stringProperty->value.size() < count)
        return kOfxStatErrBadIndex;
    for (int i = 0; i < count; ++i)
        //FIXME API should be char* const* value
        value[i] = const_cast<char*> (stringProperty->value.at(i).c_str());
    return kOfxStatOK;
}

OfxStatus propSetDouble(OfxPropertySetHandle properties, const char *property, int index, double value) {
    DeclarePropertyAndReturnIfUnknown
    CastToDoubleAndReturnIfBadHandle
    if ((int) doubleProperty->value.size() - 1 < index)
        return kOfxStatErrBadIndex;
    doubleProperty->value.at(index) = value;
    return kOfxStatOK;
}

OfxStatus propSetDoubleN(OfxPropertySetHandle properties, const char *property, int count, double *value) {
    DeclarePropertyAndReturnIfUnknown
    CastToDoubleAndReturnIfBadHandle
    if ((int) doubleProperty->value.size() < count)
        return kOfxStatErrBadIndex;
    for (int i = 0; i < count; ++i)
        doubleProperty->value.at(i) = value[i];
    return kOfxStatOK;
}

OfxStatus propSetInt(OfxPropertySetHandle properties, const char *property, int index, int value) {
    DeclarePropertyAndReturnIfUnknown
    CastToIntAndReturnIfBadHandle
    if ((int) intProperty->value.size() - 1 < index)
        return kOfxStatErrBadIndex;
    intProperty->value.at(index) = value;
    return kOfxStatOK;
}

OfxStatus propSetIntN(OfxPropertySetHandle properties, const char *property, int count, int *value) {
    DeclarePropertyAndReturnIfUnknown
    CastToIntAndReturnIfBadHandle
    if ((int) intProperty->value.size() < count)
        return kOfxStatErrBadIndex;
    for (int i = 0; i < count; ++i)
        intProperty->value.at(i) = value[i];
    return kOfxStatOK;
}

OfxStatus propSetPointer(OfxPropertySetHandle properties, const char *property, int index, void *value) {
    DeclarePropertyAndReturnIfUnknown
    CastToPointerAndReturnIfBadHandle
    if ((int) pointerProperty->value.size() - 1 < index)
        return kOfxStatErrBadIndex;
    pointerProperty->value.at(index) = value;
    return kOfxStatOK;
}

OfxStatus propSetPointerN(OfxPropertySetHandle properties, const char *property, int count, void **value) {
    DeclarePropertyAndReturnIfUnknown
    CastToPointerAndReturnIfBadHandle
    if ((int) pointerProperty->value.size() < count)
        return kOfxStatErrBadIndex;
    for (int i = 0; i < count; ++i)
        pointerProperty->value.at(i) = value[i];
    return kOfxStatOK;
}

OfxStatus propSetString(OfxPropertySetHandle properties, const char *property, int index, const char *value) {
    DeclarePropertyAndReturnIfUnknown
    CastToStringAndReturnIfBadHandle
    if ((int) stringProperty->value.size() - 1 < index)
        return kOfxStatErrBadIndex;
    stringProperty->value.at(index) = value;
    return kOfxStatOK;
}

OfxStatus propSetStringN(OfxPropertySetHandle properties, const char *property, int count, const char **value) {
    DeclarePropertyAndReturnIfUnknown
    CastToStringAndReturnIfBadHandle
    if ((int) stringProperty->value.size() < count)
        return kOfxStatErrBadIndex;
    for (int i = 0; i < count; ++i)
        stringProperty->value.at(i) = value[i];
    return kOfxStatOK;
}

// undefining nasty macros
#undef DeclarePropertyAndReturnIfUnknown
#undef CastToIntAndReturnIfBadHandle
#undef CastToDoubleAndReturnIfBadHandle
#undef CastToStringAndReturnIfBadHandle
#undef CastToPointerAndReturnIfBadHandle

} // namespace host
} // namespace openfx
