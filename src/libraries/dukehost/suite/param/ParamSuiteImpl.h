/*
 * ParamSuiteImpl.h
 *
 *  Created on: Oct 9, 2010
 *      Author: Guillaume Chatelet
 */

#ifndef PARAMSUITEIMPL_H_
#define PARAMSUITEIMPL_H_

#include <openfx/suite/ofxParam.h>

namespace openfx {
namespace host {

OfxStatus paramDefine(OfxParamSetHandle paramSet, const char *paramType, const char *name, OfxPropertySetHandle *propertySet);
OfxStatus paramGetHandle(OfxParamSetHandle paramSet, const char *name, OfxParamHandle *param, OfxPropertySetHandle *propertySet);
OfxStatus paramSetGetPropertySet(OfxParamSetHandle paramSet, OfxPropertySetHandle *propHandle);
OfxStatus paramGetPropertySet(OfxParamHandle param, OfxPropertySetHandle *propHandle);
OfxStatus paramGetValue(OfxParamHandle paramHandle, ...);
OfxStatus paramGetValueAtTime(OfxParamHandle paramHandle, OfxTime time, ...);
OfxStatus paramGetDerivative(OfxParamHandle paramHandle, OfxTime time, ...);
OfxStatus paramGetIntegral(OfxParamHandle paramHandle, OfxTime time1, OfxTime time2, ...);
OfxStatus paramSetValue(OfxParamHandle paramHandle, ...);
OfxStatus paramSetValueAtTime(OfxParamHandle paramHandle, OfxTime time/* in frames */, ...);
OfxStatus paramGetNumKeys(OfxParamHandle paramHandle, unsigned int *numberOfKeys);
OfxStatus paramGetKeyTime(OfxParamHandle paramHandle, unsigned int nthKey, OfxTime *time);
OfxStatus paramGetKeyIndex(OfxParamHandle paramHandle, OfxTime time, int direction, int *index);
OfxStatus paramDeleteKey(OfxParamHandle paramHandle, OfxTime time);
OfxStatus paramDeleteAllKeys(OfxParamHandle paramHandle);
OfxStatus paramCopy(OfxParamHandle paramTo, OfxParamHandle paramFrom, OfxTime dstOffset, OfxRangeD *frameRange);
OfxStatus paramEditBegin(OfxParamSetHandle paramSet, const char *name);
OfxStatus paramEditEnd(OfxParamSetHandle paramSet);

class ParamSuiteImpl : public OfxParameterSuiteV1 {
public:
    ParamSuiteImpl();

    /**
     * We are providing a set of helper functions to prevent developers from
     * dereferencing function pointers by themselves.
     * Actually the developer will prefer manipulating the provided PropertySet
     * and Property implementation rather than using those functions.
     */

    OfxStatus _ParamDefine(OfxParamSetHandle paramSet, const char *paramType, const char *name, OfxPropertySetHandle *propertySet){
        return (*paramDefine)(paramSet, paramType, name, propertySet);
    }

};

} // namespace host
} // namespace openfx

#endif /* PARAMSUITEIMPL_H_ */
