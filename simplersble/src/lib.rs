use std::mem;


#[cxx::bridge]
mod ffi {

    #[namespace = "SimpleBLE"]
    struct RustyWrapper {
        internal: UniquePtr::<RustyAdapter>
    }

    unsafe extern "C++" {
        include!("simplersble/src/bindings/AdapterBindings.hpp");

        #[namespace = "SimpleBLE"]
        type RustyAdapter;

        fn RustyAdapter_bluetooth_enabled() -> bool;
        fn RustyAdapter_get_adapters() -> Vec::<RustyWrapper>;

        fn identifier(&self) -> String;
        fn address(&self) -> String;

        fn scan_start(&self);
        fn scan_stop(&self);
        fn scan_for(&self, timeout_ms: i32);
        fn scan_is_active(&self) -> bool;
        // std::vector<Peripheral> scan_get_results();

        // #[namespace = "SimpleBLE"]
        // type RustyPeripheral;

        // fn identifier(&self) -> String;
        // fn address(&self) -> String;

    }

}

pub struct Adapter {
    internal : cxx::UniquePtr::<ffi::RustyAdapter>,
}

// pub struct Peripheral {
//     internal : cxx::UniquePtr::<ffi::RustyPeripheral>,
// }

impl Adapter {

    pub fn bluetooth_enabled() -> bool {
        return ffi::RustyAdapter_bluetooth_enabled();
    }

    pub fn get_adapters() -> Vec::<Adapter> {
        let mut adapters = Vec::<Adapter>::new();

        for adapter_wrapper in ffi::RustyAdapter_get_adapters().iter_mut() {
            adapters.push(Adapter::new(adapter_wrapper));
        }

        return adapters;
    }

    fn new(wrapper: &mut ffi::RustyWrapper) -> Self {
        let mut this = Self { internal: cxx::UniquePtr::<ffi::RustyAdapter>::null() };
        mem::swap(&mut this.internal, &mut wrapper.internal);
        return this;
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

}
