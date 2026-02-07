#ifndef VARINT_H
#define VARINT_H
/*
 * Variable-Length Integer Encoding Library (varint)
 * Version: 1.0
 * Copyright (C) 2026 Masahito Suzuki
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * @file varint.h
 * @brief A C99 header-only library for variable-length integer encoding.
 *
 * @details
 * This library provides compact variable-length integer encoding that uses
 * fewer bytes for smaller values, ideal for network protocols, file formats,
 * and data serialization.
 *
 * Supported encoding formats:
 * - 2-bit header (varint2): 1-4 bytes, max 1,073,741,823
 * - 1-bit header (varint1): 1-2 bytes, max 32,767
 *
 * All encodings use big-endian (network byte order).
 */

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t value;
    uint8_t  bytes_read;
} varint_decode_result_t;

/* max_bytes = 2^header_bits */
#define VARINT_MAX_BYTES(header_bits) (1 << (header_bits))

/* Max value for n-byte encoding */
#define VARINT_NBYTE_MAX(header_bits, bytes) ((1UL << ((bytes) * 8 - (header_bits))) - 1)

/* Max value for format */
#define VARINT_MAX(header_bits) VARINT_NBYTE_MAX(header_bits, VARINT_MAX_BYTES(header_bits))

#define VARINT_DEFINE_ALL(name, vtype, header_bits) \
/* Returns encoded byte length (1-MAX_BYTES), no range check */ \
static inline size_t name##__encode_len(vtype value) { \
    for (size_t i = VARINT_MAX_BYTES(header_bits); i > 1; i--) \
        if ((uint32_t)(value) > VARINT_NBYTE_MAX(header_bits, i - 1)) return i; \
    return 1; \
} \
/* Returns encoded byte length, or 0 if value > MAX */ \
static inline size_t name##_encode_len(vtype value) { \
    if ((uint32_t)(value) > VARINT_MAX(header_bits)) return 0; \
    return name##__encode_len(value); \
} \
/* Encodes value to buf, returns byte length, no range check */ \
static inline size_t name##__encode(vtype value, uint8_t *buf, size_t offset) { \
    size_t len = name##__encode_len(value); \
    uint8_t mask = (uint8_t)((1 << (8 - (header_bits))) - 1); \
    buf[offset] = (uint8_t)(((len - 1) << (8 - (header_bits))) | ((value >> ((len - 1) * 8)) & mask)); \
    for (size_t i = 1; i < len; i++) \
        buf[offset + i] = (uint8_t)((value >> ((len - 1 - i) * 8)) & 0xFF); \
    return len; \
} \
/* Encodes value to buf, returns byte length, or 0 if value > MAX */ \
static inline size_t name##_encode(vtype value, uint8_t *buf, size_t offset) { \
    if ((uint32_t)(value) > VARINT_MAX(header_bits)) return 0; \
    return name##__encode(value, buf, offset); \
} \
/* Returns byte length from encoded header */ \
static inline size_t name##_decode_len(const uint8_t *buf, size_t offset) { \
    return ((buf[offset] >> (8 - (header_bits))) & ((1 << (header_bits)) - 1)) + 1; \
} \
/* Decodes buf to value, returns {value, bytes_read} */ \
static inline varint_decode_result_t name##_decode(const uint8_t *buf, size_t offset) { \
    size_t len = name##_decode_len(buf, offset); \
    uint8_t mask = (uint8_t)((1 << (8 - (header_bits))) - 1); \
    uint32_t value = buf[offset] & mask; \
    for (size_t i = 1; i < len; i++) \
        value = (value << 8) | buf[offset + i]; \
    return (varint_decode_result_t){value, len}; \
}
VARINT_DEFINE_ALL(varint2, uint32_t, 2)  /* 2-bit header, 1-4 bytes */
VARINT_DEFINE_ALL(varint1, uint16_t, 1)  /* 1-bit header, 1-2 bytes */

#endif /* VARINT_H */
