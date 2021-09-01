#include "atomic_file_writer.h"

#include <unistd.h>
#include <stdexcept>
#include <cerrno>
#include <filesystem>
#include <sys/stat.h>
#include <sys/time.h>

using namespace std;

#define TEMP_FILENAME_LENGTH 16

//we have our own temp dir because the system one might be on another drive and we want atomic file replacement
#define TEMP_DIR "temp/"
#define TEMP_FILE_OLD_AGE_SECONDS 60

class AtomicFileWriter::Impl {
    public:
        char tmpFilePath[sizeof(TEMP_DIR)+TEMP_FILENAME_LENGTH] = TEMP_DIR;
        string targetPath;
        FILE *tmpFp;
};

static void getRandAlphaNum(char *out, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "-_";//added to hit 64
    srand((unsigned) time(NULL));
    for (int i = 0; i < len; ++i) 
        out[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    out[len] = '\0';
}

static void ensureDirectoryStructure(string path){
    auto parent = filesystem::path(path).parent_path();
    if (!parent.empty())
        filesystem::create_directories(parent);
}

/**
 * @brief deletes everything under our temp folder recursively unless it's fresh
 */
static void cleanupTempDir(){
    struct stat statbuf;
    timeval now;
    gettimeofday(&now, nullptr);

    filesystem::create_directory(TEMP_DIR);
    for (const auto& entry : std::filesystem::directory_iterator(TEMP_DIR)){
        //except fresh files
        if( entry.is_regular_file() &&
            stat(entry.path().c_str(), &statbuf) || // < if error
            (now.tv_sec - statbuf.st_ctim.tv_sec) < TEMP_FILE_OLD_AGE_SECONDS)
                continue;
        std::filesystem::remove_all(entry.path());
    }
}

AtomicFileWriter::AtomicFileWriter(string filename) : pImpl{std::make_unique<Impl>()}{
    pImpl->targetPath = filename;

    cleanupTempDir();

    //in a directory with a petabyte of min length files this would colide 1 in 3.24518554 × 1^17 times
    getRandAlphaNum(pImpl->tmpFilePath+sizeof(TEMP_DIR)-1, TEMP_FILENAME_LENGTH);

    filesystem::create_directory(TEMP_DIR);
    pImpl->tmpFp = fopen(pImpl->tmpFilePath, "w");
    if(pImpl->tmpFp == nullptr)//basically the user would have be trying to make this fail
        throw runtime_error("failed to create temporary file for writing");
}

void AtomicFileWriter::atomicWrite(){
    //flush to file system
    if(fflush(pImpl->tmpFp))
        throw runtime_error(string("failed to flush temporary file to the filesystem. errno:") + to_string(errno));

    //need to hard flush incase the system crashes after replacing the target file
    //is replaced with our temp file but before our temp file contents are are physically written to disk
    if (fsync(fileno(pImpl->tmpFp)))
        throw runtime_error(string("failed to flush temporary file to disk. errno:") + to_string(errno)); 

    fclose(pImpl->tmpFp);

    ensureDirectoryStructure(pImpl->targetPath);
    if(rename(pImpl->tmpFilePath, pImpl->targetPath.c_str()))
        throw runtime_error(string("failed to replace file with temporary file. errno:") + to_string(errno));
}

AtomicFileWriter::~AtomicFileWriter() = default;