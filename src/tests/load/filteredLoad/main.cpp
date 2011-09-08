/*
 * main.cpp
 *
 *  Created on: 28 sep. 2010
 *      Author: Nicolas Rondaud
 */

#include <dukehost/PluginManager.h>

bool acceptAllNoRecursive(const char* filename, const bool isDirectory){
	return !isDirectory;
}
bool acceptPlugOverVersion10(const OfxPlugin * plug){
	if(plug->apiVersion <= 10) return false;
	return true;
}
bool acceptPlugOverVersion20(const OfxPlugin * plug){
	if(plug->apiVersion <= 20) return false;
	return true;
}

#include <common.h>
#include <boost/test/minimal.hpp>

using namespace std;
using namespace openfx::host;

int test_main(int argc, char **argv) {
	struct OfxPropertySetStruct structure;
	OfxHost host;
	host.host = &structure;
	{
		PluginManager manager(host, argv[1], &acceptAllNoRecursive, &acceptPlugOverVersion10);
		BOOST_CHECK( manager.getNumberOfBinaries() == 1 );
		const PluginBinary &binary = manager.getBinary(0);
		BOOST_CHECK( binary.getPlugins().size() == 1 );
        BOOST_CHECK( manager.getPlugins().size() == 1 );
        BOOST_CHECK( manager.getPlugins()[0] == binary.getPlugins()[0] );
	}
	{
		PluginManager manager(host, argv[1], &acceptAllNoRecursive, &acceptPlugOverVersion20);
		BOOST_CHECK( manager.getNumberOfBinaries() == 1 );
		const PluginBinary &binary = manager.getBinary(0);
		BOOST_CHECK( binary.getPlugins().size() == 0 );
        BOOST_CHECK( manager.getPlugins().size() == 0 );
	}
	return 0;
}
