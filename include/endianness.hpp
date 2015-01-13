#ifndef _FNORD_ENDIAN
#define _FNORD_ENDIAN

#include <xpc/xpc.h>

namespace fnord {
    namespace endianness {
        enum endian {
            little,
            big
        };

        template<enum endian E, typename T>
        constexpr T to_native(T v) noexcept;

        template<enum endian E, typename T>
        constexpr T from_native(T v) noexcept;

        template<>
        constexpr uint8_t to_native<endian::little>(uint8_t v) noexcept {
            return v;
        }

        template<>
        constexpr uint8_t from_native<endian::little>(uint8_t v) noexcept {
            return v;
        }

        template<>
        constexpr uint16_t to_native<endian::little>(uint16_t v) noexcept {
            return OSSwapLittleToHostConstInt16(v);
        }

        template<>
        constexpr uint16_t from_native<endian::little>(uint16_t v) noexcept {
            return OSSwapHostToLittleConstInt16(v);
        }

        template<>
        constexpr uint32_t to_native<endian::little>(uint32_t v) noexcept {
            return OSSwapLittleToHostConstInt32(v);
        }

        template<>
        constexpr uint32_t from_native<endian::little>(uint32_t v) noexcept {
            return OSSwapHostToLittleConstInt32(v);
        }

        template<>
        constexpr uint64_t to_native<endian::little>(uint64_t v) noexcept {
            return OSSwapLittleToHostConstInt64(v);
        }

        template<>
        constexpr uint64_t from_native<endian::little>(uint64_t v) noexcept {
            return OSSwapHostToLittleConstInt64(v);
        }


        template<>
        constexpr uint8_t to_native<endian::big>(uint8_t v) noexcept {
            return v;
        }

        template<>
        constexpr uint8_t from_native<endian::big>(uint8_t v) noexcept {
            return v;
        }

        template<>
        constexpr uint16_t to_native<endian::big>(uint16_t v) noexcept {
            return OSSwapBigToHostConstInt16(v);
        }

        template<>
        constexpr uint16_t from_native<endian::big>(uint16_t v) noexcept {
            return OSSwapHostToBigConstInt16(v);
        }

        template<>
        constexpr uint32_t to_native<endian::big>(uint32_t v) noexcept {
            return OSSwapBigToHostConstInt32(v);
        }

        template<>
        constexpr uint32_t from_native<endian::big>(uint32_t v) noexcept {
            return OSSwapHostToBigConstInt32(v);
        }

        template<>
        constexpr uint64_t to_native<endian::big>(uint64_t v) noexcept {
            return OSSwapBigToHostConstInt64(v);
        }

        template<>
        constexpr uint64_t from_native<endian::big>(uint64_t v) noexcept {
            return OSSwapHostToBigConstInt64(v);
        }
    }
}

#endif