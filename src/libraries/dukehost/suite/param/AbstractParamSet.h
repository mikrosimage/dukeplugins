/*
 * AbstractParameterSet.h
 *
 *  Created on: Oct 9, 2010
 *      Author: Guillaume Chatelet
 */

#ifndef ABSTRACTPARAMSET_H_
#define ABSTRACTPARAMSET_H_

#include <dukehost/suite/property/PropertySet.h>

// linking our type with OpenFx blind Type
struct OfxParamSetStruct {
    virtual ~OfxParamSetStruct() {
    }
};

namespace openfx {
namespace host {

/**
 * For an unknown reason an OpenFx represents a descriptor of parameters
 * with the same structure as a parameter instance. This is pretty awkward.
 * To minimize the confusion we are defining here an abstract base for
 * the descriptor and the instance. This structure is then linked to the OpenFx
 * type OfxParamSetStruct
 */
struct AbstractParamSet : public OfxParamSetStruct {
    enum ParamSetType {
        eDescriptor, eParameterSet
    };

    const ParamSetType type;
    PropertySet propertySet;
    AbstractParamSet(enum ParamSetType type) :
        type(type) {
    }

    virtual ~AbstractParamSet() = 0;
};

} // namespace host
} // namespace openfx


#endif /* ABSTRACTPARAMSET_H_ */
