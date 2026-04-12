# Fisher-Yates Shuffle Algorithm

## Overview

Starting from the end of the array and moving backward, each element is swapped with a randomly chosen element from the unshuffled portion (including itself). This guarantees that every permutation is equally likely.

The implementation consists of four steps:

1. Generate a random permutation (`generate_permutation`)
2. Apply the permutation to the array (`apply_permutation`)
3. Compute the inverse of the permutation (`invert_permutation`)
4. Restore the original array using the inverse permutation (`reverse_permutation`)

---

## Step-by-Step Example

### Initial State

```
input = (10, 11, 12, 13)
```

---

### 1. generate_permutation

Start with a sorted index array:

```
perm = (0, 1, 2, 3)
```

After random shuffling:

```
perm = (1, 2, 3, 0)
```

---

### 2. apply_permutation

Apply the rule `shuffled[i] = input[perm[i]]`:

```
shuffled[0] = input[perm[0]] = input[1] = 11
shuffled[1] = input[perm[1]] = input[2] = 12
shuffled[2] = input[perm[2]] = input[3] = 13
shuffled[3] = input[perm[3]] = input[0] = 10
```

```
shuffled = (11, 12, 13, 10)
```

---

### 3. invert_permutation

Apply the rule `inv_perm[perm[i]] = i`:

```
inv_perm[perm[0]] = 0  →  inv_perm[1] = 0
inv_perm[perm[1]] = 1  →  inv_perm[2] = 1
inv_perm[perm[2]] = 2  →  inv_perm[3] = 2
inv_perm[perm[3]] = 3  →  inv_perm[0] = 3
```

```
inv_perm = (3, 0, 1, 2)
```

> If `perm[i] = j`, then `inv_perm[j] = i`.
> This records: "the element at position j in shuffled came from position i in input."

---

### 4. reverse_permutation

Apply the rule `original[i] = shuffled[inv_perm[i]]`:

```
original[0] = shuffled[inv_perm[0]] = shuffled[3] = 10
original[1] = shuffled[inv_perm[1]] = shuffled[0] = 11
original[2] = shuffled[inv_perm[2]] = shuffled[1] = 12
original[3] = shuffled[inv_perm[3]] = shuffled[2] = 13
```

```
original = (10, 11, 12, 13)  ✅
```

---

## Note: Involution

If the generated permutation happens to be its own inverse, `perm` and `inv_perm` will be identical. For example:

```
perm     = (2, 3, 0, 1)
inv_perm = (2, 3, 0, 1)  ← same
```

This is not a bug. It means the permutation maps every element back to itself when applied twice. In mathematics, this is called an **involution**.
