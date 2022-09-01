#include "uuid.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <uuid/uuid.h>
#endif

namespace wellgen::util {
    std::string get_uuid() {
        std::string res;
#ifdef _WIN32
        UUID uuid;
        UuidCreate(&uuid);
        char* uuid_cstr = nullptr;
        UuidToStringA(&uuid, reinterpret_cast<RPC_CSTR*>(&uuid_cstr));
        res = std::string(uuid_cstr);
        RpcStringFreeA(reinterpret_cast<RPC_CSTR*>(&uuid_cstr));
#else
        uuid_t uuid;
        char uuid_cstr[37]; // 36 byte uuid plus null.
        uuid_generate(uuid);
        uuid_unparse(uuid, uuid_cstr);
        res = std::string(uuid_cstr);
#endif
        return res;
    }
}
