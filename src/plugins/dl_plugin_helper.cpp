#include "dl_plugin_helper.h"

#include "proximate_temp_file.h"
#include <elf.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <vector>
#include <variant>

static constexpr unsigned char ELF64MAGIC[] = {ELFMAG0, 'E', 'L', 'F'};

using namespace Fannn::Plugins::Internal;

class FdWrapper {
    int fd;
    public:
        FdWrapper(const char * const path, int mode)
            : fd(open(path, mode)) {}
        FdWrapper(FdWrapper&&) = delete;
        FdWrapper(FdWrapper const&) = delete;
        FdWrapper& operator=(FdWrapper&&) = delete;
        FdWrapper& operator=(FdWrapper const&) = delete;
        ~FdWrapper() {
            if (fd != -1) {
                close(fd);
                fd = -1;
            }
        }
        operator int() const { return fd; }
};

//use errno for errors
bool seekAndRead(int fd, off_t start, void * data, size_t size) {
    if(-1 == lseek(fd, start, SEEK_SET))
        return false;

    if (size != read(fd, data, size))
        return false;

    return true;
}

/**
 * @brief   seekReadReturn seeks to a point, reads, then returns the cursor if successfull
 * @note    check errno on error
 * @return  success
 */
bool seekReadReturn(int fd, off_t start, void * data, size_t size) {
    off_t orig = lseek(fd, 0, SEEK_CUR);
    if (orig == -1)
        return false;

    if(-1 == lseek(fd, start, SEEK_SET))
        return false;

    if (size != read(fd, data, size))
        return false;

    if(-1 == lseek(fd, orig, SEEK_SET))
        return false;

    return true;
}

/**
 * @brief   checkGetterSymbol checks for matching getter function
 * @return  returns bool indicating if it's a match or not, or returns error message string
 * @note    jumps around but returns the cursor (unless theres an error)
 */
std::variant<bool, std::string> checkGetterSymbol(int fd, const Elf64_Shdr& symbolStringSectionHeader, const Elf64_Sym& symbol, const char * const getterName){
    bool ret = true;

    off_t originalCur = lseek(fd, 0, SEEK_CUR);
    if (originalCur == -1)
        return strerror(errno);

    if (-1 == lseek(fd, symbolStringSectionHeader.sh_offset + symbol.st_name, SEEK_SET))
        return strerror(errno);

    //check name
    const char * cPtr = getterName;
    while (true) {
        char c;
        if (1 != read(fd, &c, 1))
            return strerror(errno);

        if (c != *cPtr)
            goto cleanupAndReturnFalse;

        if (c == '\0') //we are done, cPtr also points to terminator, because we already compared
            break;

        cPtr++;
    };

    //check type
    if (ELF64_ST_TYPE(symbol.st_info) != STT_FUNC)
        goto cleanupAndReturnFalse;

cleanupAndReturnTrue:
    goto cleanup;
cleanupAndReturnFalse:
    ret = false;
cleanup:
    //return cursor for client code
    if (-1 == lseek(fd, originalCur, SEEK_SET))
        return strerror(errno);

    return ret;
}

//returns fd cursor to original position on success
bool validateDynSym(int fd, const char * const getterName, const size_t sectionHeadersOffset, const Elf64_Shdr& section, bool& looksLikeUserError, std::string& errMsg) {
    off_t originalCur = lseek(fd, 0, SEEK_CUR);
    if (originalCur == -1) {
        errMsg = strerror(errno);
        return false;
    }

    //section header for the string data we are going to look into for symbol names
    Elf64_Shdr symbolStringSection;
    if (!seekAndRead(fd, sectionHeadersOffset + section.sh_link*sizeof(Elf64_Shdr), &symbolStringSection, sizeof(Elf64_Shdr))){
        errMsg = strerror(errno);
        return false;
    }

    if (-1 == lseek(fd, section.sh_offset, SEEK_SET)) {
        errMsg = strerror(errno);
        return false;
    }

    bool hasSTB_GNU_UNIQUE = false;
    bool hasGetter = false;

    for (int symIndex = 0; symIndex < section.sh_size/section.sh_entsize; ++symIndex) {
        Elf64_Sym sym;
        if (sizeof(sym) != read(fd, &sym, sizeof(sym))) {
            errMsg = strerror(errno);
            return false;
        }

        if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE) {
            errMsg = "STB_GNU_UNIQUE binding mode is set on one or more symbols. if you are using gcc, compile with -fno-gnu-unique";
            hasSTB_GNU_UNIQUE = true;
            //if we don't have a getter yet, even though we already know we are returning false,
            //we need to keep looking for the getter
            if (hasGetter) {
                looksLikeUserError = true;
                return false;
            }
        }

        if (!hasGetter) {
            auto hasGetterOrError = checkGetterSymbol(fd, symbolStringSection, sym, getterName);
            if (std::holds_alternative<std::string>(hasGetterOrError)) {
                errMsg = std::get<std::string>(hasGetterOrError);
                return false;
            } else if (std::get<bool>(hasGetterOrError)) {
                hasGetter = true;
                if (hasSTB_GNU_UNIQUE) {
                    looksLikeUserError = true;
                    return false;
                }
            }
        }
    }

    if (hasSTB_GNU_UNIQUE && !hasGetter)//not user error, since no getter
        return false;

    //return cursor for client code
    if (-1 == lseek(fd, originalCur, SEEK_SET)) {
        errMsg = strerror(errno);
        return false;
    }

    return hasGetter && !hasSTB_GNU_UNIQUE;
}

/**
 * @brief   checks whether the elf data is valid for a plugin
 * @note    no garuntees that this is an exhaustive check(whatever that would mean)
 *          any file io errors result in this returning false
 * @param   fd file descriptor
 * @param   getterName name of the plugin getter func
 * @param   looksLikeUserError
 *              whether the issue is likely caused by the user messing something
 *              up as opposed to this just being a file that is not intended as a plugin
 * @param   errMsg error msg, empty for success
 * @return  whether it's a valid elf for a plugin
 */
bool validateElfForPlugin(int fd, const char * const getterName, bool& looksLikeUserError, std::string& errMsg) {
    //https://man7.org/linux/man-pages/man5/elf.5.html
    //https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
    const std::string INVALID_ELF = "file is not valid elf";

    auto invalid = [&] (const std::string e) {
        errMsg = e;
        return false;
    };

    Elf64_Ehdr header;
    int bytesRead = read(fd, &header, sizeof(header));
    if (sizeof(header) != bytesRead)
        return invalid(bytesRead > 0 ? INVALID_ELF : "failed to read file");

    //is elf64
    for (int i=0; i<sizeof(ELF64MAGIC); ++i)
        if (header.e_ident[i] != ELF64MAGIC[i])
            return invalid(INVALID_ELF);

    //architecture
    if (header.e_ident[4] != ELFCLASS64)
        return invalid("elf file is using not 64 bit");

    //section headers
    Elf64_Shdr section;
    size_t sectionCount;
    if (header.e_shnum == 0) {
        //sometimes if its too big to fit in e_shnum its in the reserved section instead
        if (!seekAndRead(fd, header.e_shoff, &section, sizeof(section)))
            return invalid(strerror(errno));
        //either the count is here because it couldnt fit in e_shnum, or this is zero too because there are no sections
        sectionCount = section.sh_size;
    } else {
        sectionCount = header.e_shnum;
    }

    if (sectionCount == 0)
        return invalid(INVALID_ELF);

    // skipping first section because it's reserved
    if (-1 == lseek(fd, header.e_shoff + sizeof(section), SEEK_SET))
        return invalid(strerror(errno));

    for (size_t i = 1; i < sectionCount; ++i ) {// skipping first section because it's reserved
        if (sizeof(section) != read(fd, &section, sizeof(section)))
           return invalid(strerror(errno));

        if (section.sh_type == SHT_DYNSYM) {
            if (validateDynSym(fd, getterName, header.e_shoff, section, looksLikeUserError, errMsg)) {
                looksLikeUserError = false;
                errMsg = {};
                return true;
            } else {
                return false;
            }
        }
    }

    looksLikeUserError = false;
    errMsg = "no dynamic symbol section found (SHT_DYNSYM)";
    return true;
}


bool symlinkOrHardCopy(int destFd, const char * const srcPath, const char * err) {
    assert(destFd != -1);

    FdWrapper srcFd(srcPath, O_RDONLY);
    if (srcFd == -1) {
        err = strerror(errno);
        return false;
    }

//use a reflink but fallback to hard copy if it's not supported or fails
#ifdef FICLONE
    if (-1 == ioctl(destFd, FICLONE, (int)srcFd))
#endif
    {
        //fallback to hard copy

        //start from the start
        if (-1 == lseek(destFd, 0, SEEK_SET)) {
            err = strerror(errno);
            return false;
        }

        size_t size;
        char buf[BUFSIZ];
        while ((size = read(srcFd, buf, BUFSIZ)) > 0) {
            if (-1 == write(destFd, buf, size)) {
                err = strerror(errno);
                return false;
            }
        }

        //return to the start to be nice to client code
        if (-1 == lseek(destFd, 0, SEEK_SET)) {
            err = strerror(errno);
            return false;
        }
    }

    return true;
}

std::optional<DlObj> Fannn::Plugins::Internal::loadPluginDlo(const char * const path, const char * const getterName, bool& looksLikeUserError, std::string& errMsg) {
    Fannn::FileIO::ProximateTempFile tempFile;

    auto failNonUsrErr = [&] (const std::string e) {
        looksLikeUserError = false;
        errMsg = e;
        return std::nullopt;
    };

    FdWrapper tempFd(tempFile.getPath(), O_RDWR);
    if (tempFd == -1)
        return failNonUsrErr("io error: " + std::string(strerror(errno)));

    const char * cerr;

    /*
     * copy so that our validation and our dynamic linking get the same data incase the
     * user swaps the file or it gets rolled back by a backup system, etc.
     * todo: we should probably be trying to hardlink before jumping directly to copying.
     */
    if (!symlinkOrHardCopy(tempFd, path, cerr))
        return failNonUsrErr("io error: " + std::string(cerr));

    if (!validateElfForPlugin(tempFd, getterName, looksLikeUserError, errMsg))
        return std::nullopt;

    auto dlo = Internal::DlObj::open(tempFile.getPath(), cerr);
    return dlo ?
        std::move(dlo) :
        failNonUsrErr(std::string(cerr));
}
