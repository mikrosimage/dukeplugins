/*
 * IBoostPlugin.h
 *
 *  Created on: 30 sept. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef IBOOSTPLUGIN_H_
#define IBOOSTPLUGIN_H_

#include "IPlugin.h"
#include "OstreamHelper.h"

#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include <string>


/**
 * If it's not an issue for you to use boost in your plugin project
 * you can use this implementation of plugin allowing you to easily dispatch
 * actions to functions thanks to boost bind.
 *
 * here is an example of what you can put in your plugin constructor
 *
 *     registerAction(kOfxActionLoad, boost::bind(&MyPlugin::loadPlugin, this, _1, _2, _3));
 *     registerAction(kOfxActionUnload, boost::bind(&MyPlugin::unloadPlugin, this));
 *
 * The first line will bind the kOfxActionLoad action to the following function
 * MyPlugin::loadPlugin(const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs)
 *
 * The second line will bind the kOfxActionUnload action to the following function
 * MyPlugin::unloadPlugin()
 */
class IBoostPlugin : public IPlugin {
protected:
    typedef boost::function<OfxStatus(const void*, OfxPropertySetHandle, OfxPropertySetHandle)> ActionFunction;
private:
    typedef boost::unordered_map<std::string, ActionFunction> ActionMap;
    ActionMap m_ActionMap;

public:
    virtual OfxStatus pluginMain(const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs) {
    	const ActionMap::const_iterator functionHandler = m_ActionMap.find(action);
        if (functionHandler == m_ActionMap.end())
            return unhandledAction(action, handle, inArgs, outArgs);
        return functionHandler->second(handle, inArgs, outArgs);
    }

    virtual OfxStatus unhandledAction(const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs) {
        std::cerr << *getOfxPlugin() << " - Unhandled action : " << action << std::endl;
        return kOfxStatReplyDefault;
    }

    /**
     * registers an action to a function. Use Boost.bind to do so.
     */
    void registerAction(const char* action, ActionFunction function) {
        m_ActionMap[action] = function;
    }

    /**
     * if your plugin dynamically registers action
     * you can use this function to unregister actions on the fly.
     */
    void unregisterAction(const char* action) {
        m_ActionMap.erase(action);
    }
};

#endif /* IBOOSTPLUGIN_H_ */
