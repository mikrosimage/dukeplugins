/*
 * main.cpp
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include <dukehost/suite/property/Property.h>
#include <dukehost/suite/property/OstreamHelpers.h>

#include<memory>
#include<iostream>

using namespace std;
using namespace openfx::host;

#define BOOST_TEST_MODULE PropertyTests
#include <boost/test/unit_test.hpp>

const char* name = "unnamed";

BOOST_AUTO_TEST_SUITE( BoostProperty )

BOOST_AUTO_TEST_CASE( EmptyProperty )
{
    IntProperty property(name);
    BOOST_CHECK_EQUAL(property.size(), 0U);
    BOOST_CHECK_EQUAL(property.name, name);
}

BOOST_AUTO_TEST_CASE( PropertyType ) {
    BOOST_CHECK_EQUAL(IntProperty("int").type, eInt);
    BOOST_CHECK_EQUAL(DoubleProperty("double").type, eDouble);
    BOOST_CHECK_EQUAL(StringProperty("string").type, eString);
    BOOST_CHECK_EQUAL(PointerProperty("pointer").type, ePointer);
}

BOOST_AUTO_TEST_CASE( TestValueProperty )
{
    IntProperty property("");
    property.value.push_back(3);
    BOOST_CHECK_EQUAL(property.size(), 1U);
}

BOOST_AUTO_TEST_CASE( ResetNoDefaultValueProperty )
{
    IntProperty property("");
    property.value.push_back(3);
    property.reset();
    BOOST_CHECK_EQUAL(property.size(), 0U);
}

BOOST_AUTO_TEST_CASE( ResetToDefaultValueProperty )
{
    IntProperty property("");
    property.value.push_back(3);
    property.defaultValue.push_back(10);
    property.defaultValue.push_back(5);
    BOOST_CHECK_EQUAL(property.size(), 1U);
    BOOST_CHECK_EQUAL(property.value[0], 3);
    property.reset();
    BOOST_CHECK_EQUAL(property.size(), 2U);
    BOOST_CHECK_EQUAL(property.value[0], 10);
    BOOST_CHECK_EQUAL(property.value[1], 5);
}

BOOST_AUTO_TEST_CASE( EqualsBetweenSameTypeProperty )
{
    IntProperty p1(name);
    p1.value.push_back(3);

    IntProperty p2(name);

    BOOST_CHECK(p1 != p2);
    BOOST_CHECK((p1 == p2) == false);

    p2.value.push_back(3);
    BOOST_CHECK(p1 == p2);
    BOOST_CHECK((p1 != p2) == false);
}

BOOST_AUTO_TEST_CASE( EqualsBetweenDifferentTypesProperty )
{
    IntProperty intProperty(name);
    StringProperty stringProperty(name);
    BOOST_CHECK(intProperty != stringProperty);
    BOOST_CHECK(stringProperty != intProperty);
    BOOST_CHECK(!(intProperty == stringProperty));
    BOOST_CHECK(!(stringProperty == intProperty));
}

BOOST_AUTO_TEST_CASE( EqualsBetweenCloneAndType )
{
    IntProperty intProperty(name);
    std::auto_ptr<Property> pClone(intProperty.clone());

    BOOST_CHECK( *pClone == intProperty );
    BOOST_CHECK( intProperty == *pClone );

    intProperty.value.push_back(3);
    BOOST_CHECK( *pClone != intProperty );
    BOOST_CHECK( intProperty != *pClone );
}

BOOST_AUTO_TEST_CASE( EqualsBetweenClones )
{
    IntProperty intProperty(name);
    intProperty.defaultValue.push_back(3);
    std::auto_ptr<Property> pClone1(intProperty.clone());
    std::auto_ptr<Property> pClone2(intProperty.clone());

    BOOST_CHECK( *pClone1 == *pClone2 );
    BOOST_CHECK( *pClone2 == *pClone1 );

    pClone1->reset();

    BOOST_CHECK( *pClone1 != *pClone2 );
    BOOST_CHECK( *pClone2 != *pClone1 );
}

BOOST_AUTO_TEST_CASE( PropertySetTest )
{
    IntProperty i(name,5,2);
    DoubleProperty d(name,5,2);
    StringProperty s(name,"one","two");
    PointerProperty p(name);

    std::cout << i << d << s << p;
    PropertySet set;
    set << i << d << s << p;
    BOOST_CHECK( set.map.size() == 1 );
    BOOST_CHECK( set.map[name] == &p );
}

BOOST_AUTO_TEST_CASE( Ostreams )
{
    IntProperty i(name,5,2);
    DoubleProperty d(name,5,2);
    StringProperty s(name,"one","two");
    PointerProperty p(name);

    std::cout << i << d << s << p;
    PropertySet set;
    set << i << d << s << p;
    std::cout << set;
}

BOOST_AUTO_TEST_SUITE_END()
