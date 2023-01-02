use std::mem;
use std::pin::Pin;

#[cxx::bridge]
mod ffi {

    #[namespace = "SimpleBLE"]
    struct RustyAdapterWrapper {
        internal: UniquePtr<RustyAdapter>,
    }

    #[namespace = "SimpleBLE"]
    struct RustyPeripheralWrapper {
        internal: UniquePtr<RustyPeripheral>,
    }

    #[namespace = "SimpleRsBLE"]
    extern "Rust" {
        type Adapter;

        fn on_callback_scan_start(self: &mut Adapter);
    }

    unsafe extern "C++" {
        include!("src/bindings/AdapterBindings.hpp");

        #[namespace = "SimpleBLE"]
        type RustyAdapter;

        fn RustyAdapter_bluetooth_enabled() -> bool;
        fn RustyAdapter_get_adapters() -> Vec<RustyAdapterWrapper>;

        fn link(self: &RustyAdapter, target: &mut Adapter);
        fn unlink(self: &RustyAdapter);

        fn identifier(self: &RustyAdapter) -> String;
        fn address(self: &RustyAdapter) -> String;

        fn scan_start(self: &RustyAdapter);
        fn scan_stop(self: &RustyAdapter);
        fn scan_for(self: &RustyAdapter, timeout_ms: i32);
        fn scan_is_active(self: &RustyAdapter) -> bool;
        fn scan_get_results(self: &RustyAdapter) -> Vec<RustyPeripheralWrapper>;

        fn set_callback_on_scan_start(self: &RustyAdapter, cb: fn());

        #[namespace = "SimpleBLE"]
        type RustyPeripheral;

        fn identifier(self: &RustyPeripheral) -> String;
        fn address(self: &RustyPeripheral) -> String;



    }
}


pub struct Adapter {
    internal: cxx::UniquePtr<ffi::RustyAdapter>,
    on_scan_start: Box<dyn Fn() + Send + Sync + 'static>,
}

pub struct Peripheral {
    internal: cxx::UniquePtr<ffi::RustyPeripheral>,
}

impl Adapter {
    pub fn bluetooth_enabled() -> bool {
        return ffi::RustyAdapter_bluetooth_enabled();
    }

    pub fn get_adapters() -> Vec<Pin<Box<Adapter>>> {
        let mut adapters = Vec::<Pin<Box<Adapter>>>::new();

        for adapter_wrapper in ffi::RustyAdapter_get_adapters().iter_mut() {
            adapters.push(Adapter::new(adapter_wrapper));
        }

        return adapters;
    }

    fn new(wrapper: &mut ffi::RustyAdapterWrapper) -> Pin<Box<Self>> {
        let mut this = Self {
            internal: cxx::UniquePtr::<ffi::RustyAdapter>::null(),
            on_scan_start: Box::new(||{}),
        };

        // Configure the internal object while still inside the wrappers
        wrapper.internal.link(&mut this);

        // Load the internal object held by the wrapper into this class.
        mem::swap(&mut this.internal, &mut wrapper.internal);

        let this_boxed = Box::pin(this);
        return this_boxed;
    }

    pub fn identifier(&self) -> String {
        return self.internal.identifier();
    }

    pub fn address(&self) -> String {
        return self.internal.address();
    }

    pub fn scan_start(&self) {
        self.internal.scan_start();
    }

    pub fn scan_stop(&self) {
        self.internal.scan_stop();
    }

    pub fn scan_for(&self, timeout_ms: i32) {
        self.internal.scan_for(timeout_ms);
    }

    pub fn scan_is_active(&self) {
        self.internal.scan_is_active();
    }

    pub fn scan_get_results(&self) -> Vec<Peripheral> {
        let mut peripherals = Vec::<Peripheral>::new();

        for peripheral_wrapper in self.internal.scan_get_results().iter_mut() {
            peripherals.push(Peripheral::new(peripheral_wrapper));
        }

        return peripherals;
    }

    pub fn set_callback_on_scan_start(&mut self, cb: Box<dyn Fn() + Send + Sync + 'static>) {
        self.on_scan_start = cb;
    }

    fn on_callback_scan_start(&mut self) {
        println!("On callback scan start");
    }
}

impl Peripheral {
    fn new(wrapper: &mut ffi::RustyPeripheralWrapper) -> Self {
        let mut this = Self {
            internal: cxx::UniquePtr::<ffi::RustyPeripheral>::null(),
        };
        mem::swap(&mut this.internal, &mut wrapper.internal);
        return this;
    }

    pub fn identifier(&self) -> String {
        return self.internal.identifier();
    }

    pub fn address(&self) -> String {
        return self.internal.address();
    }
}


unsafe impl Sync for Adapter {}

unsafe impl Sync for Peripheral {}

unsafe impl Send for Adapter {}

unsafe impl Send for Peripheral {}


impl Drop for Adapter {
    fn drop(&mut self) {
        self.internal.unlink();
    }
}
