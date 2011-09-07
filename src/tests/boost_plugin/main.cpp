/*
 * main.cpp
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include <dukeplugin/IBoostPlugin.h>

#include <boost/bind.hpp>

#include <boost/test/minimal.hpp>

/**
 * This test demonstrate how to use the Boost version of the plugin
 * Making easy to dispatch calls to functions rather than testing for each and every actions.
 */
struct BoostPlugin : public IBoostPlugin {
    bool loadActionCalled;
    bool unloadActionCalled;

    BoostPlugin() :
        loadActionCalled(false), unloadActionCalled(false) {
        registerAction(kOfxActionLoad, boost::bind(&BoostPlugin::loadPlugin, this, _1, _2, _3));
        registerAction(kOfxActionUnload, boost::bind(&BoostPlugin::unloadPlugin, this));
    }

    virtual ~BoostPlugin() {
    }

    OfxStatus loadPlugin(const void* handle, OfxPropertySetHandle in, OfxPropertySetHandle out) {
        BOOST_CHECK( handle == (void*)1 );
        BOOST_CHECK( in == (OfxPropertySetHandle)2 );
        BOOST_CHECK( out == (OfxPropertySetHandle)3 );
        loadActionCalled = true;
        return kOfxStatOK;
    }

    OfxStatus unloadPlugin() {
        unloadActionCalled = true;
        return kOfxStatOK;
    }
};


int test_main(int argc, char **argv) {
    BoostPlugin plugin;
    BOOST_CHECK( !plugin.loadActionCalled );
    BOOST_CHECK( !plugin.unloadActionCalled );
    plugin.pluginMain(kOfxActionLoad, (void*)1, (OfxPropertySetHandle)2, (OfxPropertySetHandle)3);
    BOOST_CHECK( plugin.loadActionCalled );
    BOOST_CHECK( !plugin.unloadActionCalled );
    plugin.pluginMain(kOfxActionUnload, NULL, NULL, NULL);
    BOOST_CHECK( plugin.loadActionCalled );
    BOOST_CHECK( plugin.unloadActionCalled );
    return 0;
}
