#pragma once

#include <stddef.h>

namespace Fannn::FileIO {

    /**
     * @brief Temp file that is stored under the current directory,
     *        usually so that its on the same disk partion for performance/reliability reasons.
     * @note  These files are deleted when their object is destroyed.
     *        During static initialization files older than a certain age
     *        are also deleted incase of zombies.
     */
    class ProximateTempFile {
        static constexpr size_t TEMP_FILENAME_LENGTH = 16;
        static constexpr size_t TEMP_FILE_OLD_AGE_SECONDS = 864000;//864000 is 10 days
        static constexpr char TEMP_DIR[] = "temp/";

        static bool initialized;

        char path[sizeof(TEMP_DIR)+TEMP_FILENAME_LENGTH];

        static void cleanupTempDir();
        static void staticInit();

        public:
            ProximateTempFile();
            ProximateTempFile(ProximateTempFile const&) = delete;
            void operator=(ProximateTempFile const&) = delete;
            void operator=(ProximateTempFile &&) = delete;
            ProximateTempFile(ProximateTempFile && other);

            ~ProximateTempFile();
            char const * const getPath() const {
                return path;
            }
    };

}
