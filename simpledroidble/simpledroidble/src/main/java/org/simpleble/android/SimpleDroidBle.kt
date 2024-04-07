package org.simpleble.android

class SimpleDroidBle {
    companion object {
        init {
            System.loadLibrary("simpleble-jni")
        }

        @JvmStatic
        fun getVersion(): String {
            return "1.0.0"
        }
    }
}