#pragma once

#include <string>
#include <vector>
#include "governor.h"
#include "curve.h"
#include <stdexcept>
#include <sensor_alias.h>
#include <controller_alias.h>

namespace Fannn {

    class Profile{
        private:
            int updateIntervalMs = 2000;
            std::vector<Governor> governors;
            std::vector<Curve> curves;
            std::vector<SensorAlias> sensorAliases;
            std::vector<ControllerAlias> controllerAliases;
        public:
            int getUpdateInterval(){ return updateIntervalMs; }
            void setUpdateInterval(int milliseconds){
                if (milliseconds < 0) throw std::out_of_range("negative interval");
                updateIntervalMs = milliseconds;
            }
            const std::vector<Governor>& getGovernors(){ return governors; }
            const std::vector<Curve>& getCurves(){ return curves; }
            const std::vector<SensorAlias>& getSensorAliases(){ return sensorAliases; }
            const std::vector<ControllerAlias>& getControllerAliases(){ return controllerAliases; }
    };

}