/*
 * PluginManager.cpp
 *
 *  Created on: 7 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include "PluginManager.h"
#include <boost/filesystem.hpp>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace boost::filesystem;

namespace openfx {
namespace host {

PluginManager::PluginManager(OfxHost &host, const char* folder, const acceptFile acceptFileFunction, const acceptPlug acceptPlugFunction) :
    m_Host(host) {
    loadPlugins(folder, acceptFileFunction, acceptPlugFunction);
}

void PluginManager::loadPlugins(const std::string folder, const acceptFile acceptFileFunction, const acceptPlug acceptPlugFunction) {
    try {
        path directory(folder);
        if (directory.has_root_path() == false)
            directory = current_path() / directory;
        const directory_iterator end_itr; // default construction yields past-the-end
        for (directory_iterator itr(directory); itr != end_itr; ++itr) {
            const path& file(itr->path());
            bool isDirectory = is_directory(file);
            const char* filename = file.string().c_str();
            if (!acceptFileFunction(filename, isDirectory))
                continue;
            if (isDirectory)
                loadPlugins(file.string(), acceptFileFunction, acceptPlugFunction);
            else
                loadPlugin(filename, acceptPlugFunction);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

PluginManager::~PluginManager() {
}

void PluginManager::loadPlugin(const char* filename, const acceptPlug acceptPlugFunction) {
    try {
        boost::shared_ptr<PluginBinary> pBinary(new PluginBinary(filename));
        pBinary->load(m_Host, acceptPlugFunction);
        m_vBinaries.push_back(pBinary);
    } catch (std::exception& exception) {
        std::cerr << "exception occurred while reading plug-in '" << filename << "' reason :" << std::endl << exception.what() << std::endl;
    }
}

size_t PluginManager::getNumberOfBinaries() const {
    return m_vBinaries.size();
}

PluginBinary& PluginManager::getBinary(size_t nth) const {
    return *(m_vBinaries[nth]);
}

PluginManager::PluginVector PluginManager::getPlugins() const {
    PluginVector plugins;
    typedef PluginBinary::PluginVector PluginVector;
    for (BinaryVector::const_iterator pBinaryItr = m_vBinaries.begin(); pBinaryItr != m_vBinaries.end(); ++pBinaryItr) {
        const PluginVector &binaryPlugins = pBinaryItr->get()->getPlugins();
        PluginVector::const_iterator itr;
        for (itr = binaryPlugins.begin(); itr != binaryPlugins.end(); ++itr)
            plugins.push_back(*itr);
    }
    return plugins;
}

} // namespace host
} // namespace openfx
