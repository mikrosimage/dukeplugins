/*
 * GoodEmptyPlugin.cpp
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include <dukeplugin/IPlugin.h>

#include "common.h"

#include <stdexcept>

class GoodEmptyPlugin : public IPlugin {
    bool *m_pDestructorCalled;
public:
    GoodEmptyPlugin() :
        m_pDestructorCalled(NULL) {
    }
    virtual ~GoodEmptyPlugin() {
        // if a communication occured between host and plugin
        // we have a valid pointer
        // and we can report the destructor has been called
        if (m_pDestructorCalled)
            *m_pDestructorCalled = true;
    }

    virtual OfxStatus pluginMain(const char *action, const void *handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs) {
        if (getOfxHost() == NULL)
            throw std::runtime_error("host structure is NULL");
        if (getOfxHost()->host == NULL)
            throw std::runtime_error("OfxPropertySetHandle host structure is NULL");
        getOfxHost()->host->vVector.push_back(action);
        m_pDestructorCalled = &(getOfxHost()->host->bDestructorCalled);
        return kOfxStatOK;
    }
};

// plugin declaration

const int PLUGIN_COUNT = 1;

#include <openfx/support/plugin/PluginBootstrap.h>

OfxPluginInstance<0> plugin(api.c_str(), apiVersion, idendifier.c_str(), major, minor, new GoodEmptyPlugin());
