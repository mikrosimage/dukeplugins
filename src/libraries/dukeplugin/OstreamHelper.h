/*
 * OstreamHelper.h
 *
 *  Created on: 1 oct. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef OSTREAMHELPER_H_
#define OSTREAMHELPER_H_

#include <openfx/ofxCorePlugin.h>
#include <iostream>

inline std::ostream& operator <<(std::ostream &os, const OfxPlugin &plugin) {
    os << plugin.pluginIdentifier << ':' << plugin.pluginVersionMajor << '.' << plugin.pluginVersionMinor;
    os << " [" << plugin.pluginApi << ':' << plugin.apiVersion << "]";
    return os;
}

#endif /* OSTREAMHELPER_H_ */
