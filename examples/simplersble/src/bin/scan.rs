use simplersble;

fn main() {
    let mut adapters = simplersble::Adapter::get_adapters();

    // If the adapter list is empty, print a message and exit
    if adapters.is_empty() {
        println!("No adapters found.");
        return;
    }

    // Pick the first adapter
    let mut adapter = adapters.pop().unwrap();

    adapter.set_callback_on_scan_start(Box::new(|| {
        println!("Scan started.");
    }));

    adapter.set_callback_on_scan_stop(Box::new(|| {
        println!("Scan stopped.");
    }));

    adapter.set_callback_on_scan_found(Box::new(|peripheral| {
        println!(
            "Found device: {} [{}] {} dBm",
            peripheral.identifier(),
            peripheral.address(),
            peripheral.rssi()
        );
    }));

    adapter.set_callback_on_scan_updated(Box::new(|peripheral| {
        println!(
            "Updated device: {} [{}] {} dBm",
            peripheral.identifier(),
            peripheral.address(),
            peripheral.rssi()
        );
    }));

    adapter.scan_for(5000);
    println!("Scan complete.");

    println!("The following devices were found:");

    for (i, peripheral) in adapter.scan_get_results().iter().enumerate() {
        let connectable_str = "unknown";
        let peripheral_str = format!(
            "{} [{}] {} dBm",
            peripheral.identifier(),
            peripheral.address(),
            peripheral.rssi()
        );

        println!("{}: {} {}", i, peripheral_str, connectable_str);

        println!("    Tx Power: {}", peripheral.tx_power());
        println!("    Address Type: {}", peripheral.address_type());

        for service in peripheral.services().iter() {
            println!("    Service: {}", service.uuid());
        }

        for (manufacturer_id, manufacturer_data) in peripheral.manufacturer_data().iter() {
            println!("    Manufacturer ID: {}", manufacturer_id);
            println!("    Manufacturer Data: {:?}", manufacturer_data);
        }
    }
}
