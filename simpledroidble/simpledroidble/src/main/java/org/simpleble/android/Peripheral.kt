package org.simpleble.android

import android.util.Log
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class Peripheral internal constructor(newAdapterId: Long, newInstanceId: Long) {

    private var instanceId: Long = newInstanceId
    private var adapterId: Long = newAdapterId

    private val _onConnected = MutableSharedFlow<Unit>()
    private val _onConnectionActive = MutableSharedFlow<Boolean>()
    private val _onDisconnected = MutableSharedFlow<Unit>()

    private val callbacks = object : Callback {
        override fun onConnected() {
            CoroutineScope(Dispatchers.Main).launch {
                _onConnected.emit(Unit)
            }
            CoroutineScope(Dispatchers.Main).launch {
                _onConnectionActive.emit(true)
            }
        }

        override fun onDisconnected() {
            CoroutineScope(Dispatchers.Main).launch {
                _onDisconnected.emit(Unit)
            }
            CoroutineScope(Dispatchers.Main).launch {
                _onConnectionActive.emit(false)
            }
        }
    }

    init {
        Log.d("SimpleBLE", "Peripheral ${this.hashCode()}.init")
        nativePeripheralRegister(adapterId, instanceId, callbacks)
    }

    val identifier: String get() {
        return nativePeripheralIdentifier(adapterId, instanceId) ?: ""
    }

    val address: BluetoothAddress get() {
        return BluetoothAddress(nativePeripheralAddress(adapterId, instanceId) ?: "")
    }

    val addressType: BluetoothAddressType get() {
        return BluetoothAddressType.fromInt(nativePeripheralAddressType(adapterId, instanceId))
    }

    val rssi: Int get() {
        return nativePeripheralRssi(adapterId, instanceId)
    }

    val txPower: Int get() {
        return nativePeripheralTxPower(adapterId, instanceId)
    }

    val mtu: Int get() {
        return nativePeripheralMtu(adapterId, instanceId)
    }

    suspend fun connect() {
        withContext(Dispatchers.IO) {
            nativePeripheralConnect(adapterId, instanceId)
        }
    }

    suspend fun disconnect() {
        withContext(Dispatchers.IO) {
            nativePeripheralDisconnect(adapterId, instanceId)
        }
    }

    val isConnected: Boolean get() {
        return false
    }

    val isConnectable: Boolean get() {
        return false
    }

    val isPaired: Boolean get() {
        return false
    }

    fun unpair() {
    }

    fun services(): List<Service> {
        return emptyList()
    }

    fun manufacturer_data(): Map<Int, ByteArray> {
        return emptyMap()
    }

    fun read(service: BluetoothUUID, characteristic: BluetoothUUID): ByteArray {
        return ByteArray(0)
    }

    fun write_request(service: BluetoothUUID, characteristic: BluetoothUUID, data: ByteArray) {
    }

    fun write_command(service: BluetoothUUID, characteristic: BluetoothUUID, data: ByteArray) {
    }

    fun notify(
        service: BluetoothUUID,
        characteristic: BluetoothUUID
    ): MutableSharedFlow<ByteArray> {
        val payloadFlow = MutableSharedFlow<ByteArray>()
        val dataCallback = object : DataCallback {
            override fun onDataReceived(data: ByteArray) {
                CoroutineScope(Dispatchers.Main).launch {
                    payloadFlow.emit(data)
                }
            }
        }

        nativePeripheralNotify(adapterId, instanceId, service.toString(), characteristic.toString(), dataCallback)
        return payloadFlow
    }

    fun indicate(
        service: BluetoothUUID,
        characteristic: BluetoothUUID
    ): MutableSharedFlow<ByteArray> {
        val payloadFlow = MutableSharedFlow<ByteArray>()
        val dataCallback = object : DataCallback {
            override fun onDataReceived(data: ByteArray) {
                CoroutineScope(Dispatchers.Main).launch {
                    payloadFlow.emit(data)
                }
            }
        }

        nativePeripheralIndicate(adapterId, instanceId, service.toString(), characteristic.toString(), dataCallback)
        return payloadFlow
    }

    fun unsubscribe(service: BluetoothUUID, characteristic: BluetoothUUID) {
        nativePeripheralUnsubscribe(adapterId, instanceId, service.toString(), characteristic.toString())
    }

    fun read(
        service: BluetoothUUID,
        characteristic: BluetoothUUID,
        descriptor: BluetoothUUID
    ): ByteArray {
        return ByteArray(0)
    }

    fun write(
        service: BluetoothUUID,
        characteristic: BluetoothUUID,
        descriptor: BluetoothUUID,
        data: ByteArray
    ) {
    }

    val onConnected get() = _onConnected

    val onDisconnected get() = _onDisconnected

    val onConnectionActive get() = _onConnectionActive

    /// ----------------------------------------------------------------------------

    private external fun nativePeripheralRegister(adapterId: Long, instanceId: Long, callback: Callback)

    private external fun nativePeripheralIdentifier(adapterId: Long, instanceId: Long): String?

    private external fun nativePeripheralAddress(adapterId: Long, instanceId: Long): String?

    private external fun nativePeripheralAddressType(adapterId: Long, instanceId: Long): Int

    private external fun nativePeripheralRssi(adapterId: Long, instanceId: Long): Int

    private external fun nativePeripheralTxPower(adapterId: Long, instanceId: Long): Int

    private external fun nativePeripheralMtu(adapterId: Long, instanceId: Long): Int

    private external fun nativePeripheralConnect(adapterId: Long, instanceId: Long)

    private external fun nativePeripheralDisconnect(adapterId: Long, instanceId: Long)

    private external fun nativePeripheralNotify(
        adapterId: Long,
        instanceId: Long,
        service: String,
        characteristic: String,
        dataCallback: DataCallback
    )

    private external fun nativePeripheralIndicate(
        adapterId: Long,
        instanceId: Long,
        service: String,
        characteristic: String,
        dataCallback: DataCallback
    )

    private external fun nativePeripheralUnsubscribe(
        adapterId: Long,
        instanceId: Long,
        service: String,
        characteristic: String)

    // ----------------------------------------------------------------------------

    private interface DataCallback {
        fun onDataReceived(data: ByteArray)
    }


    private interface Callback {
        fun onConnected()
        fun onDisconnected()
    }

}