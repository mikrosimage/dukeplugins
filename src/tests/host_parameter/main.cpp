/*
 * main.cpp
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include <dukehost/suite/param/Parameter.h>

#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>

#include <iostream>
#include <memory>

using namespace std;
using namespace openfx::host;

#define BOOST_TEST_MODULE ParameterTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( BoostParameter )

BOOST_AUTO_TEST_CASE( TypeParameter )
{
    BOOST_CHECK_EQUAL( StringParamValue().type , eStringParam );
    BOOST_CHECK_EQUAL( IntParamValue().type , eIntParam );
    BOOST_CHECK_EQUAL( Int2DParamValue().type , eInt2DParam );
    BOOST_CHECK_EQUAL( Int3DParamValue().type , eInt3DParam );
    BOOST_CHECK_EQUAL( DoubleParamValue().type , eDoubleParam );
    BOOST_CHECK_EQUAL( Double2DParamValue().type , eDouble2DParam );
    BOOST_CHECK_EQUAL( Double3DParamValue().type , eDouble3DParam );
    BOOST_CHECK_EQUAL( BooleanParamValue().type , eBooleanParam );
}

BOOST_AUTO_TEST_CASE( DefaultValueParameter )
{
    BOOST_CHECK_EQUAL( StringParamValue().value.get<0>() , "" );
    BOOST_CHECK_EQUAL( StringParamValue("test").value.get<0>() , "test" );
    BOOST_CHECK_EQUAL( IntParamValue().value.get<0>() , 0 );
    BOOST_CHECK_EQUAL( IntParamValue(1).value.get<0>() , 1 );
    Int2DParamValue p0(2,3);
    BOOST_CHECK_EQUAL( p0.value.get<0>() , 2 );
    BOOST_CHECK_EQUAL( p0.value.get<1>() , 3 );
    Int3DParamValue p1(4,5,6);
    BOOST_CHECK_EQUAL( p1.value.get<0>() , 4 );
    BOOST_CHECK_EQUAL( p1.value.get<1>() , 5 );
    BOOST_CHECK_EQUAL( p1.value.get<2>() , 6 );
    BOOST_CHECK_EQUAL( DoubleParamValue(7).value.get<0>() , 7 );
    Double2DParamValue p2(8,9);
    BOOST_CHECK_EQUAL( p2.value.get<0>() , 8 );
    BOOST_CHECK_EQUAL( p2.value.get<1>() , 9 );
    Double3DParamValue p3(10,11,12);
    BOOST_CHECK_EQUAL( p3.value.get<0>() , 10 );
    BOOST_CHECK_EQUAL( p3.value.get<1>() , 11 );
    BOOST_CHECK_EQUAL( p3.value.get<2>() , 12 );
    BOOST_CHECK_EQUAL( BooleanParamValue().value.get<0>() , false );
    BOOST_CHECK_EQUAL( BooleanParamValue(true).value.get<0>() , true );
}

BOOST_AUTO_TEST_CASE( EqualityParameter )
{
    IntParamValue p0;
    IntParamValue p1(1);
    BOOST_CHECK( p0 == p0 );
    BOOST_CHECK( p0 != p1 );
}

BOOST_AUTO_TEST_CASE( CloneParameter )
{
    IntParamValue p0(123);
    auto_ptr<ParameterValue> pValue( p0.clone() );
    BOOST_CHECK( p0 == *pValue );
    const IntParamValue *pClone = dynamic_cast<const IntParamValue*>(pValue.get());
    BOOST_CHECK( pClone->value.get<0>() == 123 );
}

BOOST_AUTO_TEST_SUITE_END()
