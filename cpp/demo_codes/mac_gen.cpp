#include <array>
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

const uint8_t ETHER_ADDR_LEN = 6;

const std::array<uint8_t, ETHER_ADDR_LEN> KNOWN_MAC_START = {0x04, 0x27, 0x28, 0x7a, 0x00, 0x00};
const size_t KNOWN_MAC_COUNT = 1024;

void printMacArray(const std::array<uint8_t, ETHER_ADDR_LEN> &address)
{
    for (int i = 0; i < address.size(); ++i) {
        cout << std::hex << static_cast<int>(address[i]) << '\t';
    }
    cout << endl;
}

void generateServerMac(uint16_t serverId, std::array<uint8_t, ETHER_ADDR_LEN> &address)
{
    if (serverId >= KNOWN_MAC_COUNT) {
        throw std::range_error("Out of MAC address range");
    }
    int index = ETHER_ADDR_LEN - 1;
    uint32_t offset = KNOWN_MAC_START[index] + serverId;
    address = KNOWN_MAC_START;
    while (offset && index >= 0) {
        address[index--] = offset % 0xff;
        offset /= 0xff;
    }
}

int main()
{
    std::array<uint8_t, ETHER_ADDR_LEN> address;
    std::array<char, 3 *ETHER_ADDR_LEN> addressStr = {0};
    generateServerMac(2000, address);
    snprintf(
        addressStr.data(), addressStr.size(), "%02x:%02x:%02x:%02x:%02x:%02x",
        address[0], address[1], address[2], address[3], address[4], address[5]
    );
    printMacArray(KNOWN_MAC_START);
    printMacArray(address);
    cout << addressStr.data() << endl;
    return 0;
}