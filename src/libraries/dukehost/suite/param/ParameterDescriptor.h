/*
 * ParameterSet.h
 *
 *  Created on: Oct 9, 2010
 *      Author: Guillaume Chatelet
 */

#ifndef PARAMETERSET_H_
#define PARAMETERSET_H_

#include "AbstractParamSet.h"

#include <boost/unordered_map.hpp>

#include <string>

namespace openfx {
namespace host {

struct ParameterDescriptor : public AbstractParamSet {
    typedef boost::shared_ptr<OfxPropertySetStruct> PropertySetPtr;
    typedef boost::unordered_map<std::string, PropertySetPtr> DescriptorMap;

    DescriptorMap descriptors;

    ParameterDescriptor() :
        AbstractParamSet(eDescriptor) {
    }

    virtual ~ParameterDescriptor() {
    }
};

} // namespace host
} // namespace openfx

#endif /* PARAMETERSET_H_ */
