/*
 * Plugin.h
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <dukeplugin/IPlugin.h>

class Plugin : public IPlugin
{
public: Plugin();
	virtual ~Plugin();

protected:
	virtual OfxStatus pluginMain( const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs );
	virtual OfxStatus onLoad( void );
	virtual OfxStatus onUnload( void );
};

#endif /* PLUGIN_H_ */
