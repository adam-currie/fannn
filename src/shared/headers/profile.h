#pragma once

#include <string>
#include <vector>
#include "governor.h"
#include "curve.h"
#include <stdexcept>
#include <tuple>

namespace Fannn {

    class Profile {

        typedef struct Alias {
            std::string alias, id;
            bool operator==(const Alias&) const = default;
        } Alias;

        private:
            int updateIntervalMs = 2000;
            std::vector<Governor> governors;
            std::vector<Curve> curves;
            std::vector<Alias> sensorAliases;
        public:
            bool operator==(const Profile&) const = default;

            int getUpdateInterval() const { return updateIntervalMs; }
            void setUpdateInterval(int milliseconds){
                if (milliseconds < 0) throw std::out_of_range("negative interval");
                updateIntervalMs = milliseconds;
            }
            const std::vector<Governor>& getGovernors(){ return governors; }
            const std::vector<Curve>& getCurves(){ return curves; }
            const std::vector<Alias>& getSensorAliases(){ return sensorAliases; }

            std::string getAliasForSensor(std::string sensorId);
            std::string removeAliasForSensor(std::string sensorId);

            /**
             * @brief adds or updates the alias for a sensor, unless the alias is already used for a sensor or governor name
             * @return true if set or updated, false if no changes
             */
            bool setAliasForSensor(std::string sensorId, std::string alias, bool& govCollision, bool& aliasCollision);
            bool setAliasForSensor(std::string sensorId, std::string alias) {
                bool a,b;
                return setAliasForSensor(sensorId, alias, a,b);
            }
    };

}
