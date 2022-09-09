#pragma once

#include <string>
#include <vector>

namespace Fannn {
        
    class IDeviceWriter {
        public:
            /**
             * @brief   gets the available device outputs
             * 
             * @note    this function won't be called by the daemon under normal operation.
             * @return  std::vector<std::string> the device ids
             */
            virtual std::vector<std::string> const getAll() = 0;

            /**
             * @brief   rescans for device outputs
             * 
             * @note    can be a no-op.
             */
            virtual void rescan() = 0;

            /**
             * @brief   sets the value of a device
             * 
             * @param   deviceId 
             * @param   value 
             * @return  true if the device was set, false if it's not a valid accessible device
             */
            virtual bool write(std::string deviceId, double value) = 0;
    };

}
