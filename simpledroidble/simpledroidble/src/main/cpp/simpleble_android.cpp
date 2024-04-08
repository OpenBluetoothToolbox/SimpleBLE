#include <string>
#include <jni.h>

#include "android_utils.h"
#include <simpleble/SimpleBLE.h>
#include <fmt/core.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include "ThreadRunner.h"

static std::map<size_t, SimpleBLE::Safe::Adapter> cached_adapters;
static std::map<size_t, std::vector<jweak>> cached_adapter_references;

static std::map<size_t, std::map<size_t, SimpleBLE::Safe::Peripheral>> cached_peripherals;
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

    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_Adapter_nativeAdapterRegister(JNIEnv *env, jobject thiz, jlong adapter_id, jobject callback) {
    // Create a weak global reference to the Java callback object
    jweak weakCallbackRef = env->NewWeakGlobalRef(callback);

    // Store the weak reference in the cached_adapter_references map
    cached_adapter_references[adapter_id].push_back(weakCallbackRef);

    log_debug(fmt::format("Registered adapter {} with callback {}", adapter_id, (void*)weakCallbackRef));

    // Retrieve the adapter from the cached_adapters map
    auto adapter = cached_adapters.at(adapter_id);

    // TODO: Remove any invalid objects before adding new ones.

    adapter.set_callback_on_scan_start([adapter_id](){
        threadRunner.enqueue([adapter_id](){
            JNIEnv *env = get_env();

            // Retrieve the weak references from the cached_adapter_references map
            std::vector<jweak> weakCallbackRefs = cached_adapter_references[adapter_id];

            // Iterate over the weak references
            for (jweak weakCallbackRef : weakCallbackRefs) {
                log_debug(fmt::format("Callback for adapter {} is {}", adapter_id, (void*)weakCallbackRef));

                // Check if the weak reference is still valid
                if (env->IsSameObject(weakCallbackRef, nullptr) == JNI_FALSE) {
                    log_debug(fmt::format("Callback for adapter {} is valid", adapter_id));

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

            // Retrieve the weak references from the cached_adapter_references map
            std::vector<jweak> weakCallbackRefs = cached_adapter_references[adapter_id];

            // Iterate over the weak references
            for (jweak weakCallbackRef : weakCallbackRefs) {
                log_debug(fmt::format("Callback for adapter {} is {}", adapter_id, (void*)weakCallbackRef));

                // Check if the weak reference is still valid
                if (env->IsSameObject(weakCallbackRef, nullptr) == JNI_FALSE) {
                    log_debug(fmt::format("Callback for adapter {} is valid", adapter_id));

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

            // Retrieve the weak references from the cached_adapter_references map
            std::vector<jweak> weakCallbackRefs = cached_adapter_references[adapter_id];

            // Iterate over the weak references
            for (jweak weakCallbackRef : weakCallbackRefs) {
                log_debug(fmt::format("Callback for adapter {} is {}", adapter_id,
                                      (void *) weakCallbackRef));

                // Check if the weak reference is still valid
                if (env->IsSameObject(weakCallbackRef, nullptr) == JNI_FALSE) {
                    log_debug(fmt::format("Callback for adapter {} is valid", adapter_id));

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

            // Retrieve the weak references from the cached_adapter_references map
            std::vector<jweak> weakCallbackRefs = cached_adapter_references[adapter_id];

            // Iterate over the weak references
            for (jweak weakCallbackRef : weakCallbackRefs) {
                log_debug(fmt::format("Callback for adapter {} is {}", adapter_id,
                                      (void *) weakCallbackRef));

                // Check if the weak reference is still valid
                if (env->IsSameObject(weakCallbackRef, nullptr) == JNI_FALSE) {
                    log_debug(fmt::format("Callback for adapter {} is valid", adapter_id));

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

    log_info(fmt::format("Found {} adapters. Total cached is {}", adapters.value().size(), cached_adapters.size()));

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
    log_debug(fmt::format("Starting scan for adapter {}", adapter_id));
    auto adapter = cached_adapters.at(adapter_id);
    bool success = adapter.scan_start();

    if (!success) {
        throw_exception(env, "Failed to start scan");
    }
}

extern "C" JNIEXPORT void JNICALL Java_org_simpleble_android_Adapter_nativeAdapterScanStop(JNIEnv *env, jobject thiz, jlong adapter_id) {
    log_debug(fmt::format("Stopping scan for adapter {}", adapter_id));
    auto adapter = cached_adapters.at(adapter_id);
    bool success = adapter.scan_stop();

    if (!success) {
        throw_exception(env, "Failed to stop scan");
    }
}

extern "C" JNIEXPORT void JNICALL Java_org_simpleble_android_Adapter_nativeAdapterScanFor(JNIEnv *env, jobject thiz, jlong adapter_id, jint timeout) {
    log_debug(fmt::format("Scanning for adapter {} with timeout {}", adapter_id, timeout));
    auto adapter = cached_adapters.at(adapter_id);
    bool success = adapter.scan_for(timeout);
    log_debug(fmt::format("Scanning for adapter {} with timeout {} finished", adapter_id, timeout));

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