use simplersble;

fn main() {
    let mut adapters = simplersble::Adapter::get_adapters().unwrap();

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
            peripheral.identifier().unwrap(),
            peripheral.address().unwrap(),
            peripheral.rssi().unwrap()
        );
    }));

    adapter.set_callback_on_scan_updated(Box::new(|peripheral| {
        println!(
            "Updated device: {} [{}] {} dBm",
            peripheral.identifier().unwrap(),
            peripheral.address().unwrap(),
            peripheral.rssi().unwrap()
        );
    }));

    adapter.scan_for(5000).unwrap();
    println!("Scan complete.");

    println!("The following devices were found:");

    for (i, peripheral) in adapter.scan_get_results().unwrap().iter().enumerate() {
        let connectable_str = "unknown";
        let peripheral_str = format!(
            "{} [{}] {} dBm",
            peripheral.identifier().unwrap(),
            peripheral.address().unwrap(),
            peripheral.rssi().unwrap()
        );

        println!("{}: {} {}", i, peripheral_str, connectable_str);
    }

    // Prompt the user to select a device
    println!("Select a device to connect to:");
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let input = input.trim();
    let input = input.parse::<usize>().unwrap();

    // Get the selected device by moving it out of the scan results
    let mut peripheral = adapter.scan_get_results().unwrap().remove(input);

    peripheral.set_callback_on_connected(Box::new(|| {
        println!("Connected to device.");
    }));

    peripheral.set_callback_on_disconnected(Box::new(|| {
        println!("Disconnected from device.");
    }));

    // Connect to the device
    println!("Connecting to device...");
    peripheral.connect().unwrap();

    println!("Connected to device.");
    println!("MTU: {}", peripheral.mtu().unwrap());

    for service in peripheral.services().unwrap().iter() {
        println!("Service: {}", service.uuid());

        for characteristic in service.characteristics().iter() {
            println!("    Characteristic: {}", characteristic.uuid());
            println!("        Capabilities: {:?}", characteristic.capabilities());
            for descriptor in characteristic.descriptors().iter() {
                println!("        Descriptor: {}", descriptor.uuid());
            }
        }
    }

    peripheral.disconnect().unwrap();
}
