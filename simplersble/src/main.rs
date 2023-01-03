use simplersble;

fn main() {
    println!(
        "Bluetooth enabled: {}",
        simplersble::Adapter::bluetooth_enabled()
    );

    let mut adapters = simplersble::Adapter::get_adapters();
    println!("Adapters length is {}", adapters.len());

    for adapter in adapters.iter_mut() {
        println!("Adapter identifier is {}", adapter.identifier());
        println!("Adapter address is {}", adapter.address());

        adapter.set_callback_on_scan_start(Box::new(|| {
            println!("Scan started.");
        }));

        adapter.set_callback_on_scan_stop(Box::new(|| {
            println!("Scan stopped.");
        }));

        adapter.scan_for(1000);
        println!("Scan complete");

        for peripheral in adapter.scan_get_results().iter() {
            println!("Peripheral identifier is {}", peripheral.identifier());
            println!("Peripheral address is {}", peripheral.address())
        }
    }
}
