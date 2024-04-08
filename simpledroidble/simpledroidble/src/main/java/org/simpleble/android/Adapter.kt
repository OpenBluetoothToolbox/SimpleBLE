package org.simpleble.android

import android.util.Log
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.withContext

class Adapter private constructor(newInstanceId: Long) {
    private val _onScanStart = MutableSharedFlow<Unit>()
    private val _onScanStop = MutableSharedFlow<Unit>()
    private val _onScanUpdated = MutableSharedFlow<Peripheral>(extraBufferCapacity = 64, onBufferOverflow = BufferOverflow.DROP_OLDEST)
    private val _onScanFound = MutableSharedFlow<Peripheral>(extraBufferCapacity = 64, onBufferOverflow = BufferOverflow.DROP_OLDEST)

    private var instanceId: Long = newInstanceId
    private val callbacks = object : Callback {
        override fun onScanStart() {
            Log.d("SimpleBLE", "incomingOnScanStart")
            _onScanStart.tryEmit(Unit)
        }

        override fun onScanStop() {
            Log.d("SimpleBLE", "incomingOnScanStop")
            _onScanStop.tryEmit(Unit)
        }

        override fun onScanUpdated(peripheralId: Long) {
            _onScanUpdated.tryEmit(Peripheral(instanceId, peripheralId))
        }

        override fun onScanFound(peripheralId: Long) {
            Log.d("SimpleBLE", "incomingOnScanFound $peripheralId")
            _onScanFound.tryEmit(Peripheral(instanceId, peripheralId))
        }
    }

    init {
        nativeAdapterRegister(instanceId, callbacks)
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

    private external fun nativeAdapterRegister(adapterId: Long, callback: Callback)

    private external fun nativeAdapterIdentifier(adapterId: Long): String?

    private external fun nativeAdapterAddress(adapterId: Long): String?

    private external fun nativeAdapterScanStart(adapterId: Long)

    private external fun nativeAdapterScanStop(adapterId: Long)

    private external fun nativeAdapterScanFor(adapterId: Long, timeout: Int)

    private external fun nativeAdapterScanIsActive(adapterId: Long): Boolean

    private external fun nativeAdapterScanGetResults(adapterId: Long) : LongArray

    // ----------------------------------------------------------------------------

    private interface Callback {
        fun onScanStart()
        fun onScanStop()
        fun onScanUpdated(peripheralId: Long)
        fun onScanFound(peripheralId: Long)
    }

}
