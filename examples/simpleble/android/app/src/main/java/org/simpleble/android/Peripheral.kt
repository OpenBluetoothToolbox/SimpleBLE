package org.simpleble.android

class Peripheral {

    private var instanceId: Long = -1
    private var adapterId: Long = -1

    constructor(newAdapterId: Long, newInstanceId: Long) {
        this.adapterId = newAdapterId
        this.instanceId = newInstanceId
    }

}
