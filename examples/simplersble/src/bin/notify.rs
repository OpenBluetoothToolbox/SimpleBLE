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

    adapter.set_callback_on_scan_found(Box::new(|peripheral| {
        println!(
            "Found device: {} [{}] {} dBm",
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

    // Make a Vec of all service/characteristic pairs
    let mut service_characteristic_pairs = Vec::new();
    for service in peripheral.services().unwrap().iter() {
        for characteristic in service.characteristics().iter() {
            service_characteristic_pairs.push((service.uuid(), characteristic.uuid()));
        }
    }

    // Print the list of services and characteristics
    println!("The following services and characteristics were found:");
    for (i, (service, characteristic)) in service_characteristic_pairs.iter().enumerate() {
        println!("{}: {} {}", i, service, characteristic);
    }

    // Prompt the user to select a service/characteristic pair
    println!("Select a service/characteristic pair to subscribe to:");
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let input = input.trim();
    let input = input.parse::<usize>().unwrap();

    // Get the selected service/characteristic pair by moving it out of the Vec
    let (service, characteristic) = service_characteristic_pairs.remove(input);

    // Subscribe to the characteristic
    println!("Subscribing to characteristic...");
    peripheral.notify(&service, &characteristic, Box::new(|data| {
        println!("Received data: {:?}", data);
    })).unwrap();

    // Sleep for 5 seconds
    std::thread::sleep(std::time::Duration::from_secs(5));

    // Unsubscribe from the characteristic
    println!("Unsubscribing from characteristic...");
    peripheral.unsubscribe(&service, &characteristic).unwrap();

    peripheral.disconnect().unwrap();
}
