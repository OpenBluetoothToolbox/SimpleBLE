package org.simpleble.android

import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.flow.MutableSharedFlow

class Adapter private constructor() {
    private val _onScanStart = MutableSharedFlow<Unit>()
    private val _onScanStop = MutableSharedFlow<Unit>()
    private val _onScanUpdated = MutableSharedFlow<Peripheral>(extraBufferCapacity = 64, onBufferOverflow = BufferOverflow.DROP_OLDEST)
    private val _onScanFound = MutableSharedFlow<Peripheral>(extraBufferCapacity = 64, onBufferOverflow = BufferOverflow.DROP_OLDEST)

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
    }
}
