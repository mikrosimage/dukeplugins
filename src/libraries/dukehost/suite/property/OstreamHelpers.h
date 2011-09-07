/*
 * OstreamHelpers.h
 *
 *  Created on: 30 sept. 2010
 *      Author: Guillaume Chatelet
 *
 * A set of convenient methods to display Property and PropertySet
 */

#ifndef OSTREAMHELPERS_H_
#define OSTREAMHELPERS_H_

#include "PropertySet.h"

#include <ostream>

std::ostream& operator <<(std::ostream &os, const ::openfx::host::Property &prop);
std::ostream& operator <<(std::ostream &os, const ::openfx::host::PropertySet &propSet);

#endif /* OSTREAMHELPERS_H_ */
