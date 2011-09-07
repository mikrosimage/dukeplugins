/*
 * OstreamHelpers.cpp
 *
 *  Created on: Oct 2, 2010
 *      Author: Guillaume Chatelet
 */

#include "OstreamHelpers.h"

#include <iterator>
#include <algorithm>

template<typename TYPE>
std::ostream& operator <<(std::ostream &os, const ::openfx::host::TProperty<TYPE> &prop) {
    using namespace openfx::host;
    switch (prop.type) {
        case eInt:
            os << "int     ";
            break;
        case eDouble:
            os << "double  ";
            break;
        case ePointer:
            os << "ptr     ";
            break;
        case eString:
            os << "string  ";
            break;
        case eNone:
            os << "eNone   ";
            break;
        default:
            break;
    }
    os << "'" << prop.name << "'" << std::endl;
    typename TProperty<TYPE>::ValueType ValueType;
    std::ostream_iterator<TYPE> osItr(os, " ");
    os << "- value   : [ ";
    std::copy(prop.value.begin(), prop.value.end(), osItr);
    os << "]" << std::endl;
    os << "- default : [ ";
    std::copy(prop.defaultValue.begin(), prop.defaultValue.end(), osItr);
    os << "]" << std::endl;
    return os;
}

std::ostream& operator <<(std::ostream &os, const ::openfx::host::Property &prop) {
    using namespace openfx::host;
    switch (prop.type) {
        case eInt: {
            const IntProperty& typedProp = dynamic_cast<const IntProperty&> (prop);
            os << typedProp;
            break;
        }
        case eDouble: {
            const DoubleProperty& typedProp = dynamic_cast<const DoubleProperty&> (prop);
            os << typedProp;
            break;
        }
        case ePointer: {
            const PointerProperty& typedProp = dynamic_cast<const PointerProperty&> (prop);
            os << typedProp;
            break;
        }
        case eString: {
            const StringProperty& typedProp = dynamic_cast<const StringProperty&> (prop);
            os << typedProp;
            break;
        }
        default:
            break;
    }
    return os;
}

std::ostream& operator <<(std::ostream &os, const ::openfx::host::PropertySet &propSet) {
    ::openfx::host::PropertySet::MAP_TYPE::const_iterator itr;
    for (itr = propSet.map.begin(); itr != propSet.map.end(); ++itr)
        os << *(itr->second);
    return os;
}
