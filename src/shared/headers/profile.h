#pragma once

#include <string>
#include <vector>
#include "governor.h"
#include "curve.h"
#include <stdexcept>

namespace Fannn {

    class Profile {
        private:
            int updateIntervalMs = 2000;
            std::vector<Governor> governors;
            std::vector<Curve> curves;
        public:
            bool operator==(const Profile&) const = default;

            int getUpdateInterval() const { return updateIntervalMs; }
            void setUpdateInterval(int milliseconds){
                if (milliseconds < 0) throw std::out_of_range("negative interval");
                updateIntervalMs = milliseconds;
            }
            const std::vector<Governor>& getGovernors(){ return governors; }
            const std::vector<Curve>& getCurves(){ return curves; }

            std::string getAliasForSensor(std::string sensorId);
    };

}
