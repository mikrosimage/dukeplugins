/*
 * This file is an example on how to bootstrap plugins
 */

const int PLUGIN_COUNT = 2;

// You should include PluginBootstrap.h only once as it defines global variables and functions
#include <dukeplugin/PluginBootstrap.h>

#include "Plugin.h"
OfxPluginInstance<0> plug1( "ImageProcessor", 0, "fr.mikrosimage.tata", 0, 1, new Plugin() );
OfxPluginInstance<1> plug2( "ImageProcessor", 0, "fr.mikrosimage.toto", 0, 1, new Plugin() );
