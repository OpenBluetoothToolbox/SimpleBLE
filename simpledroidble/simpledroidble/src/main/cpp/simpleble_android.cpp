#include <string>
#include <jni.h>

#include "android_utils.h"
#include <simpleble/SimpleBLE.h>
#include <simpleble/Logging.h>
#include <fmt/core.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include "ThreadRunner.h"

static std::map<size_t, SimpleBLE::Safe::Adapter> cached_adapters;
static std::map<size_t, std::vector<jweak>> cached_adapter_callbacks;

static std::map<size_t, std::map<size_t, SimpleBLE::Safe::Peripheral>> cached_peripherals;
static std::map<size_t, std::map<size_t, std::vector<jweak>>> cached_peripheral_callbacks;
static std::map<size_t, std::map<size_t, std::map<size_t, jobject>>> cached_peripheral_data_callbacks;
static ThreadRunner threadRunner;
static JavaVM *jvm;

JNIEnv* get_env() {
    JNIEnv *env;
    jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
    return env;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;
    threadRunner.set_jvm(vm);

//    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
//    jclass c = env->FindClass("com/example/app/package/MyClass");
//    if (c == nullptr) return JNI_ERR;

    SimpleBLE::Logging::Logger::get()->set_callback(
        [](SimpleBLE::Logging::Level level, const std::string& module, const std::string& file, uint32_t line, const std::string& function, const std::string& message) {
            std::string log_message = fmt::format("[{}] {}:{}:{}: {}", module, file, line, function, message);
            log_info(log_message);
        }
    );

    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_Adapter_nativeAdapterRegister(JNIEnv *env, jobject thiz, jlong adapter_id, jobject callback) {
    // TODO: IDEA. We could store the callback object whenever the scan starts and then remove it when the scan stops,
    //             to avoid having extra references lying around.

    // Create a weak global reference to the Java callback object
    jweak weakCallbackRef = env->NewWeakGlobalRef(callback);

    // Store the weak reference in the cached_adapter_callbacks map
    cached_adapter_callbacks[adapter_id].push_back(weakCallbackRef);

    // Retrieve the adapter from the cached_adapters map
    auto adapter = cached_adapters.at(adapter_id);

    // TODO: Remove any invalid objects before adding new ones.

    adapter.set_callback_on_scan_start([adapter_id](){
        threadRunner.enqueue([adapter_id](){
            JNIEnv *env = get_env();

            // Retrieve the weak references from the cached_adapter_callbacks map
            std::vector<jweak> weakCallbackRefs = cached_adapter_callbacks[adapter_id];

            // Iterate over the weak references
            for (jweak weakCallbackRef : weakCallbackRefs) {

                // Check if the weak reference is still valid
                if (env->IsSameObject(weakCallbackRef, nullptr) == JNI_FALSE) {
                    // Retrieve the strong reference from the weak reference
                    jobject callbackRef = env->NewLocalRef(weakCallbackRef);

                    // Find the Java class and method to invoke
                    // TODO: We should cache the class and method IDs
                    jclass callbackClass = env->GetObjectClass(callbackRef);
                    jmethodID onScanStartMethod = env->GetMethodID(callbackClass, "onScanStart", "()V");

                    // Invoke the Java callback method
                    env->CallVoidMethod(callbackRef, onScanStartMethod);

                    // Delete the local reference
                    env->DeleteLocalRef(callbackRef);
                }
            }
        });
    });

    adapter.set_callback_on_scan_stop([adapter_id](){
        threadRunner.enqueue([adapter_id](){
            JNIEnv *env = get_env();

            // Retrieve the weak references from the cached_adapter_callbacks map
            std::vector<jweak> weakCallbackRefs = cached_adapter_callbacks[adapter_id];

            // Iterate over the weak references
            for (jweak weakCallbackRef : weakCallbackRefs) {
                // Check if the weak reference is still valid
                if (env->IsSameObject(weakCallbackRef, nullptr) == JNI_FALSE) {
                    // Retrieve the strong reference from the weak reference
                    jobject callbackRef = env->NewLocalRef(weakCallbackRef);

                    // Find the Java class and method to invoke
                    // TODO: We should cache the class and method IDs
                    jclass callbackClass = env->GetObjectClass(callbackRef);
                    jmethodID onScanStopMethod = env->GetMethodID(callbackClass, "onScanStop", "()V");

                    // Invoke the Java callback method
                    env->CallVoidMethod(callbackRef, onScanStopMethod);

                    // Delete the local reference
                    env->DeleteLocalRef(callbackRef);
                }
            }
        });
    });

    adapter.set_callback_on_scan_found([adapter_id](SimpleBLE::Safe::Peripheral peripheral){
        size_t peripheral_hash = std::hash<std::string>{}(peripheral.identifier().value_or("UNKNOWN"));

        // Add to the cache if it doesn't exist
        if (cached_peripherals[adapter_id].count(peripheral_hash) == 0) {
            cached_peripherals[adapter_id].insert({peripheral_hash, peripheral});
        }

        threadRunner.enqueue([adapter_id, peripheral_hash](){
            JNIEnv *env = get_env();

            // Retrieve the weak references from the cached_adapter_callbacks map
            std::vector<jweak> weakCallbackRefs = cached_adapter_callbacks[adapter_id];

            // Iterate over the weak references
            for (jweak weakCallbackRef : weakCallbackRefs) {
                // Check if the weak reference is still valid
                if (env->IsSameObject(weakCallbackRef, nullptr) == JNI_FALSE) {

                    // Retrieve the strong reference from the weak reference
                    jobject callbackRef = env->NewLocalRef(weakCallbackRef);

                    // Find the Java class and method to invoke
                    // TODO: We should cache the class and method IDs
                    jclass callbackClass = env->GetObjectClass(callbackRef);
                    jmethodID onScanFoundMethod = env->GetMethodID(callbackClass, "onScanFound",
                                                                   "(J)V");

                    // Invoke the Java callback method
                    env->CallVoidMethod(callbackRef, onScanFoundMethod, peripheral_hash);

                    // Delete the local reference
                    env->DeleteLocalRef(callbackRef);

                }
            }
        });
    });

    adapter.set_callback_on_scan_updated([adapter_id](SimpleBLE::Safe::Peripheral peripheral){
        size_t peripheral_hash = std::hash<std::string>{}(peripheral.identifier().value_or("UNKNOWN"));

        // Add to the cache if it doesn't exist
        if (cached_peripherals[adapter_id].count(peripheral_hash) == 0) {
            cached_peripherals[adapter_id].insert({peripheral_hash, peripheral});
        }

        threadRunner.enqueue([adapter_id, peripheral_hash](){
            JNIEnv *env = get_env();

            // Retrieve the weak references from the cached_adapter_callbacks map
            std::vector<jweak> weakCallbackRefs = cached_adapter_callbacks[adapter_id];

            // Iterate over the weak references
            for (jweak weakCallbackRef : weakCallbackRefs) {
                // Check if the weak reference is still valid
                if (env->IsSameObject(weakCallbackRef, nullptr) == JNI_FALSE) {

                    // Retrieve the strong reference from the weak reference
                    jobject callbackRef = env->NewLocalRef(weakCallbackRef);

                    // Find the Java class and method to invoke
                    // TODO: We should cache the class and method IDs
                    jclass callbackClass = env->GetObjectClass(callbackRef);
                    jmethodID onScanFoundMethod = env->GetMethodID(callbackClass, "onScanUpdated",
                                                                   "(J)V");

                    // Invoke the Java callback method
                    env->CallVoidMethod(callbackRef, onScanFoundMethod, peripheral_hash);

                    // Delete the local reference
                    env->DeleteLocalRef(callbackRef);

                }
            }
        });
    });
}

extern "C" JNIEXPORT jlongArray JNICALL Java_org_simpleble_android_Adapter_nativeGetAdapters(JNIEnv *env, jclass clazz) {
    auto adapters = SimpleBLE::Safe::Adapter::get_adapters();

    // If an error occurred, return an empty list.
    if (!adapters.has_value()) return env->NewLongArray(0);

    // Go over the results, cache whatever doesn't exist and return the full list.
    jsize j_adapter_index = 0;
    jlongArray j_adapter_result = env->NewLongArray(static_cast<int>(adapters.value().size()));
    for (auto &adapter: adapters.value()) {
        size_t adapter_hash = std::hash<std::string>{}(adapter.identifier().value_or("UNKNOWN"));

        // Add to the cache if it doesn't exist
        if (cached_adapters.count(adapter_hash) == 0) {
            cached_adapters.insert({adapter_hash, adapter});
        }

        // Add to the results
        jlong j_adapter_hash = adapter_hash;
        env->SetLongArrayRegion(j_adapter_result, j_adapter_index, 1, &j_adapter_hash);
        j_adapter_index++;

    }

    return j_adapter_result;
}

extern "C" JNIEXPORT jstring JNICALL Java_org_simpleble_android_Adapter_nativeAdapterIdentifier(JNIEnv *env, jobject thiz, jlong adapter_id) {
    auto adapter = cached_adapters.at(adapter_id);
    // TODO: Should throw exception in case of failure.
    return to_jstring(env, adapter.identifier().value_or("Unknown"));
}

extern "C" JNIEXPORT jstring JNICALL Java_org_simpleble_android_Adapter_nativeAdapterAddress(JNIEnv *env, jobject thiz, jlong adapter_id) {
    auto adapter = cached_adapters.at(adapter_id);
    // TODO: Should throw exception in case of failure.
    return to_jstring(env, adapter.address().value_or("Unknown"));
}

extern "C" JNIEXPORT void JNICALL Java_org_simpleble_android_Adapter_nativeAdapterScanStart(JNIEnv *env, jobject thiz, jlong adapter_id) {
    auto adapter = cached_adapters.at(adapter_id);
    bool success = adapter.scan_start();

    if (!success) {
        throw_exception(env, "Failed to start scan");
    }
}

extern "C" JNIEXPORT void JNICALL Java_org_simpleble_android_Adapter_nativeAdapterScanStop(JNIEnv *env, jobject thiz, jlong adapter_id) {
    auto adapter = cached_adapters.at(adapter_id);
    bool success = adapter.scan_stop();

    if (!success) {
        throw_exception(env, "Failed to stop scan");
    }
}

extern "C" JNIEXPORT void JNICALL Java_org_simpleble_android_Adapter_nativeAdapterScanFor(JNIEnv *env, jobject thiz, jlong adapter_id, jint timeout) {
    auto adapter = cached_adapters.at(adapter_id);
    bool success = adapter.scan_for(timeout);

    if (!success) {
        throw_exception(env, "Failed to scan for");
    }
}

extern "C" JNIEXPORT jboolean JNICALL Java_org_simpleble_android_Adapter_nativeAdapterScanIsActive(JNIEnv *env, jobject thiz, jlong adapter_id) {
    auto adapter = cached_adapters.at(adapter_id);
    // TODO: Should throw exception in case of failure.
    return adapter.scan_is_active().value_or(false);
}

extern "C" JNIEXPORT jlongArray JNICALL Java_org_simpleble_android_Adapter_nativeAdapterScanGetResults(JNIEnv *env, jobject thiz, jlong adapter_id) {
    auto adapter = cached_adapters.at(adapter_id);

    auto peripherals = adapter.scan_get_results();

    // If an error occurred, return an empty list.
    if (!peripherals.has_value()) return env->NewLongArray(0);

    jsize j_peripheral_index = 0;
    jlongArray j_peripheral_result = env->NewLongArray(static_cast<int>(peripherals.value().size()));
    for (auto &peripheral: peripherals.value()) {
        size_t peripheral_hash = std::hash<std::string>{}(peripheral.identifier().value_or("UNKNOWN"));

        // Add to the cache if it doesn't exist
        if (cached_peripherals[adapter_id].count(peripheral_hash) == 0) {
            cached_peripherals[adapter_id].insert({peripheral_hash, peripheral});
        }

        // Add to the results
        jlong j_peripheral_hash = peripheral_hash;
        env->SetLongArrayRegion(j_peripheral_result, j_peripheral_index, 1, &j_peripheral_hash);
        j_peripheral_index++;
    }

    return j_peripheral_result;
}

// PERIPHERAL

extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralRegister(JNIEnv *env, jobject thiz,
                                                               jlong adapter_id, jlong peripheral_id, jobject callback) {
// TODO: IDEA. We could store the callback object whenever the scan starts and then remove it when the scan stops,
    //             to avoid having extra references lying around.

    // Create a weak global reference to the Java callback object
    jweak weakCallbackRef = env->NewWeakGlobalRef(callback);

    // Store the weak reference in the cached_adapter_callbacks map
    cached_peripheral_callbacks[adapter_id][peripheral_id].push_back(weakCallbackRef);

    auto peripheral = cached_peripherals[adapter_id].at(peripheral_id);

    // TODO: Remove any invalid objects before adding new ones.

    peripheral.set_callback_on_connected([adapter_id, peripheral_id](){
        threadRunner.enqueue([adapter_id, peripheral_id](){
            JNIEnv *env = get_env();

            // Retrieve the weak references from the cached_adapter_callbacks map
            std::vector<jweak> weakCallbackRefs = cached_peripheral_callbacks[adapter_id][peripheral_id];

            // Iterate over the weak references
            for (jweak weakCallbackRef : weakCallbackRefs) {

                // Check if the weak reference is still valid
                if (env->IsSameObject(weakCallbackRef, nullptr) == JNI_FALSE) {
                    // Retrieve the strong reference from the weak reference
                    jobject callbackRef = env->NewLocalRef(weakCallbackRef);

                    // Find the Java class and method to invoke
                    // TODO: We should cache the class and method IDs
                    jclass callbackClass = env->GetObjectClass(callbackRef);
                    jmethodID onConnectedMethod = env->GetMethodID(callbackClass, "onConnected", "()V");

                    // Invoke the Java callback method
                    env->CallVoidMethod(callbackRef, onConnectedMethod);

                    // Delete the local reference
                    env->DeleteLocalRef(callbackRef);
                }
            }
        });
    });

    peripheral.set_callback_on_disconnected([adapter_id, peripheral_id](){
        threadRunner.enqueue([adapter_id, peripheral_id](){
            JNIEnv *env = get_env();

            // Retrieve the weak references from the cached_adapter_callbacks map
            std::vector<jweak> weakCallbackRefs = cached_peripheral_callbacks[adapter_id][peripheral_id];

            // Iterate over the weak references
            for (jweak weakCallbackRef : weakCallbackRefs) {

                // Check if the weak reference is still valid
                if (env->IsSameObject(weakCallbackRef, nullptr) == JNI_FALSE) {
                    // Retrieve the strong reference from the weak reference
                    jobject callbackRef = env->NewLocalRef(weakCallbackRef);

                    // Find the Java class and method to invoke
                    // TODO: We should cache the class and method IDs
                    jclass callbackClass = env->GetObjectClass(callbackRef);
                    jmethodID onConnectedMethod = env->GetMethodID(callbackClass, "onDisconnected", "()V");

                    // Invoke the Java callback method
                    env->CallVoidMethod(callbackRef, onConnectedMethod);

                    // Delete the local reference
                    env->DeleteLocalRef(callbackRef);
                }
            }
        });
    });
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralIdentifier(JNIEnv *env, jobject thiz,
                                                                 jlong adapter_id,
                                                                 jlong peripheral_id) {
    auto& peripheral = cached_peripherals[adapter_id].at(peripheral_id);
    return to_jstring(env, peripheral.identifier().value_or("Unknown"));
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralAddress(JNIEnv *env, jobject thiz,
                                                              jlong adapter_id, jlong peripheral_id) {
    auto& peripheral = cached_peripherals[adapter_id].at(peripheral_id);
    return to_jstring(env, peripheral.address().value_or("Unknown"));
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralAddressType(JNIEnv *env, jobject thiz,
                                                                  jlong adapter_id,
                                                                  jlong peripheral_id) {
    auto& peripheral = cached_peripherals[adapter_id].at(peripheral_id);
    return peripheral.address_type().value_or(SimpleBLE::BluetoothAddressType::UNSPECIFIED);
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralRssi(JNIEnv *env, jobject thiz,
                                                           jlong adapter_id, jlong peripheral_id) {
    auto& peripheral = cached_peripherals[adapter_id].at(peripheral_id);
    return peripheral.rssi().value_or(INT16_MIN);
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralTxPower(JNIEnv *env, jobject thiz,
                                                              jlong adapter_id, jlong peripheral_id) {
    auto& peripheral = cached_peripherals[adapter_id].at(peripheral_id);
    return peripheral.tx_power().value_or(INT16_MIN);
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralMtu(JNIEnv *env, jobject thiz,
                                                          jlong adapter_id, jlong peripheral_id) {
    auto& peripheral = cached_peripherals[adapter_id].at(peripheral_id);
    return peripheral.mtu().value_or(UINT16_MAX);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralConnect(JNIEnv *env, jobject thiz,
                                                              jlong adapter_id, jlong peripheral_id) {
    auto peripheral = cached_peripherals[adapter_id].at(peripheral_id);

    bool success = peripheral.connect();
    if (!success) {
        throw_exception(env, "Failed to connect");
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralDisconnect(JNIEnv *env, jobject thiz,
                                                                 jlong adapter_id,
                                                                 jlong peripheral_id) {
    auto& peripheral = cached_peripherals[adapter_id].at(peripheral_id);
    peripheral.disconnect();
}

extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralNotify(JNIEnv *env, jobject thiz,
                                                             jlong adapter_id, jlong peripheral_id,
                                                             jstring j_service,
                                                             jstring j_characteristic,
                                                             jobject callback) {

    std::string service = from_jstring(env, j_service);
    std::string characteristic = from_jstring(env, j_characteristic);
    std::string service_characteristic = service + "_" + characteristic;
    size_t service_characteristic_hash = std::hash<std::string>{}(service_characteristic);

    jobject callbackRef = env->NewGlobalRef(callback);
    // TODO: Check if there is a callback already registered for this service_characteristic_hash
    cached_peripheral_data_callbacks[adapter_id][peripheral_id].insert({service_characteristic_hash, callbackRef});

    auto peripheral = cached_peripherals[adapter_id].at(peripheral_id);
    bool success = peripheral.notify(service, characteristic, [adapter_id, peripheral_id, service_characteristic_hash](SimpleBLE::ByteArray payload){

        std::string payload_contents;
        for (int i = 0; i < payload.size(); i++) {
            payload_contents += fmt::format("{:02X}", (int)(payload[i]));
        }

        log_info("Received payload: " + payload_contents);

        threadRunner.enqueue([adapter_id, peripheral_id, service_characteristic_hash, payload]() {
            JNIEnv *env = get_env();

            // Retrieve the weak references from the cached_adapter_callbacks map
            jobject callbackRef = cached_peripheral_data_callbacks[adapter_id][peripheral_id].at(service_characteristic_hash);
            jbyteArray j_payload = to_jbyteArray(env, payload);

            // TODO: We should cache the class and method IDs
            jclass callbackClass = env->GetObjectClass(callbackRef);
            jmethodID onDataReceivedMethod = env->GetMethodID(callbackClass, "onDataReceived", "([B)V");

            // Invoke the Java callback method
            env->CallVoidMethod(callbackRef, onDataReceivedMethod, j_payload);

        });
    });

    if (!success) {
        throw_exception(env, "Failed to notify");
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralIndicate(JNIEnv *env, jobject thiz,
                                                               jlong adapter_id, jlong peripheral_id,
                                                               jstring j_service,
                                                               jstring j_characteristic,
                                                               jobject callback) {

    std::string service = from_jstring(env, j_service);
    std::string characteristic = from_jstring(env, j_characteristic);
    std::string service_characteristic = service + "_" + characteristic;
    size_t service_characteristic_hash = std::hash<std::string>{}(service_characteristic);

    jobject callbackRef = env->NewGlobalRef(callback);
    // TODO: Check if there is a callback already registered for this service_characteristic_hash
    cached_peripheral_data_callbacks[adapter_id][peripheral_id].insert({service_characteristic_hash, callbackRef});

    auto peripheral = cached_peripherals[adapter_id].at(peripheral_id);
    bool success = peripheral.indicate(service, characteristic, [adapter_id, peripheral_id, service_characteristic_hash](SimpleBLE::ByteArray payload){

        std::string payload_contents;
        for (int i = 0; i < payload.size(); i++) {
            payload_contents += fmt::format("{:02X}", (int)(payload[i]));
        }

        log_info("Received payload: " + payload_contents);

        threadRunner.enqueue([adapter_id, peripheral_id, service_characteristic_hash, payload]() {
            JNIEnv *env = get_env();

            // Retrieve the weak references from the cached_adapter_callbacks map
            jobject callbackRef = cached_peripheral_data_callbacks[adapter_id][peripheral_id].at(service_characteristic_hash);
            jbyteArray j_payload = to_jbyteArray(env, payload);

            // TODO: We should cache the class and method IDs
            jclass callbackClass = env->GetObjectClass(callbackRef);
            jmethodID onDataReceivedMethod = env->GetMethodID(callbackClass, "onDataReceived", "([B)V");

            // Invoke the Java callback method
            env->CallVoidMethod(callbackRef, onDataReceivedMethod, j_payload);

        });
    });

    if (!success) {
        throw_exception(env, "Failed to notify");
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_Peripheral_nativePeripheralUnsubscribe(JNIEnv *env, jobject thiz,
                                                                  jlong adapter_id,
                                                                  jlong peripheral_id,
                                                                  jstring j_service,
                                                                  jstring j_characteristic) {
    std::string service = from_jstring(env, j_service);
    std::string characteristic = from_jstring(env, j_characteristic);
    std::string service_characteristic = service + "_" + characteristic;
    size_t service_characteristic_hash = std::hash<std::string>{}(service_characteristic);

    auto peripheral = cached_peripherals[adapter_id].at(peripheral_id);
    bool success = peripheral.unsubscribe(service, characteristic);

    if (!success) {
        throw_exception(env, "Failed to unsubscribe");
    }

    jobject callbackRef = cached_peripheral_data_callbacks[adapter_id][peripheral_id].at(service_characteristic_hash);

    // TODO: Should some check be done here to see if the callbackRef is still valid?
    env->DeleteGlobalRef(callbackRef);
    cached_peripheral_data_callbacks[adapter_id][peripheral_id].erase(service_characteristic_hash);
}
