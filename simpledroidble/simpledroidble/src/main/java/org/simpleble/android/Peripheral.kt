package org.simpleble.android

import android.util.Log
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch

class Peripheral internal constructor(newAdapterId: Long, newInstanceId: Long) {

    private var instanceId: Long = newInstanceId
    private var adapterId: Long = newAdapterId

    private val _onConnected = MutableSharedFlow<Unit>()
    private val _onDisconnected = MutableSharedFlow<Unit>()

    val identifier: String get() {
        return "Peripheral $instanceId"
    }

    val address: BluetoothAddress get() {
        return BluetoothAddress("")
    }

    val addressType: BluetoothAddressType get() {
        return BluetoothAddressType()
    }

    val rssi: Int get() {
        return 0
    }

    val txPower: Int get() {
        return 0
    }

    val mtu: Int get() {
        return 0
    }

    fun connect() {
        nativePeripheralConnect(adapterId, instanceId)
    }

    fun disconnect() {
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

        // Create a callback that emits the received payload to the StateFlow
        val callback = object : Callback {
            override fun onDataReceived(data: ByteArray) {
                CoroutineScope(Dispatchers.Main).launch {
                    payloadFlow.emit(data)
                }
            }
        }

        // Register the callback
        nativePeripheralNotify(adapterId, instanceId, service.toString(), characteristic.toString(), callback)

        return payloadFlow
    }

    fun indicate(
        service: BluetoothUUID,
        characteristic: BluetoothUUID
    ): StateFlow<ByteArray> {
        val payloadFlow = MutableStateFlow(ByteArray(0))

        // Create a callback that emits the received payload to the StateFlow
        val callback = object : Callback {
            override fun onDataReceived(data: ByteArray) {
                CoroutineScope(Dispatchers.Main).launch {
                    payloadFlow.emit(data)
                }
            }
        }

        // Register the callback with the Bluetooth stack
        // (assuming you have a function to do that)
        //registerBluetoothCallback(service, characteristic, callback)

        return payloadFlow.asStateFlow()
    }

    fun unsubscribe(service: BluetoothUUID, characteristic: BluetoothUUID) {
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

    /// ----------------------------------------------------------------------------

    private external fun nativePeripheralIdentifier(adapterId: Long, instanceId: Long): String?

    private external fun nativePeripheralAddress(adapterId: Long, instanceId: Long): String?

    private external fun nativePeripheralConnect(adapterId: Long, instanceId: Long)

    private external fun nativePeripheralDisconnect(adapterId: Long, instanceId: Long)

    private external fun nativePeripheralNotify(
        adapterId: Long,
        instanceId: Long,
        service: String,
        characteristic: String,
        callback: Callback
    )


    private interface Callback {
        fun onDataReceived(data: ByteArray)
    }

}