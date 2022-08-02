#pragma once

#include <string>
#include <vector>

namespace Fannn {

    class ISensorReader {
        public:
            /**
             * @brief   gets the available sensors
             * 
             * @note    this function won't be called by the daemon more than once under normal operation.
             * @return  std::vector<std::string> the sensor ids
             */
            virtual std::vector<std::string> const getAll() = 0;

            /**
             * @brief   rescans for/updates the list of sensors
             * 
             * @note    can be a no-op.
             * @note    this function won't be called by the daemon more than once under normal operation.
             */
            virtual void rescan() = 0;

            /**
             * @brief   reads the value of a sensor
             * 
             * @param   sensorId the sensors id
             * @return  the value or NaN if a valid sensor with that id is not available
             */
            virtual double read(std::string sensorId) = 0;
    };

}
