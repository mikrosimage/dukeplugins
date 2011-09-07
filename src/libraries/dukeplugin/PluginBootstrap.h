/*
 * Bootstrap plugin file defines the plugin instances.
 * Please be sure to include this file only *once*.
 *
 * You must define a global int constant in order to declare the number of plugins
 * this compilation unit will hold
 * ie : const int PLUGIN_COUNT = 2;
 *
 * see /test/sample project for a sample plugin and how to use this code
 *
 * Author: Guillaume Chatelet
 */

#ifndef PLUGINBOOTSTRAP_H_
#define PLUGINBOOTSTRAP_H_

#include "IPlugin.h"
#include <string>
#include <cassert>
#include <iostream>
#include <memory>

// PLUGIN_COUNT global variable needs to exists before including this file.
std::auto_ptr<IPlugin> g_pPlugins[PLUGIN_COUNT];

/**
 * Template and template specialization to allow compile time checking.
 */
template <bool> void CompileTimeAssert();
template<> void CompileTimeAssert<true>() {};

/**
 * Creates a slotted instance of OfxPlugin with templated static functions
 * binded to the OfxPlugin function pointers. Ensures that the IPlugin instance is
 * set in the 'g_pPlugins' array and that it m_pPlugin member is initialized.
 * This object also holds the pluginApi and pluginIdentifier strings.
 */
template<int SLOT>
struct OfxPluginInstance : public OfxPlugin
{
	private:
		const std::string pluginApiHolder;
		const std::string pluginIdentifierHolder;
		OfxPluginInstance( const OfxPluginInstance& );

		static void _setHost( OfxHost* pHost )
		{
			getPlugin().m_pHost = pHost;
		}

		static OfxStatus _mainEntry( const char* action, const void* handle,
		                             OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs )
		{
			try
			{
				return getPlugin().pluginMain( action, handle, inArgs, outArgs );
			}
			catch( std::bad_alloc& ex )
			{
				displayException( "Exception occurred in mainEntry", ex );
				return kOfxStatErrMemory;
			}
			catch( std::exception& ex )
			{
				displayException( "Exception occurred in mainEntry", ex );
				return kOfxStatFailed;
			}
			catch( ... )
			{
				std::cerr << "Unknown exception occurred in mainEntry" << std::endl << std::endl;
				dumpPlugin();
				return kOfxStatErrFatal;
			}
		}

		static void displayException( const char* msg, std::exception& ex )
		{
			std::cerr << msg << std::endl << ex.what() << std::endl;
			std::cerr << std::endl;
			dumpPlugin();
		}

		static void dumpPlugin()
		{
			const OfxPlugin& plugin( *( getPlugin().m_pPlugin ) );

			std::cerr << "Plugin    : " << plugin.pluginIdentifier << " ( version " << plugin.pluginVersionMajor << "." << plugin.pluginVersionMinor << ")" << std::endl;
			std::cerr << "API type  : " << plugin.pluginApi << " ( version " << plugin.apiVersion << ")" << std::endl;
		}

		static inline IPlugin& getPlugin()
		{
			return *g_pPlugins[SLOT];
		}

	public: OfxPluginInstance( const char* api, int apiVersion, const char* idendifier,
		                       int major, int minor, IPlugin* pPluginInstance )
			: pluginApiHolder( api ),
			pluginIdentifierHolder( idendifier )
		{
			// If you have an error here, then you are trying to instantiate
			// a OfxPluginInstance with a SLOT number greater than PLUGIN_COUNT
			// ie :
			// const int PLUGIN_COUNT = 1;
			// OfxPluginInstance<3> plugin(...);
			//
			CompileTimeAssert< (SLOT < PLUGIN_COUNT) >();

			OfxPlugin::apiVersion         = apiVersion;
			OfxPlugin::pluginVersionMajor = major;
			OfxPlugin::pluginVersionMinor = minor;
			OfxPlugin::pluginApi          = pluginApiHolder.c_str();
			OfxPlugin::pluginIdentifier   = pluginIdentifierHolder.c_str();
			OfxPlugin::setHost            = &::OfxPluginInstance<SLOT>::_setHost;
			OfxPlugin::mainEntry          = &::OfxPluginInstance<SLOT>::_mainEntry;
			g_pPlugins[SLOT].reset(pPluginInstance);
			getPlugin().m_pPlugin         = this ;
		}

};

#ifdef __cplusplus
extern "C" {
#endif

OfxExport OfxPlugin* OfxGetPlugin( int nth )
{
	// If assertion fails, some plug-ins are not initialized
	// Are you sure you have 'PLUGIN_COUNT' lines of plug-in definition ?
	assert( g_pPlugins[nth].get() != NULL );
	return g_pPlugins[nth]->m_pPlugin;
}

OfxExport int OfxGetNumberOfPlugins( void )
{
	return PLUGIN_COUNT;
}

#ifdef __cplusplus
}
#endif

#endif /* PLUGINBOOTSTRAP_H_ */
