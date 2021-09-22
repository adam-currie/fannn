#include "profile_engine.h"

using namespace std;
using namespace Fannn;

ProfileEngine::ProfileEngine(Profile profile, const shared_ptr<const IDeviceWriter> deviceWriter, const shared_ptr<const ISensorReader> sensorReader) 
  : profile(profile), deviceWriter(deviceWriter), sensorReader(sensorReader){
    for(auto & [govName, gov] : profile.governors){
        gov.readCurve = [](string s){
            //debug //todo
            return [](double x){ return x*3; };
        };
        gov.readSensorOrGovernor = [](string s){
            return 5;
        };
    }  
}

void ProfileEngine::runOnce(){
    //todo
}