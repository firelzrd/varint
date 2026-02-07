/*
 * test_varint.c - Test suite for varint library
 */

#include <stdio.h>
#include <string.h>
#include "varint.h"

static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_run++; \
    printf("  %s ... ", name); \
} while(0)

#define PASS() do { \
    tests_passed++; \
    printf("PASS\n"); \
} while(0)

#define FAIL(msg) do { \
    printf("FAIL: %s\n", msg); \
    return; \
} while(0)

#define ASSERT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        printf("FAIL: expected %u, got %u\n", (unsigned)(expected), (unsigned)(actual)); \
        return; \
    } \
} while(0)

/* ============================================================================
 * 2-bit header tests
 * ============================================================================ */

static void test_varint2_encode_len(void)
{
    TEST("varint2_encode_len");

    ASSERT_EQ(1, varint2_encode_len(0));
    ASSERT_EQ(1, varint2_encode_len(63));
    ASSERT_EQ(2, varint2_encode_len(64));
    ASSERT_EQ(2, varint2_encode_len(16383));
    ASSERT_EQ(3, varint2_encode_len(16384));
    ASSERT_EQ(3, varint2_encode_len(4194303));
    ASSERT_EQ(4, varint2_encode_len(4194304));
    ASSERT_EQ(4, varint2_encode_len(1073741823));

    PASS();
}

static void test_varint2_roundtrip_1byte(void)
{
    TEST("varint2 roundtrip 1-byte");

    uint8_t buf[4] = {0};
    uint32_t test_values[] = {0, 1, 31, 63};

    for (size_t i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        uint32_t val = test_values[i];
        size_t written = varint2_encode(val, buf, 0);
        ASSERT_EQ(1, written);

        varint_decode_result_t result = varint2_decode(buf, 0);
        ASSERT_EQ(val, result.value);
        ASSERT_EQ(1, result.bytes_read);
    }

    PASS();
}

static void test_varint2_roundtrip_2byte(void)
{
    TEST("varint2 roundtrip 2-byte");

    uint8_t buf[4] = {0};
    uint32_t test_values[] = {64, 100, 1000, 16383};

    for (size_t i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        uint32_t val = test_values[i];
        size_t written = varint2_encode(val, buf, 0);
        ASSERT_EQ(2, written);

        varint_decode_result_t result = varint2_decode(buf, 0);
        ASSERT_EQ(val, result.value);
        ASSERT_EQ(2, result.bytes_read);
    }

    PASS();
}

static void test_varint2_roundtrip_3byte(void)
{
    TEST("varint2 roundtrip 3-byte");

    uint8_t buf[4] = {0};
    uint32_t test_values[] = {16384, 100000, 1000000, 4194303};

    for (size_t i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        uint32_t val = test_values[i];
        size_t written = varint2_encode(val, buf, 0);
        ASSERT_EQ(3, written);

        varint_decode_result_t result = varint2_decode(buf, 0);
        ASSERT_EQ(val, result.value);
        ASSERT_EQ(3, result.bytes_read);
    }

    PASS();
}

static void test_varint2_roundtrip_4byte(void)
{
    TEST("varint2 roundtrip 4-byte");

    uint8_t buf[4] = {0};
    uint32_t test_values[] = {4194304, 10000000, 100000000, 1073741823};

    for (size_t i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        uint32_t val = test_values[i];
        size_t written = varint2_encode(val, buf, 0);
        ASSERT_EQ(4, written);

        varint_decode_result_t result = varint2_decode(buf, 0);
        ASSERT_EQ(val, result.value);
        ASSERT_EQ(4, result.bytes_read);
    }

    PASS();
}

static void test_varint2_offset(void)
{
    TEST("varint2 with offset");

    uint8_t buf[10] = {0};
    buf[0] = 0xAA;  /* padding */
    buf[1] = 0xBB;  /* padding */

    /* Encode at offset 2 */
    size_t written = varint2_encode(1000, buf, 2);
    ASSERT_EQ(2, written);

    /* Decode from offset 2 */
    varint_decode_result_t result = varint2_decode(buf, 2);
    ASSERT_EQ(1000, result.value);
    ASSERT_EQ(2, result.bytes_read);

    PASS();
}

/* ============================================================================
 * 1-bit header tests
 * ============================================================================ */

static void test_varint1_encode_len(void)
{
    TEST("varint1_encode_len");

    ASSERT_EQ(1, varint1_encode_len(0));
    ASSERT_EQ(1, varint1_encode_len(127));
    ASSERT_EQ(2, varint1_encode_len(128));
    ASSERT_EQ(2, varint1_encode_len(32767));

    PASS();
}

static void test_varint1_roundtrip_1byte(void)
{
    TEST("varint1 roundtrip 1-byte");

    uint8_t buf[2] = {0};
    uint16_t test_values[] = {0, 1, 63, 127};

    for (size_t i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        uint16_t val = test_values[i];
        size_t written = varint1_encode(val, buf, 0);
        ASSERT_EQ(1, written);

        varint_decode_result_t result = varint1_decode(buf, 0);
        ASSERT_EQ(val, result.value);
        ASSERT_EQ(1, result.bytes_read);
    }

    PASS();
}

static void test_varint1_roundtrip_2byte(void)
{
    TEST("varint1 roundtrip 2-byte");

    uint8_t buf[2] = {0};
    uint16_t test_values[] = {128, 1000, 10000, 32767};

    for (size_t i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        uint16_t val = test_values[i];
        size_t written = varint1_encode(val, buf, 0);
        ASSERT_EQ(2, written);

        varint_decode_result_t result = varint1_decode(buf, 0);
        ASSERT_EQ(val, result.value);
        ASSERT_EQ(2, result.bytes_read);
    }

    PASS();
}

static void test_varint1_offset(void)
{
    TEST("varint1 with offset");

    uint8_t buf[10] = {0};
    buf[0] = 0xAA;
    buf[1] = 0xBB;
    buf[2] = 0xCC;

    /* Encode at offset 3 */
    size_t written = varint1_encode(1000, buf, 3);
    ASSERT_EQ(2, written);

    /* Decode from offset 3 */
    varint_decode_result_t result = varint1_decode(buf, 3);
    ASSERT_EQ(1000, result.value);
    ASSERT_EQ(2, result.bytes_read);

    PASS();
}

/* ============================================================================
 * Edge case tests - varint2
 * ============================================================================ */

static void test_varint2_boundaries(void)
{
    TEST("varint2 all boundary values");

    uint8_t buf[4] = {0};
    /* Boundary values: MAX for each byte length, and MAX+1 (transition) */
    uint32_t boundaries[] = {
        0,                  /* min */
        1,
        62, 63,             /* 1-byte max-1, max */
        64, 65,             /* 2-byte min, min+1 */
        16382, 16383,       /* 2-byte max-1, max */
        16384, 16385,       /* 3-byte min, min+1 */
        4194302, 4194303,   /* 3-byte max-1, max */
        4194304, 4194305,   /* 4-byte min, min+1 */
        1073741822, 1073741823  /* 4-byte max-1, max (absolute max) */
    };
    size_t expected_lens[] = {
        1, 1, 1, 1,
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4
    };

    for (size_t i = 0; i < sizeof(boundaries)/sizeof(boundaries[0]); i++) {
        uint32_t val = boundaries[i];
        size_t written = varint2_encode(val, buf, 0);
        ASSERT_EQ(expected_lens[i], written);

        varint_decode_result_t result = varint2_decode(buf, 0);
        ASSERT_EQ(val, result.value);
        ASSERT_EQ(expected_lens[i], result.bytes_read);
    }

    PASS();
}

static void test_varint2_overflow(void)
{
    TEST("varint2 overflow detection");

    uint8_t buf[4] = {0};
    /* Values exceeding MAX (1073741823) should return 0 */
    ASSERT_EQ(0, varint2_encode_len(1073741824));
    ASSERT_EQ(0, varint2_encode_len(0xFFFFFFFF));
    ASSERT_EQ(0, varint2_encode(1073741824, buf, 0));
    ASSERT_EQ(0, varint2_encode(0xFFFFFFFF, buf, 0));

    PASS();
}

static void test_varint2_bit_patterns(void)
{
    TEST("varint2 bit patterns");

    uint8_t buf[4] = {0};
    /* Powers of 2 */
    uint32_t powers[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024,
                         2048, 4096, 8192, 16384, 32768, 65536,
                         131072, 262144, 524288, 1048576, 2097152,
                         4194304, 8388608, 16777216, 33554432,
                         67108864, 134217728, 268435456, 536870912};

    for (size_t i = 0; i < sizeof(powers)/sizeof(powers[0]); i++) {
        uint32_t val = powers[i];
        if (val > 1073741823) continue;  /* skip overflow */

        size_t written = varint2_encode(val, buf, 0);
        ASSERT_EQ(varint2_encode_len(val), written);

        varint_decode_result_t result = varint2_decode(buf, 0);
        ASSERT_EQ(val, result.value);
    }

    PASS();
}

static void test_varint2_all_ones(void)
{
    TEST("varint2 all-ones patterns");

    uint8_t buf[4] = {0};
    /* All bits set within each range */
    uint32_t patterns[] = {
        0x3F,       /* 6 bits: 63 (1-byte max) */
        0x3FFF,     /* 14 bits: 16383 (2-byte max) */
        0x3FFFFF,   /* 22 bits: 4194303 (3-byte max) */
        0x3FFFFFFF  /* 30 bits: 1073741823 (4-byte max) */
    };
    size_t expected_lens[] = {1, 2, 3, 4};

    for (size_t i = 0; i < sizeof(patterns)/sizeof(patterns[0]); i++) {
        uint32_t val = patterns[i];
        size_t written = varint2_encode(val, buf, 0);
        ASSERT_EQ(expected_lens[i], written);

        varint_decode_result_t result = varint2_decode(buf, 0);
        ASSERT_EQ(val, result.value);
    }

    PASS();
}

/* ============================================================================
 * Edge case tests - varint1
 * ============================================================================ */

static void test_varint1_boundaries(void)
{
    TEST("varint1 all boundary values");

    uint8_t buf[2] = {0};
    uint16_t boundaries[] = {
        0,              /* min */
        1,
        126, 127,       /* 1-byte max-1, max */
        128, 129,       /* 2-byte min, min+1 */
        32766, 32767    /* 2-byte max-1, max (absolute max) */
    };
    size_t expected_lens[] = {
        1, 1, 1, 1,
        2, 2, 2, 2
    };

    for (size_t i = 0; i < sizeof(boundaries)/sizeof(boundaries[0]); i++) {
        uint16_t val = boundaries[i];
        size_t written = varint1_encode(val, buf, 0);
        ASSERT_EQ(expected_lens[i], written);

        varint_decode_result_t result = varint1_decode(buf, 0);
        ASSERT_EQ(val, result.value);
        ASSERT_EQ(expected_lens[i], result.bytes_read);
    }

    PASS();
}

static void test_varint1_overflow(void)
{
    TEST("varint1 overflow detection");

    uint8_t buf[2] = {0};
    /* Values exceeding MAX (32767) should return 0 */
    ASSERT_EQ(0, varint1_encode_len(32768));
    ASSERT_EQ(0, varint1_encode_len(0xFFFF));
    ASSERT_EQ(0, varint1_encode(32768, buf, 0));
    ASSERT_EQ(0, varint1_encode(0xFFFF, buf, 0));

    PASS();
}

static void test_varint1_bit_patterns(void)
{
    TEST("varint1 bit patterns");

    uint8_t buf[2] = {0};
    /* Powers of 2 */
    uint16_t powers[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512,
                         1024, 2048, 4096, 8192, 16384};

    for (size_t i = 0; i < sizeof(powers)/sizeof(powers[0]); i++) {
        uint16_t val = powers[i];
        if (val > 32767) continue;

        size_t written = varint1_encode(val, buf, 0);
        ASSERT_EQ(varint1_encode_len(val), written);

        varint_decode_result_t result = varint1_decode(buf, 0);
        ASSERT_EQ(val, result.value);
    }

    PASS();
}

static void test_varint1_all_ones(void)
{
    TEST("varint1 all-ones patterns");

    uint8_t buf[2] = {0};
    uint16_t patterns[] = {
        0x7F,   /* 7 bits: 127 (1-byte max) */
        0x7FFF  /* 15 bits: 32767 (2-byte max) */
    };
    size_t expected_lens[] = {1, 2};

    for (size_t i = 0; i < sizeof(patterns)/sizeof(patterns[0]); i++) {
        uint16_t val = patterns[i];
        size_t written = varint1_encode(val, buf, 0);
        ASSERT_EQ(expected_lens[i], written);

        varint_decode_result_t result = varint1_decode(buf, 0);
        ASSERT_EQ(val, result.value);
    }

    PASS();
}

/* ============================================================================
 * decode_len verification
 * ============================================================================ */

static void test_decode_len_verification(void)
{
    TEST("decode_len matches actual decode");

    uint8_t buf[4] = {0};

    /* Test varint2 */
    uint32_t vals2[] = {0, 63, 64, 16383, 16384, 4194303, 4194304, 1073741823};
    for (size_t i = 0; i < sizeof(vals2)/sizeof(vals2[0]); i++) {
        varint2_encode(vals2[i], buf, 0);
        size_t peek_len = varint2_decode_len(buf, 0);
        varint_decode_result_t result = varint2_decode(buf, 0);
        ASSERT_EQ(peek_len, result.bytes_read);
    }

    /* Test varint1 */
    uint16_t vals1[] = {0, 127, 128, 32767};
    for (size_t i = 0; i < sizeof(vals1)/sizeof(vals1[0]); i++) {
        varint1_encode(vals1[i], buf, 0);
        size_t peek_len = varint1_decode_len(buf, 0);
        varint_decode_result_t result = varint1_decode(buf, 0);
        ASSERT_EQ(peek_len, result.bytes_read);
    }

    PASS();
}

/* ============================================================================
 * Sequential decode tests
 * ============================================================================ */

static void test_sequential_decode(void)
{
    TEST("sequential decode multiple values");

    uint8_t buf[16] = {0};
    size_t offset = 0;

    /* Encode multiple values */
    offset += varint2_encode(42, buf, offset);       /* 1 byte */
    offset += varint2_encode(1000, buf, offset);     /* 2 bytes */
    offset += varint2_encode(100000, buf, offset);   /* 3 bytes */
    offset += varint2_encode(10000000, buf, offset); /* 4 bytes */

    ASSERT_EQ(10, offset);  /* 1 + 2 + 3 + 4 */

    /* Decode sequentially */
    size_t pos = 0;

    varint_decode_result_t r1 = varint2_decode(buf, pos);
    ASSERT_EQ(42, r1.value);
    pos += r1.bytes_read;

    varint_decode_result_t r2 = varint2_decode(buf, pos);
    ASSERT_EQ(1000, r2.value);
    pos += r2.bytes_read;

    varint_decode_result_t r3 = varint2_decode(buf, pos);
    ASSERT_EQ(100000, r3.value);
    pos += r3.bytes_read;

    varint_decode_result_t r4 = varint2_decode(buf, pos);
    ASSERT_EQ(10000000, r4.value);
    pos += r4.bytes_read;

    ASSERT_EQ(10, pos);

    PASS();
}

/* ============================================================================
 * Main
 * ============================================================================ */

int main(void)
{
    printf("Running varint tests...\n\n");

    printf("[2-bit header tests]\n");
    test_varint2_encode_len();
    test_varint2_roundtrip_1byte();
    test_varint2_roundtrip_2byte();
    test_varint2_roundtrip_3byte();
    test_varint2_roundtrip_4byte();
    test_varint2_offset();

    printf("\n[1-bit header tests]\n");
    test_varint1_encode_len();
    test_varint1_roundtrip_1byte();
    test_varint1_roundtrip_2byte();
    test_varint1_offset();

    printf("\n[Edge cases - varint2]\n");
    test_varint2_boundaries();
    test_varint2_overflow();
    test_varint2_bit_patterns();
    test_varint2_all_ones();

    printf("\n[Edge cases - varint1]\n");
    test_varint1_boundaries();
    test_varint1_overflow();
    test_varint1_bit_patterns();
    test_varint1_all_ones();

    printf("\n[Integration tests]\n");
    test_decode_len_verification();
    test_sequential_decode();

    printf("\n========================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_run);
    printf("========================================\n");

    return (tests_passed == tests_run) ? 0 : 1;
}
