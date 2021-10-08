#include "profile_persister.h"
#include <fstream>
#include <filesystem>
#include "atomic_file_writer.h"
#include "json.hpp"

#define USER_CONFIG_FILE_DIR "config/"
#define ACTIVE_PROFILE_NAME_PATH USER_CONFIG_FILE_DIR "active-profile"

using namespace std;
using namespace Fannn;
using LoadError = ProfilePersister::LoadError;

string readFile(string path) {
    ifstream fs(path, std::ios::in | std::ios::binary);
    if(!fs) throw runtime_error("failed to open file");

    std::string contents;
    fs.seekg(0, std::ios::end);
    contents.resize(fs.tellg());
    fs.seekg(0, std::ios::beg);
    fs.read(&contents[0], contents.size());
    fs.close();
    
    return(contents);
}

void ensureDirectoryStructure(string path){
    auto parent = filesystem::path(path).parent_path();
    if (!parent.empty())
        filesystem::create_directories(parent);
}

const vector<string> ProfilePersister::getProfileNames(){
    ensureDirectoryStructure(USER_CONFIG_FILE_DIR);
    vector<string> names;

    auto sortedInsert = [&](string s){
        for(auto i = names.begin(); i != names.end(); ++i)
            if (*i > s) {
                names.insert(i, s);
                return;
            }
        names.push_back(s);
    };

    for (const auto & entry : filesystem::directory_iterator(USER_CONFIG_FILE_DIR))
        sortedInsert(entry.path().filename());

    return names;
}

string ProfilePersister::getActiveProfile() {
    string indexPath(ACTIVE_PROFILE_NAME_PATH);
    ensureDirectoryStructure(indexPath);
    ifstream fs (indexPath);
    string activeProfileName;

    if (fs.is_open()){
        if(!getline(fs, activeProfileName)){
            throw runtime_error("failed to load active profile name, could not read profile name from:'" + indexPath + "'");
        }
    }else{
        throw runtime_error("failed to load active profile name, could not open:'" + indexPath + "'");
    }

    return activeProfileName;
}

void ProfilePersister::setActiveProfile(string name) {
    (AtomicFileWriter(ACTIVE_PROFILE_NAME_PATH) << name)
        .atomicWrite();
}

ProfilePersister ProfilePersister::loadActiveProfile() {
    string prevAttempt = "";
    string active; 

    /* don't keep trying with the same active profile path, however if it has changed then
     * that's probably the problem(active profile renamed, or changed and old one deleted) so try again */
    while(prevAttempt != (active = getActiveProfile())){
        try{
            ProfilePersister p(active);
            p.load();
            return p;
        }catch(LoadError){}
    }
    throw LoadError(active, "failed to load active profile:'" + active + "', probably the file has been deleted");
}

ProfilePersister::ProfilePersister(string name) 
    : scratch({name, Profile()}) {}

void ProfilePersister::load() {
    string path(USER_CONFIG_FILE_DIR+scratch.name);

    string fileContents;
    try {
        fileContents = readFile(path);
    } catch (runtime_error) {
        throw LoadError(scratch.name);
    }

    nlohmann::json j = nlohmann::json::parse(fileContents);

    scratch.profile.setUpdateInterval(j["updateIntervalMs"]);

    lastPersistanceSynced = scratch;
}

void ProfilePersister::save() {
    nlohmann::json j;

    j["updateIntervalMs"] = scratch.profile.getUpdateInterval();

    (AtomicFileWriter(USER_CONFIG_FILE_DIR + scratch.name) << j.dump())
        .atomicWrite();

    lastPersistanceSynced = scratch;
}
