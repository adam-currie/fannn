#include "profile_governor_exec_context.h"
#include <cmath>

using namespace Fannn;
using namespace std;

inline double ProfileGovernorExecContext::readSensorByIdOrAlias(string idOrAlias) const {
    for (const auto & sa : profile->getSensorAliases())
        if (sa.alias == idOrAlias)
            return sensorReader(sa.id);
    return sensorReader(idOrAlias);
}

bool ProfileGovernorExecContext::lookupAndExec(const std::string& idOrAlias, double & out, std::string & errMsg) const {
    Governor const * g = profile->getGovernor(idOrAlias);
    
    if (g) {
        out = g->constExec(*this);
        if (isnan(out))
            errMsg = "governor ' " + idOrAlias + " ' contains errors";
    } else {
        out = readSensorByIdOrAlias(idOrAlias);
        if (isnan(out))
            errMsg = "sensor/governor ' " + idOrAlias + " ' not found";
    }
    
    return !isnan(out);
}

bool ProfileGovernorExecContext::lookupAndExec(const std::string& id, double & out, std::string & errMsg, double arg) const {
    for (const auto & c : profile->getCurves()) {
        if (c.name == id) {
            out = c.getY(arg);//curve failure is programmer error not user error, so don't worry about it
            return true;
        }
    }
    
    errMsg = "curve ' " + id + " ' not found";
    out = numeric_limits<double>::quiet_NaN();
    return false;
}

bool ProfileGovernorExecContext::testLookUpOneArgFunc(const std::string& id, std::string & errMsg) const {
    for (const auto & c : profile->getCurves())
        if (c.name == id)
            return true;
    errMsg = "curve ' " + id + " ' not found";
    return false;
}
