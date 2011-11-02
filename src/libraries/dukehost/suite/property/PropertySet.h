/*
 * PropertySet.h
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 *
 * Implementation of a PropertySet and definition of this implementation
 * as a struct OfxPropertySetStruct so it can be used as an OpenFX PropertySet
 */

#ifndef PROPERTYSET_H_
#define PROPERTYSET_H_

#include "Property.h"
#include <string>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

// linking our type with OpenFx blind Type
struct OfxPropertySetStruct {
    virtual ~OfxPropertySetStruct() {
    }
};

namespace openfx {
namespace host {

/**
 * This structure is not yet stable and might change to make it
 * more convenient to developers.
 *
 * Please note :
 *
 * > the members are public on purpose
 *      it prevents a lot of 'friend' declaration boilerplate
 *
 * > this implementation does not manage Properties life cycle
 *      you have to ensure the properties are properly allocated
 *      and remains allocated during the use of this PropertySet
 */
struct PropertySet : OfxPropertySetStruct {
    typedef boost::unordered_map<std::string, openfx::host::Property*> MAP_TYPE;
    MAP_TYPE map;

    /**
     * helper function to put properties into this PropertySet
     */
    inline void put(openfx::host::Property &property) {
        map[property.name] = &property;
    }

    /**
     * another helper to insert quickly several Properties into a
     * PropertySet, ie :
     *
     * IntParamValue one("one");
     * IntParamValue two("two");
     * IntParamValue three("three");
     * PropertySet set;
     * set << one << two << three;
     */
    PropertySet& operator <<(openfx::host::Property &property) {
        put(property);
        return *this;
    }
};

} // namespace host
} // namespace openfx


#endif /* PROPERTYSET_H_ */
