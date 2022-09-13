#include <time.h>
#include <sensors/error.h>
#include <iostream>
#include <cmath>
#include <signal.h>
#include "governor.h"
#include "profile_engine.h"
#include "profile_persister.h"
#include "plugins_composite_device_writer.h"
#include "plugins_composite_sensor_reader.h"
#include "min_interval_stepper.h"

using namespace std;
using namespace Fannn;

//SIGNAL FLAGS
bool terminating = false;
bool needsReload = false;

MinIntervalStepper stepper;
optional<ProfileEngine> engine;

void load() {
    //todo: log exception and retry? maybe the system will do this for me?
    ProfilePersister persister(ProfilePersister::getActiveProfileName());
    persister.load();

    engine.emplace(
        persister.profile(),
        PluginsCompositeDeviceWriter::instance(),
        PluginsCompositeSensorReader::instance()
    );

    stepper = MinIntervalStepper(persister.profile().getUpdateInterval());
}

void initSignalHandlers() {
    {
        struct sigaction sigAct{};
        sigAct.sa_handler = [](int sig){ 
            terminating = true;
        };
        sigemptyset(&sigAct.sa_mask);
        sigaction(SIGTERM, &sigAct, nullptr);
    }
    {
        struct sigaction sigAct{};
        sigAct.sa_handler = [](int sig){ 
            needsReload = true;
        };
        sigemptyset(&sigAct.sa_mask);
        sigaction(SIGHUP, &sigAct, nullptr);
    }
}

int main() {
    initSignalHandlers();
    load();

    while (!terminating) {
        if (needsReload) {
            load();
            needsReload = false;
        }

        engine->runOnce();

        cout << endl << endl << "MAIN LOOP ITERATION" << endl << endl << endl;//debug 

        while(
            !terminating &&
            !needsReload &&
            !stepper.step() //returns false if interputed while waiting to step, so try again
        );
    }

    return 0;
}
