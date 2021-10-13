#include "profile.h"

using namespace Fannn;
using namespace std;

string Profile::getAliasForSensor(std::string sensorId) {
    for (auto a : sensorAliases) {
        if (a.id == sensorId)
            return a.alias;
    }
    return {};
}

bool Profile::setAliasForSensor(std::string sensorId, std::string alias, bool& govCollision, bool& aliasCollision) {
    govCollision = aliasCollision = false;

    if (alias == "")//todo: check for illegal chars
        throw invalid_argument("alias can't be empty!");

    for (Governor g : governors) {
        if (alias == g.name()) {
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
        }
        if (sensorAliases[i].id == sensorId)
            oldIndex = i;
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
