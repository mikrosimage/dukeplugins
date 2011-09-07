/*
 * IPlugin.h
 *
 *  Created on: 8 avr. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef IPLUGIN_H_
#define IPLUGIN_H_

#include <openfx/ofxCorePlugin.h>
#include <cassert>

/**
 * To implement a plugin simply inherit from this very simple class.
 * You have to implement the pluginMain function and react
 * accordingly to the API you are implementing
 *
 * Please consider using the IBoostPlugin implementation,
 * it will make your life much easier as a plugin developer.
 */
class IPlugin {
private:
    IPlugin(const IPlugin&);
    // Let PluginBootstrap access OfxPlugin end OfxHost members
    friend OfxPlugin* OfxGetPlugin(int);
    template<int U>
    friend struct OfxPluginInstance;
    friend struct TEST_ACCESSOR;
    OfxPlugin* m_pPlugin;
    OfxHost* m_pHost;

public:
    IPlugin() :
        m_pPlugin(NULL), m_pHost(NULL) {
    }

    virtual ~IPlugin() {
    }

    OfxPlugin const * const getOfxPlugin() const {
        // if your code breaks here please remember you can use access
        // this function *only* between kOfxActionLoad and kOfxActionUnload action calls
        assert(m_pPlugin);
        return m_pPlugin;
    }
    OfxHost const * const getOfxHost() const {
        // if your code breaks here please remember you can use access
        // this function *only* between kOfxActionLoad and kOfxActionUnload action calls
        assert(m_pHost);
        return m_pHost;
    }

protected:
    virtual OfxStatus pluginMain(const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs) = 0;
};

#endif /* IPLUGIN_H_ */
