/*
 * Plugin.cpp
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include "Plugin.h"

static bool isEqual( const char* expected, const char* actual )
{
	const char* pExpected = expected;
	const char* pActual   = actual;

	while( true )
	{
		if( *pExpected == '\0' && *pActual == '\0' )
			return true;
		if( *pExpected != *pActual )
			return false;
		++pExpected;
		++pActual;
	}
}

Plugin::Plugin() {}

Plugin::~Plugin() {}

OfxStatus Plugin::pluginMain( const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs )
{
	// called as the very first action before any other
	if( isEqual( kOfxActionLoad, action ) )
		return onLoad();
	else if( isEqual( kOfxActionUnload, action ) )
		return onUnload();
	// all other actions return the default value
	return kOfxStatReplyDefault;
}

// Called at load
OfxStatus Plugin::onLoad( void )
{
	// fetch the host suites out of the global host pointer
	if( !getOfxHost() )
		return kOfxStatErrMissingHostFeature;
	return kOfxStatOK;
}

// Called at unload
OfxStatus Plugin::onUnload( void )
{
	return kOfxStatOK;
}

