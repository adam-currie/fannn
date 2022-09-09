#include "sysfs_pwm_writer.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>

#define HWMON_ROOT "/sys/devices/platform"

using namespace std;
using namespace Fannn;

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
using directory_options = std::filesystem::directory_options;
using directory_entry = std::filesystem::directory_entry;
using path = std::filesystem::path;
using perms = std::filesystem::perms;

const regex PWM_REGEX("pwm[0-9]+");

SysfsPwmWriter::SysfsPwmWriter(){
    for(directory_entry entry : recursive_directory_iterator(HWMON_ROOT, directory_options::skip_permission_denied)){
        string fileStr = entry.path().filename();
        if(
            regex_match(fileStr, PWM_REGEX) && //regex first was fastest in a quick/dirty test
            !entry.is_directory() &&
            (entry.status().permissions() & perms::owner_write) != perms::none){
                ids.push_back(entry.path());
        }
    }
}

bool SysfsPwmWriter::write(string deviceId, double value){
    return true;//debug

    /*
        todo: might need to enable pwm here and then reset to the original value in a destructor, see:
        https://github.com/lm-sensors/lm-sensors/blob/master/prog/pwm/fancontrol
    */
   ofstream out(deviceId, std::ofstream::trunc);
   if (out) {
       out << value;
       return true;
   } else {
       return false;
   }
}
