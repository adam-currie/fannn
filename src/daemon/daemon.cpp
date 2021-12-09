#include <time.h>
#include <sensors/error.h>
#include <iostream>
#include <cmath>
#include <signal.h>
#include "governor.h"
#include "profile_engine.h"
#include "profile_persister.h"
#include "composite_device_writer.h"
#include "composite_sensor_reader.h"
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
    ProfilePersister active(ProfilePersister::getActiveProfileName());
    active.load();

    engine.emplace(
        active.profile(), 
        CompositeDeviceWriter::instance(), 
        CompositeSensorReader::instance()
    );

    stepper = MinIntervalStepper(active.profile().getUpdateInterval());
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
            !stepper.step()
        );
    }

    return 0;
}
