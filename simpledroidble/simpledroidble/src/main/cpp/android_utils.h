#pragma once

#include <string>
#include <jni.h>

void log_error(const std::string& msg);
void log_info(const std::string& msg);
void log_debug(const std::string& msg);

jstring to_jstring(JNIEnv* env, const std::string& str);
std::string from_jstring(JNIEnv* env, jstring str);

jbyteArray to_jbyteArray(JNIEnv* env, const std::string& data);

void throw_exception(JNIEnv* env, const std::string& msg);