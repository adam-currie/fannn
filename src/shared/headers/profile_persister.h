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
            //todo: better errors
            class LoadError : std::runtime_error {
                public:
                    std::string const name;
                    int errnoSnapshot;
                    LoadError(std::string name, std::optional<std::string> msg = std::optional<std::string>())
                          : std::runtime_error(msg.value_or("failed to load profile:'" + name + "'")),
                            name(name),
                            errnoSnapshot(errno) {}
            };

            static bool isValidProfileName(std::string name);

            /**
             * @brief   Get the Active Profile Name object
             * @throws  LoadError failed to load the profile name
             * @note    if the file meant to contain this name doesn't exist or is blank,
             *          that's not an error and it will default to the first profile
             *          it's only an error if it can't access or create it on disk
             * @return  the profile name
             */
            static std::string getActiveProfileName();
            static void setActiveProfileName(std::string name);

            /**
             * @brief getProfileNames
             * @return list of profile names, this is never empty, if there are no profiles, i'll make you one dw
             */
            static const std::vector<std::string> getProfileNames();

            ProfilePersister(std::string name);

            /**
             * @brief loads the profile from persistant storage
             * @throws LoadError if the the profile could not be loaded from persistant storage
             */
            void load();

            void save(); 

            bool unsavedChanges() const {
                return scratch != lastPersistanceSynced;
            }

            Profile& profile() { return scratch.profile; }
            Profile const & profile() const { return scratch.profile; }

            void setName(std::string name) { scratch.name = name; }
            
            std::string getName() const { return scratch.name; }
    };

}
