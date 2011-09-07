/*
 * SuiteHelper.h
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef SUITEHELPER_H_
#define SUITEHELPER_H_

#include <boost/unordered_map.hpp>

namespace openfx {
namespace host {

typedef std::pair<std::string, int> SuiteDescription;
typedef boost::unordered_map<SuiteDescription, void*> RegistereredSuites;

} // namespace host
} // namespace openfx

#endif /* SUITEHELPER_H_ */
