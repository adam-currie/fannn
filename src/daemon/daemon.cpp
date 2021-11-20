#include <time.h>
#include <sensors/error.h>
#include <iostream>
#include <cmath>
#include <signal.h>
#include "governor.h"
#include "profile_engine.h"
#include "min_interval_stepper.h"

using namespace std;
using namespace Fannn;

bool terminating = false;
bool needsReload = false;
MinIntervalStepper stepper;

void load() {
    stepper = MinIntervalStepper(2000);//todo: replace with real interval
    //todo
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

        //todo the stuff
        cout << "doing stuff" << endl;//debug

        while(
            !terminating &&
            !needsReload &&
            !stepper.step()
        );
    }

    return 0;
}