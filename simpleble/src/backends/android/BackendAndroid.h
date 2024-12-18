#pragma once

#include "../common/BackendBase.h"
#include "../common/BackendUtils.h"

#include "jni/Common.hpp"

namespace SimpleBLE {

class BackendAndroid : public BackendSingleton<BackendAndroid> {
  public:
    BackendAndroid(buildToken);
    virtual ~BackendAndroid() = default;

    virtual std::vector<std::shared_ptr<AdapterBase>> get_adapters() override;
    virtual bool bluetooth_enabled() override;
    virtual std::string name() const noexcept override;

    JNI::Object& get_btAdapter() { return _btAdapter; }
    JNI::Object& get_btScanner() { return _btScanner; }

  private:
    // NOTE: The correct way to request a BluetoothAdapter is to go though the BluetoothManager,
    // as described in https://developer.android.com/reference/android/bluetooth/BluetoothManager#getAdapter()
    // However, for simplicity, we are using a direct call to BluetoothAdapter.getDefaultAdapter() which is
    // deprecated in API 31 but still works. We'll need to implement a backend bypass to get a Context
    // object and call getSystemService(Context.BLUETOOTH_SERVICE) to get the BluetoothManager.

    // NOTE: Android BluetoothAdapter and BluetoothScanner classes are singletons, but so is this
    // class, so we don't need to make them static.
    JNI::Class _btAdapterCls;
    JNI::Class _btScanCallbackCls;
    JNI::Class _btScanResultCls;
    JNI::Object _btAdapter;
    JNI::Object _btScanner;
};

}  // namespace SimpleBLE
