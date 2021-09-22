#pragma once

#include <string>
#include <map>
#include "governor.h"
#include "curve.h"
#include <stdexcept>

namespace Fannn {

    class Profile{
        private:
            int updateIntervalMs = 2000;
        public:
            int getUpdateInterval(){ return updateIntervalMs; }
            void setUpdateInterval(int milliseconds){
                if (milliseconds < 0) throw std::out_of_range("negative interval");
                updateIntervalMs = milliseconds;
            }
            std::map<std::string, std::string> aliasedSensors;
            std::map<std::string, std::string> aliasedOutputs;
            std::map<std::string, Curve> curves;
            std::map<std::string, Governor> governors;
    };

}