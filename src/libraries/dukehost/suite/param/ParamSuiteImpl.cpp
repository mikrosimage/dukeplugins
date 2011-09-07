/*
 * ParamSuiteImpl.cpp
 *
 *  Created on: Oct 9, 2010
 *      Author: Guillaume Chatelet
 */

#include "ParamSuiteImpl.h"

#include "ParameterDescriptor.h"
#include "ParameterSet.h"
#include "Parameter.h"

#include <set>
#include <string>

#include <iostream>
using namespace std;

namespace openfx {
namespace host {

namespace details {
typedef std::map<std::string, EParameterType> KnownTypeMap;
KnownTypeMap g_KnownTypes;
typedef std::set<EParameterType> SupportedTypeSet;
SupportedTypeSet g_SupportedTypes;
} // namespace details

using namespace details;

ParamSuiteImpl::ParamSuiteImpl() {
    paramDefine = &::openfx::host::paramDefine;
    paramGetHandle = &::openfx::host::paramGetHandle;
    paramSetGetPropertySet = &::openfx::host::paramSetGetPropertySet;
    paramGetPropertySet = &::openfx::host::paramGetPropertySet;
    paramGetValue = &::openfx::host::paramGetValue;
    paramGetValueAtTime = &::openfx::host::paramGetValueAtTime;
    paramGetDerivative = &::openfx::host::paramGetDerivative;
    paramGetIntegral = &::openfx::host::paramGetIntegral;
    paramSetValue = &::openfx::host::paramSetValue;
    paramSetValueAtTime = &::openfx::host::paramSetValueAtTime;
    paramGetNumKeys = &::openfx::host::paramGetNumKeys;
    paramGetKeyTime = &::openfx::host::paramGetKeyTime;
    paramGetKeyIndex = &::openfx::host::paramGetKeyIndex;
    paramDeleteKey = &::openfx::host::paramDeleteKey;
    paramDeleteAllKeys = &::openfx::host::paramDeleteAllKeys;
    paramCopy = &::openfx::host::paramCopy;
    paramEditBegin = &::openfx::host::paramEditBegin;
    paramEditEnd = &::openfx::host::paramEditEnd;

    g_KnownTypes[kOfxParamTypeInteger] = eIntParam;
    g_KnownTypes[kOfxParamTypeDouble] = eDoubleParam;
    g_KnownTypes[kOfxParamTypeBoolean] = eBooleanParam;
    g_KnownTypes[kOfxParamTypeChoice] = eChoiceParam;
    g_KnownTypes[kOfxParamTypeRGBA] = eRGBAParam;
    g_KnownTypes[kOfxParamTypeRGB] = eRGBParam;
    g_KnownTypes[kOfxParamTypeDouble2D] = eDouble2DParam;
    g_KnownTypes[kOfxParamTypeInteger2D] = eInt2DParam;
    g_KnownTypes[kOfxParamTypeDouble3D] = eDouble3DParam;
    g_KnownTypes[kOfxParamTypeInteger3D] = eInt3DParam;
    g_KnownTypes[kOfxParamTypeString] = eStringParam;
    g_KnownTypes[kOfxParamTypeCustom] = eCustomParam;
    g_KnownTypes[kOfxParamTypeGroup] = eGroupParam;
    g_KnownTypes[kOfxParamTypePage] = ePageParam;
    g_KnownTypes[kOfxParamTypePushButton] = ePushButtonParam;

    g_SupportedTypes.insert(eBooleanParam);
    g_SupportedTypes.insert(eIntParam);
    g_SupportedTypes.insert(eInt2DParam);
    g_SupportedTypes.insert(eInt3DParam);
    g_SupportedTypes.insert(eDoubleParam);
    g_SupportedTypes.insert(eDouble2DParam);
    g_SupportedTypes.insert(eDouble3DParam);
    g_SupportedTypes.insert(eStringParam);
}

#define STRINGIFY(x) #x

#define ReturnIfNull(what)                                                                  \
    if(what==NULL){                                                                         \
        cerr << STRINGIFY(what) << " is NULL" << endl;                                      \
        return kOfxStatErrBadHandle;                                                        \
    }

#define DeclareDescriptorAndReturnIfBadHandle                                               \
    ReturnIfNull(paramSet)                                                                  \
    if (dynamic_cast<AbstractParamSet*>(paramSet)->type != AbstractParamSet::eDescriptor){  \
        cerr << "OfxParamSetHandle is not of descriptor type" << endl;                      \
        return kOfxStatErrBadHandle;                                                        \
    }                                                                                       \
    ParameterDescriptor &descriptor = dynamic_cast<ParameterDescriptor&> (*paramSet);       \
    ParameterDescriptor::DescriptorMap &descriptorMap = descriptor.descriptors;

#define DeclareParameterSetAndReturnIfBadHandle                                             \
    ReturnIfNull(paramSet)                                                                  \
    if (dynamic_cast<AbstractParamSet*>(paramSet)->type != AbstractParamSet::eParameterSet){\
        cerr << "OfxParamSetHandle is not of parameter set type" << endl;                   \
        return kOfxStatErrBadHandle;                                                        \
    }                                                                                       \
    ParameterSet &parameterSet = dynamic_cast<ParameterSet&> (*paramSet);

#define DeclareParameterTypeAndReturnIfUnknown                                              \
    const KnownTypeMap::const_iterator knownTypeItr = g_KnownTypes.find(paramType);         \
    if (knownTypeItr == g_KnownTypes.end()) {                                               \
        cerr << "Unknown parameter type \"" << paramType << "\"" << endl;                   \
        return kOfxStatErrUnknown;                                                          \
    }                                                                                       \
    const EParameterType type = knownTypeItr->second;

#define ReturnIfUnsupportedType                                                             \
    if (g_SupportedTypes.find(type) == g_SupportedTypes.end()) {                            \
        cerr << "Host does not currently support type \""<<paramType<<"\""<< endl;          \
        return kOfxStatErrUnsupported;\
    }

template<typename T>
inline bool isNull(T* pointer, const char* type, const char* name, const char* function) {
    if (pointer != NULL)
        return false;
    cerr << "In function " << function << " : " << type << " " << name << " is NULL" << endl;
    return true;
}

/** @brief Defines a new parameter of the given type in a describe action
 *
 * \arg paramSet   handle to the parameter set descriptor that will hold this parameter
 * \arg paramType   type of the parameter to create, one of the kOfxParamType* #defines
 * \arg name        unique name of the parameter
 * \arg propertySet  if not null, a pointer to the parameter descriptor's property set will be placed here.
 *
 * This function defines a parameter in a parameter set and returns a property set which is used to describe that parameter.
 *
 * This function does not actually create a parameter, it only says that one should exist in any subsequent instances. To fetch an
 * parameter instance paramGetHandle must be called on an instance.
 *
 * This function can always be called in one of a plug-in's 'describe' functions which defines the parameter sets common to all instances of a plugin.
 *
 * @returns
 * - ::kOfxStatOK             - the parameter was created correctly
 * - ::kOfxStatErrBadHandle   - if the plugin handle was invalid
 * - ::kOfxStatErrExists      - if a parameter of that name exists already in this plugin
 * - ::kOfxStatErrUnknown     - if the type is unknown
 * - ::kOfxStatErrUnsupported - if the type is known but unsupported
 */
OfxStatus paramDefine(OfxParamSetHandle paramSet, const char *paramType, const char *name, OfxPropertySetHandle *propertySet) {
    DeclareParameterTypeAndReturnIfUnknown
    ReturnIfUnsupportedType
    ReturnIfNull(paramSet)
    DeclareDescriptorAndReturnIfBadHandle
    if (descriptorMap.find(name) != descriptorMap.end()) {
        cerr << "Parameter with name \"" << name << "\" already exists in this parameter descriptor";
        return kOfxStatErrExists;
    }
    typedef ParameterDescriptor::DescriptorMap DescriptorMap;
    const DescriptorMap::iterator itr = descriptorMap.insert(std::make_pair(name, new OfxPropertySetStruct())).first;
    if (propertySet != NULL)
        *propertySet = itr->second.get();
    return kOfxStatOK;
}

/** @brief Retrieves the handle for a parameter in a given parameter set
 *
 * \arg paramSet    instance of the plug-in to fetch the property handle from
 * \arg name        parameter to ask about
 * \arg param       pointer to a param handle, the value is returned here
 * \arg propertySet  if not null, a pointer to the parameter's property set will be placed here.
 *
 * Parameter handles retrieved from an instance are always distinct in each instance. The paramter handle is valid for the life-time of the instance. Parameter handles in instances are distinct from paramter handles in plugins. You cannot call this in a plugin's describe function, as it needs an instance to work on.
 *
 * @returns
 * - ::kOfxStatOK       - the parameter was found and returned
 * - ::kOfxStatErrBadHandle  - if the plugin handle was invalid
 * - ::kOfxStatErrUnknown    - if the type is unknown
 */
OfxStatus paramGetHandle(OfxParamSetHandle paramSet, const char *name, OfxParamHandle *param, OfxPropertySetHandle *propertySet) {
    DeclareParameterSetAndReturnIfBadHandle

    ParameterSet::ParameterMap::const_iterator itr = parameterSet.parameters.find(name);
    if (itr == parameterSet.parameters.end())
        return kOfxStatErrUnknown;
    return kOfxStatErrFatal;
}

OfxStatus paramSetGetPropertySet(OfxParamSetHandle paramSet, OfxPropertySetHandle *propHandle) {
    return kOfxStatErrFatal;
}

OfxStatus paramGetPropertySet(OfxParamHandle param, OfxPropertySetHandle *propHandle) {
    return kOfxStatErrFatal;
}

OfxStatus paramGetValue(OfxParamHandle paramHandle, ...) {
    return kOfxStatErrFatal;
}

OfxStatus paramGetValueAtTime(OfxParamHandle paramHandle, OfxTime time, ...) {
    return kOfxStatErrFatal;
}

OfxStatus paramGetDerivative(OfxParamHandle paramHandle, OfxTime time, ...) {
    return kOfxStatErrFatal;
}

OfxStatus paramGetIntegral(OfxParamHandle paramHandle, OfxTime time1, OfxTime time2, ...) {
    return kOfxStatErrFatal;
}

OfxStatus paramSetValue(OfxParamHandle paramHandle, ...) {
    return kOfxStatErrFatal;
}

OfxStatus paramSetValueAtTime(OfxParamHandle paramHandle, OfxTime time, ...) {
    return kOfxStatErrFatal;
}

OfxStatus paramGetNumKeys(OfxParamHandle paramHandle, unsigned int *numberOfKeys) {
    return kOfxStatErrFatal;
}

OfxStatus paramGetKeyTime(OfxParamHandle paramHandle, unsigned int nthKey, OfxTime *time) {
    return kOfxStatErrFatal;
}

OfxStatus paramGetKeyIndex(OfxParamHandle paramHandle, OfxTime time, int direction, int *index) {
    return kOfxStatErrFatal;
}

OfxStatus paramDeleteKey(OfxParamHandle paramHandle, OfxTime time) {
    return kOfxStatErrFatal;
}

OfxStatus paramDeleteAllKeys(OfxParamHandle paramHandle) {
    return kOfxStatErrFatal;
}

OfxStatus paramCopy(OfxParamHandle paramTo, OfxParamHandle paramFrom, OfxTime dstOffset, OfxRangeD *frameRange) {
    return kOfxStatErrFatal;
}

OfxStatus paramEditBegin(OfxParamSetHandle paramSet, const char *name) {
    return kOfxStatErrFatal;
}

OfxStatus paramEditEnd(OfxParamSetHandle paramSet) {
    return kOfxStatErrFatal;
}

} // namespace host
} // namespace openfx
