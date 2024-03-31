#include <jni.h>
#include <string>
#include <simpleble/SimpleBLE.h>
#include <iostream>

extern "C" JNIEXPORT jstring

JNICALL
Java_org_simpleble_android_1examples_SimpleAndroidBLE_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    bool enabled = SimpleBLE::Adapter::bluetooth_enabled();
    std::string hello = "Hello from C++: " + std::to_string(enabled);
    return env->NewStringUTF(hello.c_str());
}


#include <android/log.h>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    // jvm = vm;
    // log(DebugLevel::INFO, "OnLoad");

    int value = 47;
    __android_log_print(ANDROID_LOG_INFO, "SimpleBLEWrapper", "The answer is: %d", value);

    std::cout << "JNI_OnLoad: Hello Potato!" << std::endl;
    return JNI_VERSION_1_6;
}