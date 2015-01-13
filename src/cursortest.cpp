#include "buffer.hpp"

using namespace fnord;

int
main(int argc, char **argv) {
    char data[32];
    le_buffer b(data, sizeof(data));

    auto write = b.begin();
    write.put<uint16_t>(0x1234);

    return write.flip().begin().get<uint16_t>();
}
