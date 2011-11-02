/*
 * SuiteFetcher.h
 *
 *  Created on: 29 sept. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef SUITEFETCHER_H_
#define SUITEFETCHER_H_

#include <openfx/ofxCorePlugin.h>
#include <dukeplugin/IPlugin.h>
#include <sstream>
#include <stdexcept>
#include <cassert>

namespace openfx {
namespace plugin {

/**
 * Inherit from this class to implement your own OpenFX Suite.
 * Check the property Suite implementation to get an example
 */
template<typename SUITE>
class SuiteFetcher {
    const std::string m_pSuiteName;
    const int m_pSuiteVersion;
    // the suite pointer is an accelerator and do not contribute
    // to the state of this object so getSuite() can remain const
    mutable const SUITE * m_pSuite;
    const IPlugin &m_Plugin;
public:
    SuiteFetcher(const IPlugin &plugin, const char* SUITE_NAME, int SUITE_VERSION) :
        m_pSuiteName(SUITE_NAME), m_pSuiteVersion(SUITE_VERSION), m_pSuite(NULL), m_Plugin(plugin) {
    }

    virtual ~SuiteFetcher() {
    }

    /**
     * returns whether this Suite is available in the host
     */
    bool isAvailable() const throw () {
        try {
            getSuite();
            return true;
        } catch (...) {
            return false;
        }
    }

    /**
     * returns the OfxHost structure registered within the plugin
     * It might be needed by the implementations of this class
     * throws std::runtime_error if the host is not yet set in the plugin
     */
    OfxHost const &getHost() const {
        OfxHost const * const pHost = m_Plugin.getOfxHost();
        if (pHost == NULL)
            throw std::runtime_error(
                                     "openfx::plugin::SuiteFetcher : plugin host is NULL, remember you can use a Suite *only* between kOfxActionLoad and kOfxActionUnload action calls");
        return *pHost;
    }

    /**
     * returns the Suite
     * throws std::runtime_error if the Suite is not available
     */
    const SUITE &getSuite() const {
        if (m_pSuite == NULL) {
            // Suite is not fetched yet
            OfxHost const &host = getHost();
            m_pSuite = reinterpret_cast<const SUITE *> ((*(host.fetchSuite))(host.host, m_pSuiteName.c_str(), m_pSuiteVersion));
            if (m_pSuite == NULL) {
                std::ostringstream msg;
                msg << "The suite " << m_pSuiteName;
                msg << " version " << m_pSuiteVersion;
                msg << " is not available";
                throw std::runtime_error(msg.str());
            }
        }
        return *m_pSuite;
    }
};

} // namespace plugin
} // namespace openfx

#endif /* SUITEFETCHER_H_ */
