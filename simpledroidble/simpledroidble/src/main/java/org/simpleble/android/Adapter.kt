package org.simpleble.android

import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.flow.MutableSharedFlow

class Adapter private constructor() {
    private val _onScanStart = MutableSharedFlow<Unit>()
    private val _onScanStop = MutableSharedFlow<Unit>()
    private val _onScanUpdated = MutableSharedFlow<Peripheral>(extraBufferCapacity = 64, onBufferOverflow = BufferOverflow.DROP_OLDEST)
    private val _onScanFound = MutableSharedFlow<Peripheral>(extraBufferCapacity = 64, onBufferOverflow = BufferOverflow.DROP_OLDEST)

//    private var instanceId: Long = -1
//
//    constructor(newInstanceId: Long) {
//        this.instanceId = newInstanceId
//
//    }

    fun identifier(): String {
        return ""
    }

    fun address(): BluetoothAddress {
        return BluetoothAddress()
    }

    fun scanStart() {
    }

    fun scanStop() {
    }

    fun scanFor(timeoutMs: Int) {
    }

    fun scanIsActive(): Boolean {
        return false
    }

    fun scanGetResults(): List<Peripheral> {
        return emptyList()
    }

    val onScanStart get() = _onScanStart

    val onScanStop get() = _onScanStop

    val onScanUpdated get() = _onScanUpdated

    val onScanFound get() = _onScanFound

    fun getPairedPeripherals(): List<Peripheral> {
        return emptyList()
    }

    companion object {
        fun getAdapters(): List<Adapter> {
            return emptyList()
        }

        fun isBluetoothEnabled(): Boolean {
            return false
        }

        // What is JvmStatic?
//        @JvmStatic
//        fun getAdapters(): List<Adapter> {
//            val nativeAdapterIds = nativeGetAdapters()
//            val adapters = ArrayList<Adapter>()
//
//            for (native_adapter_id in nativeAdapterIds) {
//                adapters.add(Adapter(native_adapter_id))
//            }
//
//            return adapters
//        }
//
//        @JvmStatic
//        private external fun nativeGetAdapters(): LongArray
    }

    // ----------------------------------------------------------------------------

//    fun ScanGetResults(): ArrayList<Peripheral> {
//        val nativePeripheralIds = nativeAdapterScanGetResults(instanceId)
//        val peripherals = ArrayList<Peripheral>()
//
//        for (nativePeripheralId in nativePeripheralIds) {
//            peripherals.add(Peripheral(instanceId, nativePeripheralId))
//        }
//
//        return peripherals
//    }

    external fun nativeAdapterIdentifier(adapter_id: Long): String?

    external fun nativeAdapterAddress(adapter_id: Long): String?

    external fun nativeAdapterScanStart(adapter_id: Long)

    external fun nativeAdapterScanStop(adapter_id: Long)

    external fun nativeAdapterScanFor(adapter_id: Long, timeout: Int)

    external fun nativeAdapterScanIsActive(adapter_id: Long): Boolean

    external fun nativeAdapterScanGetResults(adapter_id: Long) : LongArray

    fun incomingOnScanUpdated(adapter_id: Long, peripheral_id: Long) {
        println("incomingOnScanUpdated")
    }

    private fun incomingOnScanFound(adapter_id: Long, peripheral_id: Long) {
        println("incomingOnScanUpdated")
    }
}
