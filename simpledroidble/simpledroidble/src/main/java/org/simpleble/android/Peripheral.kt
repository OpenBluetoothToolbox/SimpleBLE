package org.simpleble.android

import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

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
    ): StateFlow<ByteArray> {
        val payloadFlow = MutableStateFlow(ByteArray(0))

        // Create a callback that emits the received payload to the StateFlow
        val callback = { payload: ByteArray ->
            payloadFlow.value = payload
        }

        // Register the callback with the Bluetooth stack
        // (assuming you have a function to do that)
        //registerBluetoothCallback(service, characteristic, callback)

        return payloadFlow.asStateFlow()
    }

    fun indicate(
        service: BluetoothUUID,
        characteristic: BluetoothUUID
    ): StateFlow<ByteArray> {
        val payloadFlow = MutableStateFlow(ByteArray(0))

        // Create a callback that emits the received payload to the StateFlow
        val callback = { payload: ByteArray ->
            payloadFlow.value = payload
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






}