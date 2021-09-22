#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <string>

namespace Fannn {

    class UserConfig {
        std::string filename;
        public:
            static const int DEFAULT_INTERVAL_MS = 2000;
            UserConfig(std::string filename);
            int load();
            int save();
            
            std::string getFilename();
            void setFilename(std::string filename);
            bool checkForExternalChanges();
    };

}