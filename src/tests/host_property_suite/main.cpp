/*
 * main.cpp
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include <dukehost/suite/property/PropertySuiteImpl.h>

#include<memory>
#include<iostream>

using namespace std;
using namespace openfx::host;

#define BOOST_TEST_MODULE PropertySuiteTests
#include <boost/test/unit_test.hpp>

const char* myPropertyName = "my_property";
//
// Testing the OpenFx PropertySuite defined here
// http://openfx.sourceforge.net/Documentation/1.2/ofxProgrammingReference.html#PropertySuiteReference
//
BOOST_AUTO_TEST_SUITE( BoostPropertySuite )

BOOST_AUTO_TEST_CASE( CallFunctionOnUnknownPropertyTest )
{
	// declare property set
	PropertySet propertySet;

	PropertySuiteV1Impl impl; // registers functions
	int count=-1;
	BOOST_CHECK_EQUAL( kOfxStatErrUnknown, impl._PropGetDimension(&propertySet, "myUnknownProperty", &count));
}

BOOST_AUTO_TEST_CASE( AddPropertyWithAndWithoutValueTest )
{
	// declare property set
	PropertySet propertySet;
	IntProperty property(myPropertyName);
	propertySet.map[property.name] = &property;

	PropertySuiteV1Impl impl; // registers functions
	int count=-1;
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetDimension(&propertySet, myPropertyName, &count));
	BOOST_CHECK_EQUAL( count, 0);

	property.value.push_back(3);
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetDimension(&propertySet, myPropertyName, &count));
	BOOST_CHECK_EQUAL( count, 1);
}

BOOST_AUTO_TEST_CASE( GetIntValueFromPropertyTest )
{
	// declare property set
	PropertySet propertySet;
	IntProperty property(myPropertyName);
	propertySet.map[property.name] = &property;

	PropertySuiteV1Impl impl; // registers functions
	int storedValue=-1;

	// get stored value
	property.value.push_back(3);
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetInt(&propertySet, myPropertyName, 0, &storedValue));
	BOOST_CHECK_EQUAL( storedValue, 3);

	// get at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropGetInt(&propertySet, myPropertyName, 1, &storedValue));

	// get with bad value type
	double badValueType = -1.0;
	BOOST_CHECK_EQUAL( kOfxStatErrBadHandle, impl._PropGetDouble(&propertySet, myPropertyName, 0, &badValueType));

	// set new value
	int newValue = 12345;
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropSetInt(&propertySet, myPropertyName, 0, newValue));

	// get stored value
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetInt(&propertySet, myPropertyName, 0, &storedValue));
	BOOST_CHECK_EQUAL( storedValue, newValue);

	// set at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropSetInt(&propertySet, myPropertyName, 1, newValue));

	// set with bad value type
	BOOST_CHECK_EQUAL( kOfxStatErrBadHandle, impl._PropSetDouble(&propertySet, myPropertyName, 0, badValueType));
}

BOOST_AUTO_TEST_CASE( GetDoubleValueFromPropertyTest )
{
	// declare property set
	PropertySet propertySet;
	DoubleProperty property(myPropertyName);
	propertySet.map[property.name] = &property;

	PropertySuiteV1Impl impl; // registers functions
	double storedValue=-1;

	// get stored value
	property.value.push_back(3.123);
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetDouble(&propertySet, myPropertyName, 0, &storedValue));
	BOOST_CHECK_EQUAL( storedValue, 3.123);

	// get at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropGetDouble(&propertySet, myPropertyName, 1, &storedValue));

	// get with bad value type
	int badValueType = -1;
	BOOST_CHECK_EQUAL( kOfxStatErrBadHandle, impl._PropGetInt(&propertySet, myPropertyName, 0, &badValueType));

	// set new value
	double newValue = 123.45;
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropSetDouble(&propertySet, myPropertyName, 0, newValue));

	// get stored value
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetDouble(&propertySet, myPropertyName, 0, &storedValue));
	BOOST_CHECK_EQUAL( storedValue, newValue);

	// set at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropSetDouble(&propertySet, myPropertyName, 1, newValue));

	// set with bad value type
	BOOST_CHECK_EQUAL( kOfxStatErrBadHandle, impl._PropSetInt(&propertySet, myPropertyName, 0, badValueType));
}

BOOST_AUTO_TEST_CASE( GetStringValueFromPropertyTest )
{
	// declare property set
	PropertySet propertySet;
	StringProperty property(myPropertyName);
	propertySet.map[property.name] = &property;

	PropertySuiteV1Impl impl; // registers functions
	char * storedValue = NULL;

	// get stored value
	property.value.push_back("Hello,");
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetString(&propertySet, myPropertyName, 0, &storedValue));
	BOOST_CHECK_EQUAL( storedValue, "Hello,");

	// get at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropGetString(&propertySet, myPropertyName, 1, &storedValue));

	// get with bad value type
	double badValueType = -1.0;
	BOOST_CHECK_EQUAL( kOfxStatErrBadHandle, impl._PropGetDouble(&propertySet, myPropertyName, 0, &badValueType));

	// set new value
	const char * newValue = " World!";
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropSetString(&propertySet, myPropertyName, 0, newValue));

	// get stored value
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetString(&propertySet, myPropertyName, 0, &storedValue));
	BOOST_CHECK_EQUAL( storedValue, newValue);

	// set at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropSetString(&propertySet, myPropertyName, 1, newValue));

	// set with bad value type
	BOOST_CHECK_EQUAL( kOfxStatErrBadHandle, impl._PropSetDouble(&propertySet, myPropertyName, 0, badValueType));
}

BOOST_AUTO_TEST_CASE( GetPointerValueFromPropertyTest )
{
	// declare property set
	PropertySet propertySet;
	PointerProperty property(myPropertyName);
	propertySet.map[property.name] = &property;

	PropertySuiteV1Impl impl; // registers functions
	void * storedValue = NULL;

	// get stored value
	property.value.push_back((void*)0x123456);
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetPointer(&propertySet, myPropertyName, 0, &storedValue));
	BOOST_CHECK_EQUAL( storedValue, (void*)0x123456);
	BOOST_CHECK_NE( storedValue, (void*)NULL);

	// get at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropGetPointer(&propertySet, myPropertyName, 1, &storedValue));

	// get with bad value type
	double badValueType = -1.0;
	BOOST_CHECK_EQUAL( kOfxStatErrBadHandle, impl._PropGetDouble(&propertySet, myPropertyName, 0, &badValueType));

	// set new value
	void * newValue = (void*)0x112233;
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropSetPointer(&propertySet, myPropertyName, 0, newValue));

	// get stored value
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetPointer(&propertySet, myPropertyName, 0, &storedValue));
	BOOST_CHECK_EQUAL( storedValue, newValue);

	// set at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropSetPointer(&propertySet, myPropertyName, 1, newValue));

	// set with bad value type
	BOOST_CHECK_EQUAL( kOfxStatErrBadHandle, impl._PropSetDouble(&propertySet, myPropertyName, 0, badValueType));
}

BOOST_AUTO_TEST_CASE( GetIntValueRangeFromPropertyTest )
{
	// declare property set
	PropertySet propertySet;
	IntProperty property(myPropertyName);
	propertySet.map[property.name] = &property;

	PropertySuiteV1Impl impl; // registers functions
	int storedValue[] = {-1,-1,-1,-1};

	// get stored value
	property.value.push_back(0);
	property.value.push_back(1);
	property.value.push_back(2);
	property.value.push_back(3);
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetIntN(&propertySet, myPropertyName, 4, storedValue));
	BOOST_CHECK_EQUAL( storedValue[0], 0);
	BOOST_CHECK_EQUAL( storedValue[1], 1);
	BOOST_CHECK_EQUAL( storedValue[2], 2);
	BOOST_CHECK_EQUAL( storedValue[3], 3);

	// get at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropGetIntN(&propertySet, myPropertyName, 5, storedValue));
}

BOOST_AUTO_TEST_CASE( GetDoubleValueRangeFromPropertyTest )
{
	// declare property set
	PropertySet propertySet;
	DoubleProperty property(myPropertyName);
	propertySet.map[property.name] = &property;

	PropertySuiteV1Impl impl; // registers functions
	double storedValue[] = {-1.0,-1.0,-1.0,-1.0};

	// get stored value
	property.value.push_back(0.0);
	property.value.push_back(1.0);
	property.value.push_back(2.0);
	property.value.push_back(3.0);
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetDoubleN(&propertySet, myPropertyName, 4, storedValue));
	BOOST_CHECK_EQUAL( storedValue[0], 0.0);
	BOOST_CHECK_EQUAL( storedValue[1], 1.0);
	BOOST_CHECK_EQUAL( storedValue[2], 2.0);
	BOOST_CHECK_EQUAL( storedValue[3], 3.0);

	// get at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropGetDoubleN(&propertySet, myPropertyName, 5, storedValue));
}

BOOST_AUTO_TEST_CASE( GetStringValueRangeFromPropertyTest )
{
	// declare property set
	PropertySet propertySet;
	StringProperty property(myPropertyName);
	propertySet.map[property.name] = &property;

	PropertySuiteV1Impl impl; // registers functions
	char * storedValue[4];

	// get stored value
	property.value.push_back("a");
	property.value.push_back("b");
	property.value.push_back("c");
	property.value.push_back("d");
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetStringN(&propertySet, myPropertyName, 4, storedValue));
	BOOST_CHECK_EQUAL( storedValue[0], "a");
	BOOST_CHECK_EQUAL( storedValue[1], "b");
	BOOST_CHECK_EQUAL( storedValue[2], "c");
	BOOST_CHECK_EQUAL( storedValue[3], "d");

	// get at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropGetStringN(&propertySet, myPropertyName, 5, storedValue));
}

BOOST_AUTO_TEST_CASE( GetPointerValueRangeFromPropertyTest )
{
	// declare property set
	PropertySet propertySet;
	PointerProperty property(myPropertyName);
	propertySet.map[property.name] = &property;

	PropertySuiteV1Impl impl; // registers functions
	void * storedValue[] = {(void*)0x000000,(void*)0x000000,(void*)0x000000,(void*)0x000000};

	// get stored value
	property.value.push_back((void*)0x000000);
	property.value.push_back((void*)0x111111);
	property.value.push_back((void*)0x222222);
	property.value.push_back((void*)0x333333);
	BOOST_CHECK_EQUAL( kOfxStatOK, impl._PropGetPointerN(&propertySet, myPropertyName, 4, storedValue));
	BOOST_CHECK_EQUAL( storedValue[0], (void*)0x000000);
	BOOST_CHECK_EQUAL( storedValue[1], (void*)0x111111);
	BOOST_CHECK_EQUAL( storedValue[2], (void*)0x222222);
	BOOST_CHECK_EQUAL( storedValue[3], (void*)0x333333);

	// get at bad index
	BOOST_CHECK_EQUAL( kOfxStatErrBadIndex, impl._PropGetPointerN(&propertySet, myPropertyName, 5, storedValue));
}

BOOST_AUTO_TEST_SUITE_END()
