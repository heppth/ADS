/**
   Copyright (c) 2015 - 2016 Beckhoff Automation GmbH & Co. KG

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
 */

#include "AdsDef.h"

#include <stdlib.h>
#include <string.h>

bool operator<(const AmsNetId& lhs, const AmsNetId& rhs)
{
    for (unsigned int i = 0; i < sizeof(rhs.b); ++i) {
        if (lhs.b[i] != rhs.b[i]) {
            return lhs.b[i] < rhs.b[i];
        }
    }
    return false;
}

bool operator<(const AmsAddr& lhs, const AmsAddr& rhs)
{
    if (memcmp(&lhs.netId, &rhs.netId, sizeof(lhs.netId))) {
        return lhs.netId < rhs.netId;
    }
    return lhs.port < rhs.port;
}

AmsNetId make_AmsNetId(const std::string& addr)
{
    // Handwritten parser instead of std::istringstream/std::getline:
    // the stream classes pull the complete libstdc++ locale/iostream code (~400 KB) into static binaries.
    // Semantics are identical to the former getline('.') loop.
    const char* p = addr.c_str();
    const char* const end = p + addr.size();
    size_t i = 0;
    AmsNetId id {};

    while ((i < sizeof(id.b)) && (p != end)) {
        const char* dot = static_cast<const char*>(memchr(p, '.', static_cast<size_t>(end - p)));
        const char* const tokenEnd = dot ? dot : end;
        char token[16];
        const size_t len = static_cast<size_t>(tokenEnd - p) < sizeof(token) - 1 ? static_cast<size_t>(tokenEnd - p) : sizeof(token) - 1;
        memcpy(token, p, len);
        token[len] = '\0';
        id.b[i] = atoi(token) % 256;
        ++i;
        p = dot ? dot + 1 : end;
    }

    if ((i != sizeof(id.b)) || (p != end)) {
        memset(id.b, 0, sizeof(id.b));
    }
    return id;
}
