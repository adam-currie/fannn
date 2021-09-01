#include "user_config.h"
#include "atomic_file_writer.h"

using namespace std;

#define USER_CONFIG_FILE_DIR "config/"

UserConfig::UserConfig(string filename) : filename(filename){}

void UserConfig::setFilename(string filename){
    this->filename = filename;
}

string UserConfig::getFilename(){
    return this->filename;
}

int UserConfig::load(){
    //todo
    return 0;
}

int UserConfig::save(){
    AtomicFileWriter writer(AtomicFileWriter(string("config/").append(filename)));

    //todo: send data to writer

    writer.atomicWrite();
    return 0;//todo: errors?
}

bool UserConfig::checkForExternalChanges(){
    return true;//todo
}
