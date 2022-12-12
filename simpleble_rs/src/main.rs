use simplersble;

fn main() {

    let x = simplersble::add(4, 3);

    println!("X was {} - Z was {}", x, simplersble::Adapter::bluetooth_enabled());

}
