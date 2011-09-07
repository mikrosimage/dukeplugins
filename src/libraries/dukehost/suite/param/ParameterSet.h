/*
 * ParameterSet.h
 *
 *  Created on: Oct 9, 2010
 *      Author: Guillaume Chatelet
 */

#ifndef PARAMETERDESCRIPTOR_H_
#define PARAMETERDESCRIPTOR_H_

#include "AbstractParamSet.h"
#include "Parameter.h"

#include <boost/unordered_map.hpp>

#include <string>

namespace openfx {
namespace host {

struct ParameterSet : public AbstractParamSet {
    typedef boost::shared_ptr<Parameter> ParameterPtr;
    typedef boost::unordered_map<std::string, ParameterPtr> ParameterMap;

    ParameterMap parameters;
    std::string editParameterName;

    ParameterSet() :
        AbstractParamSet(eParameterSet) {
    }

    virtual ~ParameterSet() {
    }
};

} // namespace host
} // namespace openfx

#endif /* PARAMETERDESCRIPTOR_H_ */
