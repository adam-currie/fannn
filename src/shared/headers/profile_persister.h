#pragma once

#include <string>
#include <stdexcept>
#include <optional>
#include "profile.h"

namespace Fannn {
    
    class ProfilePersister {
        std::string name;

        public:
            Profile profile;

            class LoadError : std::runtime_error {
                public:
                    std::string const name;
                    LoadError(std::string name, std::optional<std::string> msg = std::optional<std::string>())
                    : std::runtime_error(msg.value_or("failed to load profile:'" + name + "'")), name(name) {}
            };

            static std::string getActiveProfile();
            static void setActiveProfile(std::string name);

            /**
             * @brief loads the active profile
             * @throws LoadError if the profile cannot be loaded, probably because the the file has been deleted
             */
            static ProfilePersister loadActiveProfile();

            static const std::vector<std::string> getProfileNames();

            ProfilePersister(std::string name);

            /**
             * @brief loads the profile from persistant storage
             * @throws LoadError if the the profile could not be loaded from persistant storage
             */
            void load();

            void save(); 
    };

}