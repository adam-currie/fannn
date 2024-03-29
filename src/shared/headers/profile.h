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

            typedef struct Controller {
                std::string id, governorName;
                bool operator==(const Controller&) const = default;
            } Controller;

        private:
            int updateIntervalMs = 2000;
            std::vector<Governor> governors;
            std::vector<Curve> curves;
            std::vector<Alias> sensorAliases;
            std::vector<Controller> controllers;

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
            const std::vector<Controller>& getControllers() const { return controllers; }

            size_t governorCount() const { return governors.size(); }
            Governor& governorAt(size_t index) { return governors[index]; }
            Governor const * getGovernor(const std::string & name) {
                for (auto & g : governors) 
                    if (g.name == name) return &g;
                return nullptr;
            }
 
            std::string getAliasForSensor(std::string sensorId) const;
            std::string removeAliasForSensor(std::string sensorId);


            bool hasIssues() const;

            /**
             * @brief adds or updates the alias for a sensor, unless the alias is already used for a sensor alias or governor name
             * @return true if successfully added or updated, or if already set
             */
            bool setSensorAlias(std::string sensorId, std::string alias, bool& govCollision, bool& aliasCollision);

            /**
             * @brief adds or updates the alias for a sensor, unless the alias is already used for a sensor alias or governor name
             * @return true if successfully added or updated, or if already set
             */
            bool setSensorAlias(std::string sensorId, std::string alias) {
                bool a,b;
                return setSensorAlias(sensorId, alias, a,b);
            }

            void addGovernor(Governor gov, bool& govCollision, bool& sensorAliasCollision);

            /**
             * @brief   updates the governor if it is different, and it's name doesn't collide
             *
             * @note    governors with different errors count as different governors in this context
             * @return  true if governor was updated, false if issues prevent the change or if no changes to be made
             */
            bool updateGovernor(int index, Governor gov, bool& govCollision, bool& sensorAliasCollision);
            void removeGovernor(int index);

            bool addCurve(Curve curve);
            void removeCurve(int index);

            /**
             * @brief updates a curve
             * @param nameCollision
             *      true if the curve can't be updated because it's new name collides with another curves name
             * @return true if curve was updated, false if issues prevent the change or if no changes to be made
             */
            bool updateCurve(int index, Curve curve, bool& nameCollision);

            /**
             * @brief setGovernorForController
             * @return false if already set
             */
            bool setGovernorForController(std::string controllerId, std::string governorName);
            /**
             * @brief removeController
             * @return true on success, false if nothing to remove
             */
            bool removeController(std::string controllerId);

    };
}
