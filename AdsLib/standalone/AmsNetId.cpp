// SPDX-License-Identifier: MIT
/**
   Copyright (c) 2020 Beckhoff Automation GmbH & Co. KG
 */

#include "AdsDef.h"
#include <string.h>

AmsNetId::AmsNetId(uint32_t ipv4Addr)
{
    b[5] = 1;
    b[4] = 1;
    b[3] = (uint8_t)(ipv4Addr & 0x000000ff);
    b[2] = (uint8_t)((ipv4Addr & 0x0000ff00) >> 8);
    b[1] = (uint8_t)((ipv4Addr & 0x00ff0000) >> 16);
    b[0] = (uint8_t)((ipv4Addr & 0xff000000) >> 24);
}

AmsNetId::AmsNetId(uint8_t id_0, uint8_t id_1, uint8_t id_2, uint8_t id_3, uint8_t id_4, uint8_t id_5)
{
    b[5] = id_5;
    b[4] = id_4;
    b[3] = id_3;
    b[2] = id_2;
    b[1] = id_1;
    b[0] = id_0;
}

// Implemented in AdsLib/AdsDef.cpp
AmsNetId make_AmsNetId(const std::string& addr);

AmsNetId::AmsNetId(const std::string& addr)
{
    // Parsing without iostream, see AdsDef.cpp
    *this = make_AmsNetId(addr);
}

AmsNetId::operator bool() const
{
    static const AmsNetId empty {};
    return 0 != memcmp(b, &empty.b, sizeof(b));
}
