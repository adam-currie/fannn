#include "profile.h"

using namespace Fannn;
using namespace std;

string Profile::getAliasForSensor(std::string sensorId) {
    for (auto const & a : sensorAliases) {
        if (a.id == sensorId)
            return a.alias;
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
    //check gov issues
    for (const Fannn::Governor & g : governors)
        if (!g.getErrors().empty())
            return true;

    //todo: more issues

    return false;
}

bool Profile::addOrUpdateSensorAlias(std::string sensorId, std::string alias, bool& govCollision, bool& aliasCollision) {
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
            }
            return true;//already set
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
