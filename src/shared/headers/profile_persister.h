#pragma once

#include <string>
#include <stdexcept>
#include <optional>
#include "profile.h"

namespace Fannn {
    
    class ProfilePersister {

        struct NamedProfile {
            std::string name;
            Profile profile;
            bool operator==(const NamedProfile&) const = default;
            bool operator==(const Profile& p) const { return profile == p; }
        } typedef NamedProfile;

        std::optional<NamedProfile> lastPersistanceSynced;
        NamedProfile scratch;

        public:
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

            bool unsavedChanges() const { return scratch != lastPersistanceSynced; }

            Profile& profile() { return scratch.profile; }
            Profile const & constProfile() const { return scratch.profile; }

            void setName(std::string name) { scratch.name = name; }
            
            std::string getName() const { return scratch.name; }
    };

}
