#include "lm_sensors_reader.h"
#include <sensors/sensors.h>
#include <sensors/error.h>
#include <iostream>
#include <atomic>

static std::atomic<LmSensorsReader*> instance(nullptr);

LmSensorsReader::LmSensorsReader() {
    LmSensorsReader* np = nullptr;
    if(std::atomic_compare_exchange_strong(&instance, &np, this)){
        sensors_init(nullptr);
    }else{
        throw std::logic_error(
            "not implemented: I have no idea why libsensors"
            "doesn't want us calling sensors_init more than once,"
            "but i don't feel like finding out."
            "Probably we can just ignore this or work around it.");
    };
}

int LmSensorsReader::getValue(std::string sensorId){
    return 666;
}

int LmSensorsReader::debugListEverything() { 
    sensors_chip_name const * cn;
    int c = 0;
    while ((cn = sensors_get_detected_chips(0, &c)) != 0) {
        std::cout << "Chip: " << cn->prefix << "/" << cn->path << std::endl;

        sensors_feature const *feat;
        int f = 0;

        while ((feat = sensors_get_features(cn, &f)) != 0) {
            std::cout << " feature: " << feat->name << " type: " << feat->type << " label: " << sensors_get_label(cn, feat) << std::endl;

            sensors_subfeature const *subf;
            int s = 0;

            while ((subf = sensors_get_all_subfeatures(cn, feat, &s)) != 0) {
                std::cout << "  " << f << ":" << s << ": name:" << subf->name
                        << " number:" << subf->number << " flags:" << subf->flags << " number:" << subf->number << " = ";
                double val;
                if (subf->flags & SENSORS_MODE_R) {
                    int rc = sensors_get_value(cn, subf->number, &val);
                    if (rc < 0) {
                        std::cout << "err: " << rc;
                    } else {
                        std::cout << val;
                    }
                }
                std::cout << std::endl;
            }
        }
    }
    return 666;
}

LmSensorsReader::~LmSensorsReader() {
    sensors_cleanup();
    instance.store(nullptr);
}