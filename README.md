# varint: Variable-Length Integer Encoding Library

`varint` is a C99 header-only library for compact variable-length integer encoding. It provides efficient encoding schemes that use fewer bytes for smaller values, ideal for network protocols, file formats, and data serialization.

## Encoding Formats

The library provides two encoding formats with different trade-offs:

### 1. varint2 (2-bit header)

Uses the upper 2 bits of the first byte to encode the length (1-4 bytes).

| Bytes | Range | Max Value |
|-------|-------|-----------|
| 1 | 0 - 63 | 63 |
| 2 | 64 - 16,383 | 16,383 |
| 3 | 16,384 - 4,194,303 | 4,194,303 |
| 4 | 4,194,304 - 1,073,741,823 | 1,073,741,823 |

### 2. varint1 (1-bit header)

Uses the upper 1 bit of the first byte to encode the length (1-2 bytes).

| Bytes | Range | Max Value |
|-------|-------|-----------|
| 1 | 0 - 127 | 127 |
| 2 | 128 - 32,767 | 32,767 |

## Key Features

- **Header-Only**: Single file, just include `varint.h`
- **C99 Compatible**: No external dependencies
- **Big-Endian**: Network byte order for portability
- **Safe & Unsafe APIs**: Choose between checked and unchecked functions
- **Macro-Based**: Easy to extend with custom formats

## Quick Start

### Installation

Simply include `varint.h` in your project:

```c
#include "varint.h"
```

### Basic Usage

```c
#include "varint.h"
#include <stdio.h>

int main(void) {
    uint8_t buf[4];

    // Encode
    size_t len = varint2_encode(12345, buf, 0);
    printf("Encoded %u in %zu bytes\n", 12345, len);

    // Decode
    varint_decode_result_t result = varint2_decode(buf, 0);
    printf("Decoded: %u (read %u bytes)\n", result.value, result.bytes_read);

    return 0;
}
```

### Sequential Encoding/Decoding

```c
uint8_t buf[16];
size_t offset = 0;

// Encode multiple values
offset += varint2_encode(42, buf, offset);
offset += varint2_encode(1000, buf, offset);
offset += varint2_encode(100000, buf, offset);

// Decode them back
size_t pos = 0;
while (pos < offset) {
    varint_decode_result_t r = varint2_decode(buf, pos);
    printf("Value: %u\n", r.value);
    pos += r.bytes_read;
}
```

## API Reference

### Macros

| Macro | Description |
|-------|-------------|
| `VARINT_MAX_BYTES(h)` | Maximum bytes for h-bit header format |
| `VARINT_NBYTE_MAX(h, n)` | Maximum value for n-byte encoding |
| `VARINT_MAX(h)` | Maximum value for format |

### Functions (generated for varint2 and varint1)

| Function | Description |
|----------|-------------|
| `encode_len(value)` | Returns encoded byte length, or 0 if overflow |
| `__encode_len(value)` | Returns encoded byte length (unchecked) |
| `encode(value, buf, offset)` | Encodes value, returns length or 0 if overflow |
| `__encode(value, buf, offset)` | Encodes value (unchecked) |
| `decode_len(buf, offset)` | Returns byte length from encoded header |
| `decode(buf, offset)` | Decodes and returns `{value, bytes_read}` |

### Return Type

```c
typedef struct {
    uint32_t value;
    uint8_t  bytes_read;
} varint_decode_result_t;
```

## Building Tests

```bash
make test
```

## License

Copyright (C) 2026 Masahito Suzuki. All rights reserved.
