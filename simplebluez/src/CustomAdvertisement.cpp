#include <simplebluez/CustomAdvertisement.h>
#include <simpledbus/interfaces/ObjectManager.h>

using namespace SimpleBluez;

CustomAdvertisement::CustomAdvertisement(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {

    _interfaces.emplace(std::make_pair("org.bluez.LEAdvertisement1", std::make_shared<LEAdvertisement1>(conn, this)));
    _interfaces.emplace(std::make_pair("org.freedesktop.DBus.ObjectManager", std::make_shared<SimpleDBus::ObjectManager>(conn, this)));
}

std::shared_ptr<LEAdvertisement1> CustomAdvertisement::le_advertisement1() {
    return std::dynamic_pointer_cast<LEAdvertisement1>(interface_get("org.bluez.LEAdvertisement1"));
}

std::shared_ptr<SimpleDBus::ObjectManager> CustomAdvertisement::object_manager() {
    return std::dynamic_pointer_cast<SimpleDBus::ObjectManager>(interface_get("org.freedesktop.DBus.ObjectManager"));
}

std::string CustomAdvertisement::adv_type() {
    return le_advertisement1()->Type();
}

void CustomAdvertisement::adv_type(const std::string& type) {
    le_advertisement1()->Type(type);
}

std::vector<std::string> CustomAdvertisement::service_uuids() {
    return le_advertisement1()->ServiceUUIDs();
}

void CustomAdvertisement::service_uuids(const std::vector<std::string>& service_uuids) {
    le_advertisement1()->ServiceUUIDs(service_uuids);
}

std::map<uint16_t, std::vector<uint8_t>> CustomAdvertisement::manufacturer_data() {
    return le_advertisement1()->ManufacturerData();
}

void CustomAdvertisement::manufacturer_data(const std::map<uint16_t, std::vector<uint8_t>>& manufacturer_data) {
    le_advertisement1()->ManufacturerData(manufacturer_data);
}

std::map<std::string, std::vector<uint8_t>> CustomAdvertisement::service_data() {
    return le_advertisement1()->ServiceData();
}

void CustomAdvertisement::service_data(const std::map<std::string, std::vector<uint8_t>>& service_data) {
    le_advertisement1()->ServiceData(service_data);
}

std::vector<std::string> CustomAdvertisement::solicit_uuids() {
    return le_advertisement1()->SolicitUUIDs();
}

void CustomAdvertisement::solicit_uuids(const std::vector<std::string>& solicit_uuids) {
    le_advertisement1()->SolicitUUIDs(solicit_uuids);
}

std::map<uint8_t, std::vector<uint8_t>> CustomAdvertisement::data() {
    return le_advertisement1()->Data();
}

void CustomAdvertisement::data(const std::map<uint8_t, std::vector<uint8_t>>& data) {
    le_advertisement1()->Data(data);
}

bool CustomAdvertisement::discoverable() {
    return le_advertisement1()->Discoverable();
}

void CustomAdvertisement::discoverable(bool discoverable) {
    le_advertisement1()->Discoverable(discoverable);
}

uint16_t CustomAdvertisement::discoverable_timeout() {
    return le_advertisement1()->DiscoverableTimeout();
}

void CustomAdvertisement::discoverable_timeout(uint16_t timeout) {
    le_advertisement1()->DiscoverableTimeout(timeout);
}

std::vector<std::string> CustomAdvertisement::includes() {
    return le_advertisement1()->Includes();
}

void CustomAdvertisement::includes(const std::vector<std::string>& includes) {
    le_advertisement1()->Includes(includes);
}

std::string CustomAdvertisement::local_name() {
    return le_advertisement1()->LocalName();
}

void CustomAdvertisement::local_name(const std::string& name) {
    le_advertisement1()->LocalName(name);
}

uint16_t CustomAdvertisement::appearance() {
    return le_advertisement1()->Appearance();
}

void CustomAdvertisement::appearance(uint16_t appearance) {
    le_advertisement1()->Appearance(appearance);
}

uint16_t CustomAdvertisement::duration() {
    return le_advertisement1()->Duration();
}

void CustomAdvertisement::duration(uint16_t duration) {
    le_advertisement1()->Duration(duration);
}

uint16_t CustomAdvertisement::timeout() {
    return le_advertisement1()->Timeout();
}

void CustomAdvertisement::timeout(uint16_t timeout) {
    le_advertisement1()->Timeout(timeout);
}

std::string CustomAdvertisement::secondary_channel() {
    return le_advertisement1()->SecondaryChannel();
}

void CustomAdvertisement::secondary_channel(const std::string& channel) {
    le_advertisement1()->SecondaryChannel(channel);
}

uint32_t CustomAdvertisement::min_interval() {
    return le_advertisement1()->MinInterval();
}

void CustomAdvertisement::min_interval(uint32_t interval) {
    le_advertisement1()->MinInterval(interval);
}

uint32_t CustomAdvertisement::max_interval() {
    return le_advertisement1()->MaxInterval();
}

void CustomAdvertisement::max_interval(uint32_t interval) {
    le_advertisement1()->MaxInterval(interval);
}

int16_t CustomAdvertisement::tx_power() {
    return le_advertisement1()->TxPower();
}

void CustomAdvertisement::tx_power(int16_t power) {
    le_advertisement1()->TxPower(power);
}

bool CustomAdvertisement::include_tx_power() {
    return le_advertisement1()->IncludeTxPower();
}

void CustomAdvertisement::include_tx_power(bool include) {
    le_advertisement1()->IncludeTxPower(include);
}
