/*
 * PropertySuiteImpl.h
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef PROPERTYSUITEIMPL_H_
#define PROPERTYSUITEIMPL_H_

#include <openfx/suite/ofxProperty.h>
#include <dukeplugin/suite/SuiteFetcher.h>

namespace openfx {
namespace plugin {

// forward declaration
struct PropertySuiteImpl;

/**
 * The propertySuite is C low level and not quite easy to use.
 * This class encapsulate the PropertySuite implementation and the
 * PropertySet to refer to so you can interact with the property set
 * very easily.
 *
 * Get an instance of this object through the PropertySuiteImpl object
 * defined below.
 */
class PropertyHelper {
    friend struct PropertySuiteImpl;

    const PropertySuiteImpl &m_Suite;
    const OfxPropertySetHandle m_Handle;

    PropertyHelper(const PropertySuiteImpl &PropertySuite, const OfxPropertySetHandle handle) :
        m_Suite(PropertySuite), m_Handle(handle) {
        assert(handle); ///<ensuring handle is not NULL
    }
public:
    //
    // PLEASE NOTE
    // We deliberately renamed these functions and reordered the arguments
    // so for a plugin developer to be as concise and efficient as possible.
    //
    // Remember this object is intended to be a helper.
    //

    int dimension(const char *property) const;
    bool reset(const char *property);

    int getInt(const char *property, int index = 0) const;
    double getDouble(const char *property, int index = 0) const;
    char* getString(const char *property, int index = 0) const;
    void* getPointer(const char *property, int index = 0) const;

    void getIntN(const char *property, int *value, int count) const;
    void getDoubleN(const char *property, double *value, int count) const;
    void getStringN(const char *property, char **value, int count) const;
    void getPointerN(const char *property, void **value, int count) const;

    void setInt(const char *property, int value, int index = 0);
    void setDouble(const char *property, double value, int index = 0);
    void setString(const char *property, const char *value, int index = 0);
    void setPointer(const char *property, void *value, int index = 0);

    void setIntN(const char *property, int *value, int count);
    void setDoubleN(const char *property, double *value, int count);
    void setStringN(const char *property, const char **value, int count);
    void setPointerN(const char *property, void **value, int count);
};

/**
 * Implementation of the PropertySuiteV1 as defined here
 * http://openfx.sourceforge.net/Documentation/1.2/ofxProgrammingReference.html#PropertySuiteReference
 */
struct PropertySuiteImpl : public SuiteFetcher<OfxPropertySuiteV1> {
    PropertySuiteImpl(const IPlugin &plugin) :
      SuiteFetcher<OfxPropertySuiteV1>(plugin, kOfxPropertySuite, 1) {
    }

    /**
     * returns a helper to the OfxHost.host property set
     * this function might throw std::runtime_error if the host does not
     * provide a propertySet
     */
    PropertyHelper getHostHelper() const {
        return PropertyHelper(*this, getHost().host);
    }

    /**
     * returns a helper to a particular propertySet.
     *
     * for instance you can use it to get/set properties from
     * the inArgs/outArgs of the plugin's actions.
     */
    PropertyHelper getHelper(OfxPropertySetHandle propertySet) const {
        return PropertyHelper(*this, propertySet);
    }
};

} // namespace plugin
} // namespace openfx

#include "PropertySuiteImpl.inl"

#endif /* PROPERTYSUITEIMPL_H_ */
