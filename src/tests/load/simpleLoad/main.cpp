/*
 * main.cpp
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include <dukehost/PluginManager.h>

bool acceptAllNoRecursive(const char* filename, const bool isDirectory){
	return !isDirectory;
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
		PluginManager manager(host, argv[1], &acceptAllNoRecursive);
		BOOST_CHECK( manager.getNumberOfBinaries() == 1 );
		const PluginBinary &binary = manager.getBinary(0);
		BOOST_CHECK( binary.getPlugins().size() == 1 );
		BOOST_CHECK( manager.getPlugins().size() == 1 );
		BOOST_CHECK( manager.getPlugins()[0] == binary.getPlugins()[0] );
		const OfxPlugin& plugin = *(binary.getPlugins()[0]);
		BOOST_CHECK( plugin.apiVersion == apiVersion );
		BOOST_CHECK( plugin.pluginApi == api );
		BOOST_CHECK( plugin.pluginIdentifier == idendifier );
		BOOST_CHECK( plugin.pluginVersionMajor == major );
		BOOST_CHECK( plugin.pluginVersionMinor == minor );
		BOOST_CHECK( structure.vVector.size() == 1 );
		BOOST_CHECK( structure.vVector[0] == kOfxActionLoad );
	}
	BOOST_CHECK( structure.vVector.size() == 2 );
	BOOST_CHECK( structure.vVector[0] == kOfxActionLoad );
	BOOST_CHECK( structure.vVector[1] == kOfxActionUnload );
	BOOST_CHECK( structure.bDestructorCalled );
	return 0;
}
