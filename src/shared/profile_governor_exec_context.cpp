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

double ProfileGovernorExecContext::lookupAndExec(const std::string& idOrAlias, std::string & errMsg) const {
    double value;    
    Governor const * g = profile->getGovernor(idOrAlias);
    
    if (g) {
        value = g->constExec(*this);
        if (isnan(value))
            errMsg = "governor ' " + idOrAlias + " ' contains errors";
    } else {
        value = readSensorByIdOrAlias(idOrAlias);
        if (isnan(value))
            errMsg = "sensor/governor ' " + idOrAlias + " ' not found";
    }
    
    return value;
}

double ProfileGovernorExecContext::lookupAndExec(const std::string& id, std::string & errMsg, double arg) const {
    for (const auto & c : profile->getCurves())
        if (c.name == id) 
            return c.getY(arg);
    
    errMsg = "curve ' " + errMsg + " ' not found";
    return numeric_limits<double>::quiet_NaN();
}
