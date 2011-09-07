/*
 * main.cpp
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include <dukehost/HostImpl.h>
#include <dukehost/suite/property/PropertySet.h>
#include <dukehost/suite/property/PropertySuiteImpl.h>
#include <dukeplugin/suite/property/PropertySuiteImpl.h>
#include <dukeplugin/suite/OfxPluginExceptions.h>

const char* propertyName = "name";

void* fetchNoSuite(OfxPropertySetHandle host, const char* suiteName, int suiteVersion) {
    return NULL;
}

struct MyPlugin : public IPlugin {
    virtual OfxStatus pluginMain(const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs) {
        return kOfxStatReplyDefault;
    }
};

struct TEST_ACCESSOR {
    MyPlugin plugin;
    TEST_ACCESSOR(OfxHost *pHost) {
        plugin.m_pHost = pHost;
    }
};

using namespace ::openfx::host;


#define BOOST_TEST_MODULE PropertySuiteTests
#include <boost/test/unit_test.hpp>

//
// Testing the OpenFx PropertySuite from the plugin side
//
BOOST_AUTO_TEST_SUITE( BoostPluginPropertySuite )

BOOST_AUTO_TEST_CASE( FetchingInexistentSuiteTest )
{
    PropertySet propertySet;
    OfxHost host;
    host.host = &propertySet;
    host.fetchSuite = &::fetchNoSuite;
    TEST_ACCESSOR pluginHolder(&host);
    openfx::plugin::PropertySuiteImpl propertySuite(pluginHolder.plugin);
    BOOST_CHECK(!propertySuite.isAvailable());
    // getting suite if suite is inexistent should throw
    BOOST_CHECK_THROW(propertySuite.getSuite(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE( FetchingSuiteTest )
{
    openfx::host::HostImpl host;
    PropertySet &propertySet = host.m_HostPropertySet;

    openfx::host::IntProperty prop(propertyName);
    propertySet.map[propertyName] = &prop;

    TEST_ACCESSOR pluginHolder(&host);
    openfx::plugin::PropertySuiteImpl propertySuite(pluginHolder.plugin);

    BOOST_CHECK(propertySuite.isAvailable());
}

BOOST_AUTO_TEST_CASE( CheckBadPropertyThrowTest )
{
    openfx::host::HostImpl host;
    PropertySet &propertySet = host.m_HostPropertySet;

    // declaring standard properties
    const char* intName = "i";
    const char* doubleName = "d";
    const char* pointerName = "v";
    const char* stringName = "s";
    openfx::host::IntProperty intProp(intName);
    intProp.value.push_back(3);
    intProp.defaultValue.push_back(3);
    intProp.defaultValue.push_back(4);
    openfx::host::StringProperty stringProp(stringName);
    stringProp.value.push_back("three");
    stringProp.defaultValue.push_back("three");
    stringProp.defaultValue.push_back("four");
    openfx::host::PointerProperty pointerProp(pointerName);
    pointerProp.value.push_back((void*)3);
    pointerProp.defaultValue.push_back((void*)3);
    pointerProp.defaultValue.push_back((void*)4);
    openfx::host::DoubleProperty doubleProp(doubleName);
    doubleProp.value.push_back(3);
    doubleProp.defaultValue.push_back(3);
    doubleProp.defaultValue.push_back(4);
    propertySet.map[intName] = &intProp;
    propertySet.map[stringName] = &stringProp;
    propertySet.map[pointerName] = &pointerProp;
    propertySet.map[doubleName] = &doubleProp;

    TEST_ACCESSOR pluginHolder(&host);
    openfx::plugin::PropertySuiteImpl propertySuite(pluginHolder.plugin);
    openfx::plugin::PropertyHelper helper = propertySuite.getHostHelper();

    /**
     * Checking if unknown_object is thrown when accessing bad property
     */
    const char* badProperty = "bad";
    BOOST_CHECK_THROW( helper.setPointer(badProperty,NULL), openfx::plugin::exception::unknown_object );
    BOOST_CHECK_THROW( helper.setString(badProperty,NULL), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.setDouble(badProperty,0), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.setInt(badProperty,0), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.setPointerN(badProperty,NULL,0), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.setStringN(badProperty,NULL,0), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.setDoubleN(badProperty,0,0), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.setIntN(badProperty,0,0), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.reset(badProperty), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.getPointer(badProperty), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.getString(badProperty), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.getDouble(badProperty), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.getInt(badProperty), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.getPointerN(badProperty,NULL,0), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.getStringN(badProperty,NULL,0), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.getDoubleN(badProperty,NULL,0), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.getIntN(badProperty,NULL,0), openfx::plugin::exception::unknown_object);
    BOOST_CHECK_THROW( helper.dimension(badProperty), openfx::plugin::exception::unknown_object);

    /**
     * checking get/set
     */
    BOOST_CHECK_EQUAL( helper.getInt(intName,0) ,3);
    helper.setInt(intName,5);
    BOOST_CHECK_EQUAL( helper.getInt(intName,0) ,5);

    BOOST_CHECK_EQUAL( helper.getString(stringName,0) ,"three");
    helper.setString(stringName,"five");
    BOOST_CHECK_EQUAL( helper.getString(stringName,0) ,"five");

    BOOST_CHECK_EQUAL( helper.getPointer(pointerName,0) ,(void*)3);
    helper.setPointer(pointerName,(void*)5);
    BOOST_CHECK_EQUAL( helper.getPointer(pointerName,0) ,(void*)5);

    BOOST_CHECK_EQUAL( helper.getDouble(doubleName,0) ,3);
    helper.setDouble(doubleName,5);
    BOOST_CHECK_EQUAL( helper.getDouble(doubleName,0) ,5);

    /**
     * checking reset
     */
    helper.reset(intName);
    BOOST_CHECK_EQUAL( helper.getInt(intName) ,3);
    BOOST_CHECK_EQUAL( helper.dimension(intName) ,2);

    helper.reset(stringName);
    BOOST_CHECK_EQUAL( helper.getString(stringName) ,"three");
    BOOST_CHECK_EQUAL( helper.dimension(stringName) ,2);

    helper.reset(pointerName);
    BOOST_CHECK_EQUAL( helper.getPointer(pointerName) ,(void*)3);
    BOOST_CHECK_EQUAL( helper.dimension(pointerName) ,2);

    helper.reset(doubleName);
    BOOST_CHECK_EQUAL( helper.getDouble(doubleName) ,3);
    BOOST_CHECK_EQUAL( helper.dimension(doubleName) ,2);

    /**
     * checking bad type trying to change an int property with non int accessors
     */
    BOOST_CHECK_THROW( helper.setPointer(intName,NULL), openfx::plugin::exception::bad_handle );
    BOOST_CHECK_THROW( helper.setString(intName,NULL), openfx::plugin::exception::bad_handle );
    BOOST_CHECK_THROW( helper.setDouble(intName,0), openfx::plugin::exception::bad_handle );
    BOOST_CHECK_THROW( helper.getPointer(intName), openfx::plugin::exception::bad_handle );
    BOOST_CHECK_THROW( helper.getString(intName), openfx::plugin::exception::bad_handle );
    BOOST_CHECK_THROW( helper.getDouble(intName), openfx::plugin::exception::bad_handle );

    /**
     * checking bad type on SetN/GetN
     */
    // int
    BOOST_CHECK_THROW( helper.setPointerN(intName,NULL,0), openfx::plugin::exception::bad_handle );
    BOOST_CHECK_THROW( helper.setStringN(intName,NULL,0), openfx::plugin::exception::bad_handle );
    BOOST_CHECK_THROW( helper.setDoubleN(intName,NULL,0), openfx::plugin::exception::bad_handle );
    BOOST_CHECK_THROW( helper.getPointerN(intName,NULL,0), openfx::plugin::exception::bad_handle );
    BOOST_CHECK_THROW( helper.getStringN(intName,NULL,0), openfx::plugin::exception::bad_handle );
    BOOST_CHECK_THROW( helper.getDoubleN(intName,NULL,0), openfx::plugin::exception::bad_handle );
    // FIXME is it necessary to make all the other cross products ? (string, pointer, double)

    /**
     * checking GetN/SetN
     */
    const int arraySize= 2;
    // int
    int intData[arraySize];
    helper.getIntN(intName,intData,arraySize);
    BOOST_CHECK_EQUAL( intData[0] ,3);
    BOOST_CHECK_EQUAL( intData[1] ,4);
    intData[0] = 5;
    helper.setIntN(intName,intData,arraySize);
    BOOST_CHECK_EQUAL( helper.getInt(intName,0), 5);
    // pointer
    void* voidData[arraySize];
    helper.getPointerN(pointerName,voidData,arraySize);
    BOOST_CHECK_EQUAL( voidData[0] ,(void*)3);
    BOOST_CHECK_EQUAL( voidData[1] ,(void*)4);
    voidData[0] = (void*)5;
    helper.setPointerN(pointerName,voidData,arraySize);
    BOOST_CHECK_EQUAL( helper.getPointer(pointerName,0), (void*)5);
    // string
    char* charData[arraySize];
    helper.getStringN(stringName,charData,arraySize);
    BOOST_CHECK_EQUAL( charData[0] ,"three");
    BOOST_CHECK_EQUAL( charData[1] ,"four");
    charData[0] = "five";
    helper.setStringN(stringName,const_cast<const char**>(charData),arraySize);
    BOOST_CHECK_EQUAL( helper.getString(stringName,0), "five");
    // double
    double doubleData[arraySize];
    helper.getDoubleN(doubleName,doubleData,arraySize);
    BOOST_CHECK_EQUAL( doubleData[0] ,3);
    BOOST_CHECK_EQUAL( doubleData[1] ,4);
    doubleData[0] = 5;
    helper.setDoubleN(doubleName,doubleData,arraySize);
    BOOST_CHECK_EQUAL( helper.getDouble(doubleName,0), 5);
}

BOOST_AUTO_TEST_SUITE_END()
