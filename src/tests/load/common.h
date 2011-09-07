/*
 * constants.h
 *
 *  Created on: 12 avr. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>
#include <vector>

const std::string api = "api";
const int apiVersion = 11;
const std::string idendifier = "identifer";
const unsigned int major = 22;
const unsigned int minor = 33;

struct OfxPropertySetStruct {
    std::vector<std::string> vVector;
    bool bDestructorCalled;
    OfxPropertySetStruct() :
        bDestructorCalled(false) {
    }
};

#endif /* CONSTANTS_H_ */
