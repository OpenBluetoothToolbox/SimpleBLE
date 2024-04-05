#pragma once

#include <string>
#include <jni.h>

void log_error(const std::string& msg);
void log_info(const std::string& msg);
void log_debug(const std::string& msg);

jstring to_jstring(JNIEnv* env, const std::string& str);
