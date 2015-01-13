#include "buffer.hpp"
#include "chunk_assembler.hpp"

using namespace fnord;
using namespace fnord::endianness;

enum frame_state {
    INITIAL,
    HEAD, PAYLOAD
};

void
test_assembler() {
    chunk_assembler<le_buffer, enum frame_state, INITIAL, 8> a;

    uint32_t n = 0;
    auto parser = [&] (auto id, auto b, auto next) {
        le_buffer::cursor cursor = b.begin();
        switch(id) {
            case HEAD:
                next.length = cursor.get<uint32_t>();
                next.id = PAYLOAD;
                break;
            case PAYLOAD:
                n += cursor.get<uint32_t>();
                /* FALLTHROUGH */
            case INITIAL:
                next.length = 4;
                next.id = HEAD;
                break;
        }
    };

    char data[32];
    le_buffer b(data, sizeof(data));

    // Put three "messages" into the buffer
    auto w = b.begin();
    w.put<uint32_t>(4);
    w.put<uint32_t>(1);

    w.put<uint32_t>(4);
    w.put<uint32_t>(2);

    w.put<uint32_t>(4);
    w.put<uint32_t>(3);

    // Four sub-views, each containing only a part of a message
    le_buffer b1(data, 6);
    le_buffer b2(data + 6, 6);
    le_buffer b3(data + 12, 6);
    le_buffer b4(data + 18, 6);


    a.assemble(b1, parser);
    assert(n == 0);

    a.assemble(b2, parser);
    assert(n == 1);

    a.assemble(b3, parser);
    assert(n == 3);

    a.assemble(b4, parser);
    assert(n == 6);
}

void
test_cursor() {
    char data[32];
    le_buffer b(data, sizeof(data));

    auto write = b.begin();
    for (int i = 0; i < 10; i++)
        write.put<uint8_t>(i);

    assert(write.position() == 10);
    assert(write.remaining() == 22);

    auto read = write.flip().begin();

    assert(read.position() == 0);
    assert(read.remaining() == 10);

    for (int i = 0; i < 10; i++) {
        uint8_t v = read.get<uint8_t>();
        assert(v == i);
    }
    assert(read.remaining() == 0);
}

void
test_iovec() {
    char data[32];
    le_buffer b(data, sizeof(data));

    struct iovec iov = b;

    le_buffer c = iov;

    assert(b == c);
}

int
main(int argc, char **argv) {
    test_assembler();
    test_cursor();
    test_iovec();
}