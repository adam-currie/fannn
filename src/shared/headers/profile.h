#pragma once

#include <string>
#include <vector>
#include "governor.h"
#include "curve.h"
#include <stdexcept>

namespace Fannn {

    class Profile {
        public:
            typedef struct Alias {
                std::string id, alias;
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
            const std::vector<Governor>& getGovernors() const { return governors; }
            const std::vector<Curve>& getCurves() const { return curves; }
            const std::vector<Alias>& getSensorAliases() const { return sensorAliases; }

            std::string getAliasForSensor(std::string sensorId);
            std::string removeAliasForSensor(std::string sensorId);

            /**
             * @brief adds or updates the alias for a sensor, unless the alias is already used for a sensor alias or governor name
             * @return true if set or updated, false if no changes
             */
            bool addOrUpdateSensorAlias(std::string sensorId, std::string alias, bool& govCollision, bool& aliasCollision);

            /**
             * @brief adds or updates the alias for a sensor, unless the alias is already used for a sensor alias or governor name
             * @return true if set or updated, false if no changes
             */
            bool addOrUpdateSensorAlias(std::string sensorId, std::string alias) {
                bool a,b;
                return addOrUpdateSensorAlias(sensorId, alias, a,b);
            }

            /**
             * @brief adds or updates a Governor, unless it's name is already used for a sensor alias
             * @return true if set or updated, false if no changes
             */
            bool addOrUpdateGovernor(Governor gov, bool& aliasCollision);
            /**
             * @brief adds or updates a Governor, unless it's name is already used for a sensor alias
             * @return true if set or updated, false if no changes
             */
            bool addOrUpdateGovernor(Governor gov) {
                bool b;
                return addOrUpdateGovernor(gov, b);
            }

    };
}
