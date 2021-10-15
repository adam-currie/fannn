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
            }//else already set
            return false;
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

bool Profile::addOrUpdateGovernor(Governor gov, bool& sensorAliasCollision) {
    sensorAliasCollision = false;

    if (gov.name == "")//todo: check for illegal chars
        throw invalid_argument("name can't be empty!");

    for (auto const & sa : sensorAliases) {
        if (gov.name == sa.alias) {
            sensorAliasCollision = true;
            return false;
        }
    }

    for (int i=0; i<governors.size(); i++){
        if (governors[i].name == gov.name) {
            if (governors[i] == gov) {
                //already set, no change needed
                return false;
            } else {
                governors[i] = gov;
                return true;
            }
        }
    }

    governors.push_back(gov);
    return true;
}

void Profile::removeGovernor(int index) {
    governors.erase(governors.begin() + index);
}

bool Profile::removeGovernor(string name) {
    for (auto iter = governors.begin(); iter != governors.end(); ++iter) {
        if (iter->name == name) {
            governors.erase(iter);
            return true;
        }
    }
    return false;
}
