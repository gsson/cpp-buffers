#ifndef _FNORD_BUFFER
#define _FNORD_BUFFER

#include <cassert>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <sys/uio.h>

#include "endianness.hpp"

namespace fnord {
    using namespace std;
    using namespace fnord::endianness;

    template<enum endian E>
    class buffer {
    private:
        uint8_t *const start;
        uint8_t *const end;
    public:
        constexpr buffer(uint8_t *const start, uint8_t *const end) noexcept : start(start), end(end) {}

        constexpr buffer(void * const start, size_t length) noexcept : buffer(
                reinterpret_cast<uint8_t * const>(start),
                reinterpret_cast<uint8_t * const>(start) + length) {}

        constexpr buffer(const struct iovec &v) noexcept : buffer(v.iov_base, v.iov_len) {}

        static constexpr const buffer empty() {
            return buffer(reinterpret_cast<uint8_t *const>(NULL), reinterpret_cast<uint8_t *const>(NULL));
        }

        class cursor {
        private:
            friend class buffer;
            const buffer parent;
            uint8_t *pos;

            constexpr cursor(const buffer parent, uint8_t *pos) noexcept : parent(parent), pos(pos) {}

        public:

            static constexpr const cursor empty() noexcept {
                return buffer::empty().begin();
            }

            template<typename T>
            T get() noexcept {
                T *t = reinterpret_cast<T *>(pos);
                offset(sizeof(T));
                return to_native<E>(*t);
            }

            template<typename T>
            void put(const T value) noexcept {
                *reinterpret_cast<T *>(pos) = from_native<E>(value);
                offset(sizeof(T));
                assert(pos <= parent.end);
            }

            void get(cursor other) noexcept {
                size_t n = other.remaining() < remaining() ? other.remaining() : remaining();
                memcpy(other.pos, pos, n);
                other.pos += n;
                pos += n;
            }

            void put(cursor other) noexcept {
                size_t n = other.remaining() < remaining() ? other.remaining() : remaining();
                memcpy(pos, other.pos, n);
                other.pos += n;
                pos += n;
            }

            void put(const buffer other) noexcept {
                size_t n = other.length() < remaining() ? other.length() : remaining();
                memcpy(pos, other.start, n);
                pos += n;
            }

            size_t remaining() const noexcept {
                return parent.end - pos;
            }

            size_t position() const noexcept {
                return pos - parent.start;
            }

            void position(size_t position) noexcept {
                pos = parent.start + position;
                assert(pos < parent.end);
            }

            void reset() noexcept {
                pos = parent.start;
            }

            void offset(ssize_t offset) noexcept {
                pos += offset;
                assert(pos <= parent.end);
                assert(pos >= parent.start);
            }

            void compact() noexcept {
                size_t r = remaining();
                memmove(parent.start, pos, r);
                pos = parent.start + r;
            }

            buffer flip() const noexcept {
                return buffer(parent.start, pos);
            }

            buffer slice() const noexcept {
                return buffer(pos, parent.end);
            }

            buffer consume(size_t length) noexcept {
                uint8_t *b = pos;
                offset(length);
                return buffer(b, pos);
            }
        };

        size_t length() const noexcept {
            return end - start;
        }

        cursor begin() const noexcept {
            return cursor(*this, start);
        }

        template<enum endian E2>
        buffer<E2> order() const noexcept {
            return buffer<E2>(start, end);
        }

        operator iovec() const noexcept {
            return iovec{ start, length() };
        }

        operator void *() const noexcept {
            return start;
        }

        bool operator==(const buffer &other) const noexcept {
            return start == other.start && end == other.end;
        }

        bool operator!=(const buffer &other) const noexcept {
            return start != other.start || end != other.end;
        }

    };

    using le_buffer = buffer<endian::little>;
    using be_buffer = buffer<endian::big>;
}
#endif