/*
 * main.cpp
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include <dukehost/suite/param/ParameterSet.h>
#include <dukehost/suite/param/ParameterDescriptor.h>
#include <dukehost/suite/param/ParamSuiteImpl.h>

#include <iostream>
#include <memory>

using namespace std;
using namespace openfx::host;

#define BOOST_TEST_MODULE ParameterTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( BoostParameterSuite )

BOOST_AUTO_TEST_CASE( ParamDefineFunction )
{
    ParameterDescriptor descriptor;
    ParameterSet badDescriptor;
    ParamSuiteImpl suite;
    OfxPropertySetHandle handle;
    // unknown type => kOfxStatErrUnknown
    BOOST_CHECK_EQUAL( suite._ParamDefine( &descriptor, "unknownType", "name", &handle), kOfxStatErrUnknown );
    // NULL descriptor handle => kOfxStatErrBadHandle
    BOOST_CHECK_EQUAL( suite._ParamDefine( NULL, kOfxParamTypeInteger, "name", &handle), kOfxStatErrBadHandle );
    // invalid descriptor handle => kOfxStatErrBadHandle
    BOOST_CHECK_EQUAL( suite._ParamDefine( &badDescriptor, kOfxParamTypeInteger, "name", &handle), kOfxStatErrBadHandle );
    // valid but unsupported type ( this test will be removed at some point )
    BOOST_CHECK_EQUAL( suite._ParamDefine( &descriptor, kOfxParamTypeCustom, "name", &handle), kOfxStatErrUnsupported );
    // successfully creating parameter
    BOOST_CHECK_EQUAL( suite._ParamDefine( &descriptor, kOfxParamTypeInteger, "duplicate name", &handle), kOfxStatOK );
    // creating twice the same parameter is not permitted
    BOOST_CHECK_EQUAL( suite._ParamDefine( &descriptor, kOfxParamTypeInteger, "duplicate name", &handle), kOfxStatErrExists );
}

BOOST_AUTO_TEST_SUITE_END()
