#include "profile.h"

using namespace Fannn;
using namespace std;

string Profile::getAliasForSensor(std::string sensorId) const {
    for (auto const & a : sensorAliases) {
        if (a.id == sensorId)
            return a.alias;
    }
    return {};
}

string Profile::getGovernorForController(std::string controllerId) const {
    for (auto const & c : controllers) {
        if (c.id == controllerId)
            return c.governorName;
    }
    return {};
}

string Profile::removeAliasForSensor(std::string sensorId) {
    for (auto iter=sensorAliases.begin(); iter!=sensorAliases.end(); iter++) {
        if (iter->id == sensorId) {
            sensorAliases.erase(iter);
            return iter->alias;
        }
    }
    return {};
}

bool Profile::hasIssues() const {
    //check governors
    for (const Fannn::Governor & g : governors)
        if (!g.getErrors().empty())
            return true;

    //check controllers
    for (const Controller & c : controllers) {
        bool found = false;
        for (const Governor & g : governors) {
            if (g.name == c.governorName) {
                found = true;
                break;
            }
        }
        if (!found)
            return true;
    }

    return false;
}

bool Profile::setSensorAlias(std::string sensorId, std::string alias, bool& govCollision, bool& aliasCollision) {
    govCollision = aliasCollision = false;

    if (alias == "")//todo: check for illegal chars
        throw invalid_argument("alias can't be empty!");

    for (Governor const & g : governors) {
        if (alias == g.name) {
            govCollision = true;
            return false;
        }
    }

    int oldIndex = -1;
    for (int i=0; i<sensorAliases.size(); i++){
        if (sensorAliases[i].alias == alias) {
            if (sensorAliases[i].id != sensorId) {
                aliasCollision = true;
                return false;
            } else {
                //already set
                return true;
            }
        } else if (sensorAliases[i].id == sensorId) {
            oldIndex = i;
            break;
        }
    }

    Alias a;
    a.id = sensorId;
    a.alias = alias;

    if (oldIndex == -1) {
        sensorAliases.push_back(a);
    } else {
        sensorAliases[oldIndex] = a;
    }

    return true;
}

bool Profile::addGovernor(Governor gov, bool& govCollision, bool& sensorAliasCollision) {
    sensorAliasCollision = govCollision = false;

    if (gov.name == "")//todo: check for illegal chars
        throw invalid_argument("name can't be empty!");

    for (const Governor & g : governors) {
        if (g.name == gov.name) {
            govCollision = true;
            return false;
        }
    }

    for (auto const & sa : sensorAliases) {
        if (gov.name == sa.alias) {
            sensorAliasCollision = true;
            return false;
        }
    }

    governors.push_back(gov);
    return true;
}

bool Profile::updateGovernor(int index, Governor newGov, bool& govCollision, bool& sensorAliasCollision) {
    sensorAliasCollision = govCollision = false;

    if (newGov.name == "")//todo: check for illegal chars
        throw invalid_argument("name can't be empty!");

    Governor oldGov = governors[index];

    if (oldGov == newGov &&
        oldGov.getErrors() == newGov.getErrors())
        return true;//already set

    //check for name collisions
    if (oldGov.name != newGov.name) {
        for (const Governor & g : governors) {
            if (g.name == newGov.name){
                govCollision = true;
                return false;
            }
        }
        for (auto const & sa : sensorAliases) {
            if (newGov.name == sa.alias) {
                sensorAliasCollision = true;
                return false;
            }
        }
    }

    governors[index] = newGov;

    return true;
}

void Profile::removeGovernor(int index) {
    governors.erase(governors.begin() + index);
}

bool Profile::addCurve(Curve curve) {
    if (curve.name == "")//todo: check for illegal chars
        throw invalid_argument("name can't be empty!");

    for (const Curve & c : curves) {
        if (c.name == curve.name) {
            return false;
        }
    }

    curves.push_back(curve);
    return true;
}

void Profile::removeCurve(int index) {
    curves.erase(curves.begin() + index);
}

bool Profile::updateCurve(int index, Curve newCurve, bool& nameCollision) {
    nameCollision = false;

    if (newCurve.name == "")//todo: check for illegal chars
        throw invalid_argument("name can't be empty!");

    Curve oldCurve = curves[index];

    if (oldCurve == newCurve)
        return false;//already set

    //check for name collisions
    if (oldCurve.name != newCurve.name) {
        for (const Curve & c : curves) {
            if (c.name == newCurve.name){
                nameCollision = true;
                return false;
            }
        }
    }

    curves[index] = newCurve;
    return true;
}

bool Profile::setGovernorForController(std::string controllerId, std::string governorName){
    for (int i=0; i<controllers.size(); ++i){
        auto c = controllers[i];
        if (c.id == controllerId) {
            if (c.governorName == governorName)
                return false;
            c.governorName = governorName;
            controllers[i] = c;
            return true;
        }
    }

    controllers.push_back({controllerId, governorName});
    return true;
}

bool Profile::removeController(std::string controllerId){
    for (auto it=controllers.begin(); it != controllers.end(); ++it) {
        if (it->id == controllerId) {
            controllers.erase(it);
            return true;
        }
    }
    return false;
}

