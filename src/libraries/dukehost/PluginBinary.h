/*
 * PluginBinary.h
 *
 *  Created on: 7 avr. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef PLUGINBINARY_H_
#define PLUGINBINARY_H_

#include "openfx/ofxCorePlugin.h"
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

typedef bool (*acceptPlug)(const OfxPlugin* plug);

namespace openfx {
namespace host {

/**
 * PluginBinary represents a loaded shared library.
 * This shared library can itself contain one or more
 * OpenFX plugin.
 */
class PluginBinary : public boost::noncopyable {
public:
    typedef std::vector<OfxPlugin*> PluginVector;
private:
    void* _dlHandle;
    std::string _binaryPath;
    PluginVector _plugins;
    void* findSymbol(const char* name);
public:
    PluginBinary(const char* filename);
    ~PluginBinary();

    void load(OfxHost &host, const acceptPlug acceptPlugFunction = NULL);
    void unload();

    bool isLoaded() {
        return _dlHandle != NULL;
    }
    const PluginVector& getPlugins() const {
        return _plugins;
    }
};

} // namespace host
} // namespace openfx

#endif /* PLUGINBINARY_H_ */
