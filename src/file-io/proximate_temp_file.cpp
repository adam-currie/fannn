#include "proximate_temp_file.h"

#include <filesystem>
#include <cstring>
#include <sys/stat.h>
#include <sys/time.h>
#include <cassert>

using namespace Fannn;
using namespace Fannn::FileIO;

bool ProximateTempFile::initialized = false;

static void getRandAlphaNum(char *out, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "-_";
    srand(rand()^time(NULL));
    for (int i = 0; i < len; ++i) 
        out[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    out[len] = '\0';
}

/**
 * @brief deletes everything under our temp folder recursively unless it's fresh
 */
void ProximateTempFile::cleanupTempDir() {
    struct stat statbuf;
    timeval now;
    //todo: check if this is a good time to compare against stat, might need to use clock_gettime with a specific format
    gettimeofday(&now, nullptr);

    std::filesystem::create_directory(TEMP_DIR);
    for (const auto& entry : std::filesystem::directory_iterator(TEMP_DIR)){
        //except fresh files
        if( entry.is_regular_file() &&
            stat(entry.path().c_str(), &statbuf) || // < if error
            (now.tv_sec - statbuf.st_ctim.tv_sec) < TEMP_FILE_OLD_AGE_SECONDS)
                continue;
        std::filesystem::remove_all(entry.path());
    }
}

void ProximateTempFile::staticInit() {
    if (!initialized) {
        cleanupTempDir();
        ProximateTempFile::initialized = true;
    }
}

ProximateTempFile::ProximateTempFile(ProximateTempFile&& other) {
    assert(other.path[0] != '\0');
    assert(sizeof(path) == sizeof(other.path));
    memcpy(path, other.path, sizeof(path));
    other.path[0] = '\0';
}

ProximateTempFile::ProximateTempFile() {
    staticInit();

    memcpy(path, TEMP_DIR, sizeof(TEMP_DIR));
    //in a directory with a petabyte of min length files this would colide 1 in 3.24518554 × 1^17 times
    getRandAlphaNum(path+sizeof(TEMP_DIR)-1, TEMP_FILENAME_LENGTH);

    std::filesystem::create_directory(TEMP_DIR);
    FILE* fp = fopen(path, "w");
    if(fp == nullptr)//basically the user would have be trying to make this fail
        throw std::runtime_error("failed to create temporary file");
    fclose(fp);
}

ProximateTempFile::~ProximateTempFile() {
    if (path[0] != '\0') {
        /*  stoping exceptions cause i dont care,
            its the responsibility of the next process execution to delete it if this doesnt work*/
        std::error_code ec;
        std::filesystem::remove(path, ec);
        path[0] = '\0';
    }
}
