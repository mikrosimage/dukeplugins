/*
 * PluginManager.cpp
 *
 *  Created on: 7 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include "PluginManager.h"
#include <boost/filesystem.hpp>
#include <algorithm>
#include <memory>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace boost::filesystem;
using namespace std;

namespace openfx {
namespace host {

PluginManager::PluginManager(OfxHost &host, const char* folder, const acceptFile acceptFileFunction, const acceptPlug acceptPlugFunction) :
    m_Host(host) {
    loadPlugins(folder, acceptFileFunction, acceptPlugFunction);
}

void PluginManager::loadPlugins(const string &folder, const acceptFile acceptFileFunction, const acceptPlug acceptPlugFunction) {
    try {
        path directory(folder);
        if (directory.has_root_path() == false)
            directory = current_path() / directory;
        const directory_iterator end_itr; // default construction yields past-the-end
        for (directory_iterator itr(directory); itr != end_itr; ++itr) {
            const path& fullpath(itr->path());
            const string path = fullpath.string();
            const bool isDirectory = is_directory(fullpath);
            if (!acceptFileFunction(path.c_str(), isDirectory))
                continue;
            if (isDirectory)
                loadPlugins(path, acceptFileFunction, acceptPlugFunction);
            else
                loadPlugin(path.c_str(), acceptPlugFunction);
        }
    } catch (exception& e) {
        cerr << e.what() << endl;
        throw;
    }
}

PluginManager::~PluginManager() {
}

void PluginManager::loadPlugin(const char* filename, const acceptPlug acceptPlugFunction) {
    try {
        auto_ptr<PluginBinary> pPlugin(new PluginBinary(filename));
        pPlugin->load(m_Host, acceptPlugFunction);
        m_vBinaries.push_back(pPlugin.release());
    } catch (exception& exception) {
        cerr << "exception occurred while reading plug-in '" << filename << "' reason :" << endl << exception.what() << endl;
    }
}

size_t PluginManager::getNumberOfBinaries() const {
    return m_vBinaries.size();
}

const PluginBinary& PluginManager::getBinary(size_t nth) const {
    return m_vBinaries[nth];
}

PluginManager::PluginVector PluginManager::getPlugins() const {
    PluginVector plugins;
    for (BinaryVector::const_iterator pBinaryItr = m_vBinaries.begin(); pBinaryItr != m_vBinaries.end(); ++pBinaryItr)
        plugins.insert(plugins.end(), pBinaryItr->getPlugins().begin(), pBinaryItr->getPlugins().end());
    return plugins;
}

} // namespace host
} // namespace openfx
