/*
 * PluginManager.h
 *
 *  Created on: 7 avr. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include "PluginBinary.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>

typedef bool (*acceptFile)(const char* filename, const bool isDirectory);

namespace openfx {
namespace host {

/**
 * PluginManager allow you to browse folder for plugins filtering
 * the loaded one according to the filename and the API they implements
 */
class PluginManager : public boost::noncopyable {
private:
    OfxHost &m_Host;
    typedef boost::ptr_vector<PluginBinary> BinaryVector;
    BinaryVector m_vBinaries;

    void loadPlugins(const std::string &folder, const acceptFile acceptFileFunction, const acceptPlug acceptPlugFunction = NULL);
    void loadPlugin(const char* filename, const acceptPlug acceptPlugFunction = NULL);

public:
    typedef std::vector<OfxPlugin const *> PluginVector;
    PluginManager(OfxHost &host, const char* folder, const acceptFile acceptFileFunction, const acceptPlug acceptPlugFunction = NULL);
    ~PluginManager();

    size_t getNumberOfBinaries() const;

    const PluginBinary& getBinary(size_t nth) const;

    // construct on the fly a list of all currently available plugins
    PluginVector getPlugins() const;
};

} // namespace host
} // namespace openfx

#endif /* PLUGINMANAGER_H_ */
