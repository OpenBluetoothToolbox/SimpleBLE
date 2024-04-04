#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_org_simpleble_android_SimpleDroidBle_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

#include <android/log.h>
#include <iostream>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    // jvm = vm;
    // log(DebugLevel::INFO, "OnLoad");

    int value = 47;
    __android_log_print(ANDROID_LOG_INFO, "SimpleBLEWrapper", "The answer is: %d", value);

    std::cout << "JNI_OnLoad: Hello Potato!" << std::endl;
    return JNI_VERSION_1_6;
}