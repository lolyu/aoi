#ifndef _ICMP_HEADER_H_
#define _ICMP_HEADER_H_

#include <array>
#include <iostream>
#include <algorithm>

// ICMP header for both IPv4 and IPv6.
//
// The wire format of an ICMP header is:
//
// 0               8               16                             31
// +---------------+---------------+------------------------------+      ---
// |               |               |                              |       ^
// |     type      |     code      |          checksum            |       |
// |               |               |                              |       |
// +---------------+---------------+------------------------------+    8 bytes
// |                               |                              |       |
// |          identifier           |       sequence number        |       |
// |                               |                              |       v
// +-------------------------------+------------------------------+      ---

class icmp_header
{
public:
    enum
    {
        echo_reply = 0,
        destination_unreachable = 3,
        source_quench = 4,
        redirect = 5,
        echo_request = 8,
        time_exceeded = 11,
        parameter_problem = 12,
        timestamp_request = 13,
        timestamp_reply = 14,
        info_request = 15,
        info_reply = 16,
        address_request = 17,
        address_reply = 18
    };

    icmp_header() = default;

    unsigned char type() const { return data[0]; }
    void type(unsigned char n) { data[0] = n; }

    unsigned char code() const { return data[1]; }
    void code(unsigned char n) { data[1] = n; }

    unsigned short checksum() const { return decode(2, 3); }
    void checksum(unsigned short n) { encode(2, 3, n); }

    unsigned short identifier() const { return decode(4, 5); }
    void identifier(unsigned short n) { encode(4, 5, n); }

    unsigned short sequence_number() const { return decode(6, 7); }
    void sequence_number(unsigned short n) { encode(6, 7, n); }

    friend std::istream &operator>>(std::istream &is, icmp_header &header)
    {
        return is.read(reinterpret_cast<char *>(header.data.data()), header.data.size());
    }

    friend std::ostream &operator<<(std::ostream &os, const icmp_header &header)
    {
        return os.write(reinterpret_cast<const char *>(header.data.data()), header.data.size());
    }

private:
    void encode(int a, int b, unsigned short n)
    {
        data[a] = static_cast<unsigned char>(n >> 8);
        data[b] = static_cast<unsigned char>(n & 0xff);
    }

    unsigned short decode(int a, int b) const
    {
        return (data[a] << 8) + data[b];
    }

private:
    std::array<unsigned char, 8> data{0};
};

template <typename Iterator>
void compute_checksum(icmp_header &header, Iterator body_begin, Iterator body_end)
{
    uint32_t sum = (header.type() << 8) + header.code() + header.identifier() + header.sequence_number();

    for (Iterator bi = body_begin; bi != body_end;)
    {
        sum += (static_cast<unsigned char>(*bi++) << 8);
        if (bi != body_end)
        {
            sum += static_cast<unsigned char>(*bi++);
        }
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    header.checksum(static_cast<uint16_t>(~sum));
}

#endif
