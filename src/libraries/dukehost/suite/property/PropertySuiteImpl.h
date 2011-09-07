/*
 * PropertySuiteV1Impl.h
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 *
 * A default yet efficient implementation of the PropertySuite
 */

#ifndef PROPERTYSUITEV1IMPL_H_
#define PROPERTYSUITEV1IMPL_H_

#include "PropertySet.h"
#include <openfx/suite/ofxProperty.h>

namespace openfx {
namespace host {

OfxStatus propGetDimension(OfxPropertySetHandle properties, const char *property, int *count);
OfxStatus propGetDouble(OfxPropertySetHandle properties, const char *property, int index, double *value);
OfxStatus propGetDoubleN(OfxPropertySetHandle properties, const char *property, int count, double *value);
OfxStatus propGetInt(OfxPropertySetHandle properties, const char *property, int index, int *value);
OfxStatus propGetIntN(OfxPropertySetHandle properties, const char *property, int count, int *value);
OfxStatus propGetPointer(OfxPropertySetHandle properties, const char *property, int index, void **value);
OfxStatus propGetPointerN(OfxPropertySetHandle properties, const char *property, int count, void **value);
OfxStatus propGetString(OfxPropertySetHandle properties, const char *property, int index, char **value);
OfxStatus propGetStringN(OfxPropertySetHandle properties, const char *property, int count, char **value);
OfxStatus propReset(OfxPropertySetHandle properties, const char *property);
OfxStatus propSetDouble(OfxPropertySetHandle properties, const char *property, int index, double value);
OfxStatus propSetDoubleN(OfxPropertySetHandle properties, const char *property, int count, double *value);
OfxStatus propSetInt(OfxPropertySetHandle properties, const char *property, int index, int value);
OfxStatus propSetIntN(OfxPropertySetHandle properties, const char *property, int count, int *value);
OfxStatus propSetPointer(OfxPropertySetHandle properties, const char *property, int index, void *value);
OfxStatus propSetPointerN(OfxPropertySetHandle properties, const char *property, int count, void **value);
OfxStatus propSetString(OfxPropertySetHandle properties, const char *property, int index, const char *value);
OfxStatus propSetStringN(OfxPropertySetHandle properties, const char *property, int count, const char **value);

struct PropertySuiteV1Impl : public OfxPropertySuiteV1 {
    PropertySuiteV1Impl();

    /**
     * We are providing a set of helper functions to prevent developers from
     * dereferencing function pointers by themselves.
     * Actually the developer will prefer manipulating the provided PropertySet
     * and Property implementation rather than using those functions.
     */

    inline OfxStatus _PropGetDimension(OfxPropertySetHandle properties, const char *property, int *count) {
        return (*propGetDimension)(properties, property, count);
    }
    inline OfxStatus _PropGetDouble(OfxPropertySetHandle properties, const char *property, int index, double *value) {
        return (*propGetDouble)(properties, property, index, value);
    }
    inline OfxStatus _PropGetDoubleN(OfxPropertySetHandle properties, const char *property, int count, double *value) {
        return (*propGetDoubleN)(properties, property, count, value);
    }
    inline OfxStatus _PropGetInt(OfxPropertySetHandle properties, const char *property, int index, int *value) {
        return (*propGetInt)(properties, property, index, value);
    }
    inline OfxStatus _PropGetIntN(OfxPropertySetHandle properties, const char *property, int count, int *value) {
        return (*propGetIntN)(properties, property, count, value);
    }
    inline OfxStatus _PropGetPointer(OfxPropertySetHandle properties, const char *property, int index, void **value) {
        return (*propGetPointer)(properties, property, index, value);
    }
    inline OfxStatus _PropGetPointerN(OfxPropertySetHandle properties, const char *property, int count, void **value) {
        return (*propGetPointerN)(properties, property, count, value);
    }
    inline OfxStatus _PropGetString(OfxPropertySetHandle properties, const char *property, int index, char **value) {
        return (*propGetString)(properties, property, index, value);
    }
    inline OfxStatus _PropGetStringN(OfxPropertySetHandle properties, const char *property, int count, char **value) {
        return (*propGetStringN)(properties, property, count, value);
    }
    inline OfxStatus _PropReset(OfxPropertySetHandle properties, const char *property) {
        return (*propReset)(properties, property);
    }
    inline OfxStatus _PropSetDouble(OfxPropertySetHandle properties, const char *property, int index, double value) {
        return (*propSetDouble)(properties, property, index, value);
    }
    inline OfxStatus _PropSetDoubleN(OfxPropertySetHandle properties, const char *property, int count, double *value) {
        return (*propSetDoubleN)(properties, property, count, value);
    }
    inline OfxStatus _PropSetInt(OfxPropertySetHandle properties, const char *property, int index, int value) {
        return (*propSetInt)(properties, property, index, value);
    }
    inline OfxStatus _PropSetIntN(OfxPropertySetHandle properties, const char *property, int count, int *value) {
        return (*propSetIntN)(properties, property, count, value);
    }
    inline OfxStatus _PropSetPointer(OfxPropertySetHandle properties, const char *property, int index, void *value) {
        return (*propSetPointer)(properties, property, index, value);
    }
    inline OfxStatus _PropSetPointerN(OfxPropertySetHandle properties, const char *property, int count, void **value) {
        return (*propSetPointerN)(properties, property, count, value);
    }
    inline OfxStatus _PropSetString(OfxPropertySetHandle properties, const char *property, int index, const char *value) {
        return (*propSetString)(properties, property, index, value);
    }
    inline OfxStatus _PropSetStringN(OfxPropertySetHandle properties, const char *property, int count, const char **value) {
        return (*propSetStringN)(properties, property, count, value);
    }
};

} // namespace host
} // namespace openfx

#endif /* PROPERTYSUITEV1IMPL_H_ */
