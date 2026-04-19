# PixCrypt v1.0

**PixCrypt** is a command-line tool written in C that combines two powerful techniques — **Shamir's Secret Sharing (SSS)** and **LSB Steganography** — to securely split and hide images.

<img width="700" src="https://github.com/user-attachments/assets/08107dfe-d16b-4c23-8595-7c870d191dfb"/>

---

## What It Does

PixCrypt lets you:
- Split a secret image into **N shares** using Shamir's Secret Sharing, requiring at least **K shares** to reconstruct it
- Hide each share inside an innocent-looking cover image using **LSB steganography**
- Decode the hidden shares back and reconstruct the original image
- Use either technique independently

---

## The Problem with Shamir's Secret Sharing Alone

Shamir's Secret Sharing splits a secret image into N shares. Any K of those shares can reconstruct the original. However, the shares look visually suspicious — they appear as noise, which could raise unwanted attention.

**Original image:**

<img width="350" src="https://github.com/user-attachments/assets/7c536f6b-faa5-49a2-9e80-bf76192f5efb"/>

**Shares after Shamir encoding (N=3, K=2):**

<img width="250" src="https://github.com/user-attachments/assets/e12bb73c-778c-416b-8ac0-2cae6682fea1"/> <img width="250" src="https://github.com/user-attachments/assets/94c30ecb-5633-4962-84bc-ab281eee105c"/> <img width="250" src="https://github.com/user-attachments/assets/1dbf80d4-69ea-4ff9-a225-d600dbd00bfc"/>

It's immediately obvious that something is being hidden. Sending or storing these images would raise suspicion. This is where steganography comes in.

---

## The Solution: Hiding Shares Inside Cover Images

The noisy share images are a dead giveaway. To solve this, PixCrypt takes each share and **embeds it invisibly inside an ordinary cover image** — a landscape photo, a family picture, anything that looks completely normal.

The embedding is done by replacing only the **least significant bits** of the cover image's pixel values with bits from the share. Since the human eye cannot perceive changes in the least significant bits, the cover image looks identical before and after embedding. The statistical noise introduced is below the threshold of human perception.

The result: instead of sending suspicious noise images, you send what appears to be a collection of ordinary photos. No one looking at them would suspect they contain hidden cryptographic shares. Only someone who knows how many shares exist, which images hold them, and what their share indexes are can reconstruct the original secret.

This combination — **threshold secret sharing + steganography** — gives you both cryptographic security and plausible deniability.

---

## How It Works

### Shamir's Secret Sharing
Each pixel byte of the (permuted) secret image is treated as the secret value `s`. A random polynomial of degree `K-1` is constructed with `s` as the constant term:

```
f(x) = s + a1*x + a2*x² + ... + a(K-1)*x^(K-1)  mod 251
```

Each share `i` stores `f(i)` for all pixel bytes. Reconstruction uses Lagrange interpolation at `x = 0`. The finite field GF(251) is used because 251 is the largest prime below 256, keeping values within byte range.

### Pixel Permutation
Before sharing, pixel bytes are shuffled using a Fisher-Yates permutation. This spreads the secret across the image and prevents spatial correlation attacks. The permutation is reversed during decoding.

### LSB Steganography
Each share image is embedded into a cover image by replacing the least significant `m` bits of cover image bytes with bits from the share. For `m=1`, the visual impact on the cover image is imperceptible.

---

## Features

- Shamir's Secret Sharing over GF(251) using Lagrange interpolation
- LSB steganography with configurable bit depth (1–8 bits)
- Pixel permutation using Fisher-Yates shuffle for added diffusion
- Combined encode/decode pipeline
- PSNR calculation to measure image quality loss
- Cross-platform (Linux/Windows)

---

## Building

Requirements: `gcc`, `make`

```bash
git clone https://github.com/102Furkant/shamir-steganography
cd shamir-steganography
make
```

This produces the `pixcrypt` executable.

---

## Usage

```bash
./pixcrypt
```

Then follow the interactive menu:

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

## Example Workflow

**Encoding:**
1. Choose option 3 (SSS only) or 1 (SSS + Steganography)
2. Enter the path to your secret image (e.g. `secret.png`)
3. Enter N (total shares) and K (threshold)
4. For combined mode, provide a cover image and LSB bit count
5. Shares are saved to your chosen output folder

**Decoding:**
1. Choose option 6 (SSS only) or 4 (SSS + Steganography)
2. Provide K share paths and their corresponding share indexes
3. Enter a name for the reconstructed image

---

## Limitations

- Maximum K threshold: 7, Maximum N shares: 20
- Cover image must be large enough to contain the share data
- Only PNG output is supported for shares and decoded images

---

## Dependencies

- [stb_image](https://github.com/nothings/stb) — single-header image loading/writing (included)

---

## License

MIT License — see [LICENSE](LICENSE)

---

*Author: [102Furkant](https://github.com/102Furkant)*
