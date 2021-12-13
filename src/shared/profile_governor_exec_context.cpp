#include "profile_governor_exec_context.h"
#include <cmath>

using namespace Fannn;
using namespace std;

inline double ProfileGovernorExecContext::readSensorByIdOrAlias(string idOrAlias) const {
    for (const auto & sa : profile->getSensorAliases())
        if (sa.alias == idOrAlias)
            return sensorReader.read(sa.id);
    return sensorReader.read(idOrAlias);
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
            bool testRun = isnan(arg);
            out = testRun ?
                arg :
                c.getY(arg);
            return true;
        }
    }
    
    errMsg = "curve ' " + id + " ' not found";
    out = numeric_limits<double>::quiet_NaN();
    return false;
}
