/*
 * OfxPluginExceptions.h
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef OFXPLUGINEXCEPTIONS_H_
#define OFXPLUGINEXCEPTIONS_H_

#include <string>
#include <new>
#include <stdexcept>

namespace openfx {
namespace plugin {
namespace exception {

/** kOfxStatErrUnknown  */
class unknown_object : public std::invalid_argument {
public:
    explicit unknown_object(const std::string& __arg) :
        std::invalid_argument(std::string("kOfxStatErrUnknown : ") + __arg) {
    }
};

/** kOfxStatErrBadHandle  */
class bad_handle : public std::invalid_argument {
public:
    explicit bad_handle(const std::string& __arg) :
        std::invalid_argument(std::string("kOfxStatErrBadHandle : ") + __arg) {
    }
};

/** kOfxStatErrBadIndex  */
class bad_index : public std::out_of_range {
public:
    explicit bad_index(const std::string& __arg) :
        std::out_of_range(std::string("kOfxStatErrBadIndex : ") + __arg) {
    }
};

/** kOfxStatErrMemory  */
class memory : public std::bad_alloc {
public:
    explicit memory() :
        std::bad_alloc() {
    }
};

} // namespace exception
} // namespace plugin
} // namespace openfx


#endif /* OFXPLUGINEXCEPTIONS_H_ */
