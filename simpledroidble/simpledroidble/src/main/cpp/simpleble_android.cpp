#include <string>
#include <jni.h>

#include "android_utils.h"
#include <simpleble/SimpleBLE.h>
#include <fmt/core.h>
#include <vector>
#include <map>

static std::map<size_t, SimpleBLE::Safe::Adapter> cached_adapters;
static std::map<size_t, std::map<size_t, SimpleBLE::Safe::Peripheral>> cached_peripherals;


// https://docs.oracle.com/javase/9/docs/specs/jni/functions.html
// https://developer.android.com/training/articles/perf-jni
// https://www.developer.com/java/data/manipulating-java-objects-in-native-code/
//         jclass Exception = env->FindClass("java/lang/Exception");
//        env->ThrowNew(Exception,"Can't divide by zero.");

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

//    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
//    jclass c = env->FindClass("com/example/app/package/MyClass");
//    if (c == nullptr) return JNI_ERR;

    return JNI_VERSION_1_6;
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
    auto adapter = cached_adapters.at(adapter_id);
    bool success = adapter.scan_start();

    if (!success) {
        // Throw an exception
        jclass Exception = env->FindClass("java/lang/Exception");
        env->ThrowNew(Exception, "Failed to start scan");
    }
}

extern "C" JNIEXPORT void JNICALL Java_org_simpleble_android_Adapter_nativeAdapterScanStop(JNIEnv *env, jobject thiz, jlong adapter_id) {
    auto adapter = cached_adapters.at(adapter_id);
    bool success = adapter.scan_stop();

    if (!success) {
        // Throw an exception
        jclass Exception = env->FindClass("java/lang/Exception");
        env->ThrowNew(Exception, "Failed to stop scan");
    }
}

extern "C" JNIEXPORT void JNICALL Java_org_simpleble_android_Adapter_nativeAdapterScanFor(JNIEnv *env, jobject thiz, jlong adapter_id, jint timeout) {
    auto adapter = cached_adapters.at(adapter_id);
    bool success = adapter.scan_for(timeout);

    if (!success) {
        // Throw an exception
        jclass Exception = env->FindClass("java/lang/Exception");
        env->ThrowNew(Exception, "Failed to scan");
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