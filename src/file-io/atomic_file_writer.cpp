#include "atomic_file_writer.h"
#include "proximate_temp_file.h"

#include <unistd.h>
#include <stdexcept>
#include <cerrno>
#include <filesystem>
#include <sys/stat.h>
#include <sys/time.h>

using namespace std;
using namespace Fannn;
using namespace Fannn::FileIO;

class AtomicFileWriter::Impl {
    public:
        ProximateTempFile temp = {};//todo: check destructor is called
        string targetPath;
        FILE *tmpFp;//todo: close?
};

static void ensureDirectoryStructure(string path){
    auto parent = filesystem::path(path).parent_path();
    if (!parent.empty())
        filesystem::create_directories(parent);
}

AtomicFileWriter::AtomicFileWriter(string filename) : pImpl{std::make_unique<Impl>()} {
    pImpl->targetPath = filename;

    pImpl->tmpFp = fopen(pImpl->temp.getPath(), "w");
    if(pImpl->tmpFp == nullptr)//basically the user would have be trying to make this fail
        throw runtime_error("failed to create temporary file for writing");
}

AtomicFileWriter& AtomicFileWriter::operator<<(const char* s){
    fputs(s, pImpl->tmpFp);
    return *this;
}

void AtomicFileWriter::atomicWrite(){
    //flush to file system
    if(fflush(pImpl->tmpFp))
        throw runtime_error(string("failed to flush temporary file to the filesystem. errno:") + to_string(errno));

    //need to hard flush incase the system crashes after the target file
    //is replaced with our temp file but before our temp file contents are are physically written to disk
    //todo: this is slow and unneeded with some filesystems/configurations, but is generally fast and required with most configurations of ext4
    if (fsync(fileno(pImpl->tmpFp)))
        throw runtime_error(string("failed to flush temporary file to disk. errno:") + to_string(errno)); 

    ensureDirectoryStructure(pImpl->targetPath);
    if(rename(pImpl->temp.getPath(), pImpl->targetPath.c_str()))
        throw runtime_error(string("failed to replace file with temporary file. errno:") + to_string(errno));
}

AtomicFileWriter::~AtomicFileWriter() = default;
