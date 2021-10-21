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

void serialize(nlohmann::ordered_json& j, vector<Profile::Alias> const & aliases) {
    nlohmann::json list;
    for (auto const & a : aliases) {
        list.push_back({
            {"id", a.id},
            {"alias", a.alias}
        });
    }
    j["sensors"] = list;
}

void serialize(nlohmann::ordered_json& j, std::vector<Governor> const & governors) {
    nlohmann::json list;
    for (auto const & g : governors) {
        list.push_back({
            {"name", g.name},
            {"exp", g.getExpressionStr()}
        });
    }
    j["governors"] = list;
}

ProfilePersister::ProfilePersister(string name)
    : scratch({name, Profile()}) {}

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

    for (auto const & s : j["sensors"])
        scratch.profile.addOrUpdateSensorAlias(s["id"], s["alias"]);

    bool a,b;//todo
    for (auto const & g : j["governors"])
        scratch.profile.addGovernor(Fannn::Governor(g["name"], g["exp"]),a,b);

    lastPersistanceSynced = scratch;
}

void ProfilePersister::save() {
    nlohmann::ordered_json j;

    j["updateIntervalMs"] = scratch.profile.getUpdateInterval();
    serialize(j, scratch.profile.getSensorAliases());
    serialize(j, scratch.profile.getGovernors());

    (AtomicFileWriter(USER_CONFIG_FILE_DIR + scratch.name) << j.dump(4))
        .atomicWrite();

    lastPersistanceSynced = scratch;
}
