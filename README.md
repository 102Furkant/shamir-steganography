# PixCrypt

**Split secrets. Hide them in plain sight. Reconstruct with mathematics.**

PixCrypt is a pure C project that combines:

- Shamir's Secret Sharing (SSS)
- LSB Steganography
- Pixel permutation (Fisher–Yates)

to securely split and covertly store images inside other images.

---

## Interface

<img width="700" src="https://github.com/user-attachments/assets/08107dfe-d16b-4c23-8595-7c870d191dfb"/>

PixCrypt provides an interactive CLI where you can:

- Encode using SSS + Steganography
- Use each technique independently
- Decode hidden data
- Calculate PSNR

---

## Example

**Original Image**

<img width="350" src="https://github.com/user-attachments/assets/7c536f6b-faa5-49a2-9e80-bf76192f5efb"/>

**Shares (After Shamir Encoding)**

<img width="250" src="https://github.com/user-attachments/assets/e12bb73c-778c-416b-8ac0-2cae6682fea1"/> <img width="250" src="https://github.com/user-attachments/assets/94c30ecb-5633-4962-84bc-ab281eee105c"/> <img width="250" src="https://github.com/user-attachments/assets/1dbf80d4-69ea-4ff9-a225-d600dbd00bfc"/>

These shares look like random noise and clearly attract attention.
PixCrypt solves this by hiding them inside normal images.

---

## Core Idea

PixCrypt combines two key concepts:

- Divide the secret → Shamir's Secret Sharing
- Hide the result → Steganography

Not only is the data protected, but its existence is concealed.

---

## Full Pipeline

```
[Secret Image]
      ↓
[Pixel Permutation]
      ↓
[Shamir Secret Sharing]
      ↓
[Bit-Level Packing]
      ↓
[LSB Embedding]
      ↓
[Stego Images]
```

## Reconstruction

```
[Stego Images]
      ↓
[Metadata Validation]
      ↓
[Bit Extraction]
      ↓
[Lagrange Interpolation]
      ↓
[Inverse Permutation]
      ↓
[Original Image]
```

---

## Implementation Details

### Pixel Permutation

- Uses Fisher–Yates shuffle
- Removes spatial patterns
- Makes statistical analysis harder

### Shamir's Secret Sharing

Each pixel is encoded as:

```
f(x) = s + a₁x + a₂x² + ... mod 251
```

- `s` = pixel value
- Any K shares reconstruct the image
- Fewer than K shares reveal nothing

### Reconstruction (Lagrange Interpolation)

- Evaluated at x = 0
- Uses modular inverse via Extended Euclidean Algorithm

### Bit-Level Packing (Matrix Transpose)

Instead of embedding bytes directly, data is rearranged at the bit level for efficient and reversible packing.

```c
bits[b] = (bits[b] << 1) | bit_val;
bytes_arr[bit_pos] = (bytes_arr[bit_pos] << 1) | bit;
```

### LSB Steganography

Embeds data into least significant bits with configurable bit depth (1–8).

```c
cover_pixel &= ~mask;
cover_pixel |= data_bits;
```

### Metadata & Validation

PixCrypt embeds metadata into the image:

- width & height
- LSB configuration
- magic numbers

During decoding, if magic numbers don't match, the image is treated as clean (no hidden data).

### Cross-Platform Support

PixCrypt works on both Linux and Windows. Platform-specific behavior such as
directory creation, screen clearing, and path separators are handled via
conditional macros:

```c
#ifdef _WIN32
    #include <direct.h>
    #define make_dir(name) _mkdir(name);
    #define CLEAR_SCREEN system("cls");
    #define PATH_SEP '\\'
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define make_dir(name) mkdir(name, 0777);
    #define CLEAR_SCREEN system("clear");
    #define PATH_SEP '/'
#endif
```

---

## Features

- Shamir Secret Sharing (K-of-N)
- LSB Steganography (configurable)
- Pixel permutation (Fisher–Yates)
- Bit-level data transformation
- Metadata validation (magic numbers)
- Cross-platform (Linux / Windows)
- Multi-format image support via stb_image
- PSNR calculation
- Modular design (combined or separate modes)
- Written entirely in pure C

---

## Build

```bash
git clone https://github.com/102Furkant/shamir-steganography
cd shamir-steganography
make
```

---

## Usage

```bash
./pixcrypt
```

```
1. Encode using Steganography + Shamir's Secret Sharing
2. Encode using only Steganography
3. Encode using only Shamir's Secret Sharing
4. Decode from Steganography + Shamir's Secret Sharing
5. Decode from Steganography Only
6. Decode from Shamir's Secret Sharing Only
7. Calculate PSNR value
8. Exit
```

---

## PSNR (Quality Measurement)

PixCrypt calculates PSNR (Peak Signal-to-Noise Ratio):

- Measures distortion caused by embedding
- Higher PSNR → better visual quality

---

## Limitations

- Maximum K threshold: 7, Maximum N shares: 20
- Large secrets require sufficiently large cover images
- High LSB values may introduce visible distortion
- Only PNG output is supported for shares and decoded images

---

## Future Work

- CLI argument support
- Seed generation from file hash instead of fixed value
- Embedding generated seed into the image or its metadata
- Stronger randomness
- Advanced steganography techniques

---

## Security Notes

- Requires correct K shares for reconstruction
- Shares alone do not reveal the original image
- Hidden inside natural images for low visibility

---

## Dependencies

- [stb_image / stb_image_write](https://github.com/nothings/stb) (included)

---

## License

MIT License — see [LICENSE](LICENSE)

---

## Author

[102Furkant](https://github.com/102Furkant)

---

*PixCrypt is a combination of low-level programming, applied cryptography, and data hiding techniques — built in pure C.*
