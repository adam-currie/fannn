#pragma once

#include <string>
#include <memory>
#include "governor.h"
#include "expression.h"
#include "profile.h"
#include "i_sensor_reader.h"

namespace Fannn {

    class ProfileGovernorExecContext final : public Expression::INamedFuncContext {
        
        double readSensorByIdOrAlias(std::string idOrAlias) const;

        public: 
            ISensorReader& sensorReader;
            Profile * profile;

            ProfileGovernorExecContext(ISensorReader& sensorReader, Profile * profile) 
                : sensorReader(sensorReader), profile(profile) {}

            double lookupAndExec(const std::string& id, std::string & errMsg) const override;
            double lookupAndExec(const std::string& id, std::string & errMsg, double arg) const override;
    };
    
}
