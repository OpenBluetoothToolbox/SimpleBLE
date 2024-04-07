package org.simpleble.android

import android.util.Log
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class Adapter private constructor(newInstanceId: Long) {
    private val _onScanStart = MutableSharedFlow<Unit>()
    private val _onScanStop = MutableSharedFlow<Unit>()
    private val _onScanUpdated = MutableSharedFlow<Peripheral>(extraBufferCapacity = 64, onBufferOverflow = BufferOverflow.DROP_OLDEST)
    private val _onScanFound = MutableSharedFlow<Peripheral>(extraBufferCapacity = 64, onBufferOverflow = BufferOverflow.DROP_OLDEST)

    private var instanceId: Long = newInstanceId

    init {
        Log.d("SimpleBLE","Adapter($instanceId) created")
    }

    val identifier: String get() {
        return nativeAdapterIdentifier(instanceId) ?: ""
    }

    val address: BluetoothAddress get() {
        return BluetoothAddress(nativeAdapterAddress(instanceId) ?: "")
    }

    fun scanStart() {
        nativeAdapterScanStart(instanceId)
    }

    fun scanStop() {
        nativeAdapterScanStop(instanceId)
    }

    suspend fun scanFor(timeoutMs: Int) {
        withContext(Dispatchers.IO) {
            nativeAdapterScanFor(instanceId, timeoutMs)
        }
    }

    val scanIsActive: Boolean get() {
        return nativeAdapterScanIsActive(instanceId)
    }

    fun scanGetResults(): List<Peripheral> {
        return nativeAdapterScanGetResults(instanceId).map { Peripheral(instanceId, it) }
    }

    val onScanStart get() = _onScanStart

    val onScanStop get() = _onScanStop

    val onScanUpdated get() = _onScanUpdated

    val onScanFound get() = _onScanFound

    fun getPairedPeripherals(): List<Peripheral> {
        return emptyList()
    }

    companion object {

        @JvmStatic
        fun isBluetoothEnabled(): Boolean {
            return false
        }

        @JvmStatic
        fun getAdapters(): List<Adapter> {
            val nativeAdapterIds = nativeGetAdapters()
            val adapters = ArrayList<Adapter>()

            for (nativeAdapterId in nativeAdapterIds) {
                Log.d("SimpleBLE", "Adapter found: $nativeAdapterId")
                adapters.add(Adapter(nativeAdapterId))
            }

            return adapters
        }

        @JvmStatic
        private external fun nativeGetAdapters(): LongArray
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

    external fun nativeAdapterIdentifier(adapterId: Long): String?

    external fun nativeAdapterAddress(adapterId: Long): String?

    external fun nativeAdapterScanStart(adapterId: Long)

    external fun nativeAdapterScanStop(adapterId: Long)

    external fun nativeAdapterScanFor(adapterId: Long, timeout: Int)

    external fun nativeAdapterScanIsActive(adapterId: Long): Boolean

    external fun nativeAdapterScanGetResults(adapterId: Long) : LongArray

    fun incomingOnScanUpdated(adapterId: Long, peripheralId: Long) {
        println("incomingOnScanUpdated")
    }

    private fun incomingOnScanFound(adapterId: Long, peripheralId: Long) {
        println("incomingOnScanUpdated")
    }
}
