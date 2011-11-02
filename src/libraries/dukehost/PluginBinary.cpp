/*
 * PluginBinary.cpp
 *
 *  Created on: 7 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include "PluginBinary.h"
#include "openfx/platform.h"

#include "HostUtils.h"

#include <sstream>
#include <iostream>
#include <boost/filesystem.hpp>

using namespace std;

namespace openfx {
namespace host {

namespace _private {

#if defined ( WINDOWS )
#define _WIN32_WINNT 0x0502
#include <Windows.h>
#include <sys/types.h>
#include <sys/stat.h>

inline void* loadLibrary(const char* filename) {
    // Deactivate message box while trying to load an invalid plugin
    // http://msdn.microsoft.com/en-us/library/ms680621(VS.85).aspx
    SetErrorMode(SEM_FAILCRITICALERRORS);
    // adding the library folder as a new folder to look for dll
    // in case our plugin needs other dlls
    const boost::filesystem::path dllPath(filename);
    SetDllDirectory(dllPath.parent_path().string().c_str());
    void* module = LoadLibrary(filename);
    SetDllDirectory(NULL);
    return module;
}

inline void freeHandle(void* handle) {
    FreeLibrary((HMODULE) handle);
}

inline void* findSymbol(void* handle, const char* name) {
    return (void*) GetProcAddress((HMODULE) handle, name);
}

string buildErrorMsg(string& filename) {
    LPVOID lpMsgBuf = NULL;
    DWORD err = GetLastError();

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
            (LPTSTR) &lpMsgBuf, 0, NULL);

    ostringstream msg;
    msg << "couldn't open library " << filename << " reason :" << endl << (char*) lpMsgBuf;
    if (lpMsgBuf != NULL)
    LocalFree(lpMsgBuf);

    return msg.str();
}

#else
#include <dlfcn.h>

inline void* loadLibrary(const char* filename) {
    return dlopen(filename, RTLD_LAZY | RTLD_LOCAL);
}

inline void freeHandle(void* handle) {
    dlclose(handle);
}

inline void* findSymbol(void* handle, const char* name) {
    return dlsym(handle, name);
}

string buildErrorMsg(string& filename) {
    ostringstream msg;

    msg << "couldn't open library " << filename << " reason :" << endl << dlerror();
    return msg.str();
}

#endif

} // namespace _private

PluginBinary::PluginBinary(const char* filename) :
    _dlHandle(NULL), _binaryPath(filename) {
    ::boost::filesystem::path path(_binaryPath);

    if (!boost::filesystem::is_regular_file(path))
        throw runtime_error(string("Unable to find plug-in file ") + _binaryPath);
}

PluginBinary::~PluginBinary() {
    if (_dlHandle)
        unload();
}

void PluginBinary::load(OfxHost &host, const acceptPlug acceptPlugFunction) {
    if (_dlHandle)
        throw runtime_error("load function was already call on this binary");

    _dlHandle = _private::loadLibrary(_binaryPath.c_str());

    if (!_dlHandle)
        throw runtime_error(_private::buildErrorMsg(_binaryPath));

    int (* getCount)(void) = ( int ( * )( void ) )findSymbol( "OfxGetNumberOfPlugins" );
    OfxPlugin* (*getPlugin)(int) = ( OfxPlugin * ( * )( int ) )findSymbol( "OfxGetPlugin" );
    if (getCount == NULL || getPlugin == NULL)
        throw runtime_error("One of the bootstrapping functions (OfxGetNumberOfPlugins or OfxGetPlugin) is missing");
    const int pluginCount = (*getCount)();
    if (pluginCount == 0)
        throw runtime_error("Invalid binary : contains no plug-in");
    for (int i = 0; i < pluginCount; ++i) {
        OfxPlugin* pPlugin = (*getPlugin)(i);

        if (pPlugin == NULL)
            throw runtime_error("GetPlugin() returned NULL");
        if (pPlugin->setHost == NULL)
            throw runtime_error("setHost function is NULL");
        if (pPlugin->mainEntry == NULL)
            throw runtime_error("mainEntry function is NULL");

        if ((acceptPlugFunction != NULL) && !acceptPlugFunction(pPlugin)) {
            cerr << "Plugin " << pPlugin->pluginIdentifier << " was rejected." << endl;
            continue;
        }
        pPlugin->setHost(&host);
        perform(pPlugin, kOfxActionLoad, NULL, NULL, NULL);

        _plugins.push_back(pPlugin);
    }
}

void PluginBinary::unload() {
    if (!_dlHandle)
        throw runtime_error("unload function was already call on this binary");

    vector<OfxPlugin*>::iterator itr;
    for (itr = _plugins.begin(); itr != _plugins.end(); ++itr)
        (*itr)->mainEntry(kOfxActionUnload, NULL, NULL, NULL);

    _private::freeHandle(_dlHandle);
    _dlHandle = 0;
}

void* PluginBinary::findSymbol(const char* name) {
    if (_dlHandle) {
        return _private::findSymbol(_dlHandle, name);
    } else {
        ostringstream msg;
        msg << "Symbol '" << name << "' not found in plug-in file: '" << _binaryPath << "'";
        throw logic_error(msg.str());
    }
}

} // namespace host
} // namespace openfx
