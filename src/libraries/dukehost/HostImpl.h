/*
 * HostImpl.h
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 *
 * This file provides default host implementations as well as
 * methods to easily register your own crafted Suites
 */

#ifndef HOSTIMPL_H_
#define HOSTIMPL_H_

#include <openfx/ofxCorePlugin.h>
#include "suite/SuiteHelper.h"
#include "suite/property/PropertySet.h"
#include "suite/property/PropertySuiteImpl.h"
#include <boost/noncopyable.hpp>

namespace openfx {
namespace host {

/**
 * Those global variable and function needs to be in a private namespace so
 * multiple host can coexist in the same compilation unit
 */
namespace {

// we have no choice but defining a global variable here because of the C
// nature of OpenFX. This variable will hold the available suites you want
// to provide as a Host
RegistereredSuites g_RegisteredSuites;

// the global function automatically registered in the host
// to provide Suites to plugins
void* gFetchSuite(OfxPropertySetHandle host, const char* suiteName, int suiteVersion) {
    const RegistereredSuites::const_iterator suiteItr = g_RegisteredSuites.find(SuiteDescription(suiteName, suiteVersion));
    if (suiteItr == g_RegisteredSuites.end()) return NULL;
    return suiteItr->second;
}

} // namespace

/**
 * Very simple host without support for properties
 * It can load plugins and execute actions on them but communication
 * between the host and plugin will be almost inexistent
 *
 * Please note this implementation will not be suitable
 * if you're intending to load commercial plugins as most
 * of them will need the Host to provide a PropertySet and
 * a PropertySuite implementation
 */
struct HostNoPropertySuiteImpl : public ::OfxHost, public boost::noncopyable {
    HostNoPropertySuiteImpl() {
        host = NULL;
        fetchSuite = &gFetchSuite;
    }
};

/**
 * More standard host with registered PropertySuite and implementation
 */
struct HostImpl : public HostNoPropertySuiteImpl {
    PropertySuiteV1Impl m_PropertySuite;
    PropertySet m_HostPropertySet;

    HostImpl() {
        host = &m_HostPropertySet;
        // registering the property suite
        g_RegisteredSuites[SuiteDescription(kOfxPropertySuite, 1)] = &m_PropertySuite;
    }
};

} // namespace host
} // namespace openfx

#endif /* HOSTIMPL_H_ */
