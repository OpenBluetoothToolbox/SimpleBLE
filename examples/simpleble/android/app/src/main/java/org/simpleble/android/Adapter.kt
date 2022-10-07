package org.simpleble.android


class Adapter {

    private var instanceId: Long = -1

    constructor(newInstanceId: Long) {
        this.instanceId = newInstanceId

    }

    fun ScanGetResults(): ArrayList<Peripheral> {
        val nativePeripheralIds = nativeAdapterScanGetResults(instanceId)
        val peripherals = ArrayList<Peripheral>()

        for (nativePeripheralId in nativePeripheralIds) {
            peripherals.add(Peripheral(instanceId, nativePeripheralId))
        }

        return peripherals
    }

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

    companion object {
        init {
            System.loadLibrary("simpleble_android")
        }

        @JvmStatic
        fun getAdapters(): List<Adapter> {
            val nativeAdapterIds = nativeGetAdapters()
            val adapters = ArrayList<Adapter>()

            for (native_adapter_id in nativeAdapterIds) {
                adapters.add(Adapter(native_adapter_id))
            }

            return adapters
        }

        @JvmStatic
        private external fun nativeGetAdapters(): LongArray
    }
}