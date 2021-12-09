#include "profile_engine.h"
#include <iostream>
#include "composite_device_writer.h"
#include "cmath"

using namespace std;
using namespace Fannn;

//returns the index of the value, wether added or already present
inline size_t appendAndOrGetIndex(vector<Governor> & vec, Governor const & gov) {
    size_t preAddSize = vec.size();

    for (int i=0; i<preAddSize; i++)
        if (vec[i].name == gov.name) 
            return i;

    vec.push_back(gov);
    return preAddSize;
}

inline void printParseErrors(const Governor& gov, const string& controller) {
    cout << "governor '" << gov.name << "' for controller '" << controller << "' contains parse errors:" << endl;
    for (auto const & error : gov.getParseErrors())
        cout << "\t" << error.errMsg << endl;
}

inline void printExecErrors(const Governor& gov, const string& controller) {
    cout << "governor '" << gov.name << "' for controller '" << controller << "' contains execution errors:" << endl;
    for (auto const & error : gov.getExecutionErrors())
        cout << "\t" << error.errMsg << endl;
}

ProfileEngine::ProfileEngine(Profile profile, IDeviceWriter& deviceWriter, ISensorReader& sensorReader) 
    :   profile(profile), 
        deviceWriter(deviceWriter), 
        context(sensorReader, &(this->profile))
{
    for (auto const & c : profile.getControllers()) {
        bool foundValid = false;

        for (size_t i = 0; i < profile.governorCount(); ++i) {
            auto const & gov = profile.governorAt(i);

            if (gov.name == c.governorName) {
                if (gov.getParseErrors().empty()) {
                    controllers.push_back({c.id, i});
                } else {
                    printParseErrors(gov, c.id);
                }
                break;
            }
        }
        
    }
}


    

void ProfileEngine::runOnce(){
    /*
        todo: maybe try caching, 
        this is only going to be good if the branch prediction is really good, 
        most users will have a 1 to 1 relationship between govs and controllers
        so if this is not basically branch predicted away then it's not worth it
    */

    /*
     * todo:    do a rescan here at the start of the loop if
     *          a sensor stops being valid, but not every time if it stays invalid,
     *          maybe only once every few minutes or something
     *          (variable set by calling code based on number of iterations)
     */

    for (const auto & c : controllers) {
        Governor& gov = profile.governorAt(c.govIndex);
        double v = gov.constExec(context);
        if (!isnan(v)) {
            deviceWriter.write(c.id, v);
        } else {
            printExecErrors(gov, c.id);
        }
    }
}
