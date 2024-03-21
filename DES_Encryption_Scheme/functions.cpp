#include <bitset>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "tables.h"

using namespace std;

// Function to do the initial permutation
bitset<64> initial_permutation(bitset<64>& plaintext) {
    bitset<64> permuted_pt;
    for (int i = 0; i < 64; i++) {
        permuted_pt[63 - i] = plaintext[64 - initial_perm[i]];
    }
    return permuted_pt;
}

// Function to do the inverse initial permutation
bitset<64> inverse_initial_permutation(bitset<64>& plaintext) {
    bitset<64> permuted_pt;
    for (int i = 0; i < 64; i++) {
        permuted_pt[63 - i] = plaintext[64 - inverse_initial_perm[i]];
    }
    return permuted_pt;
}

// Function to generate the subkeys
vector<bitset<48>> generate_subkeys(bitset<64>& key) {
    bitset<56> key56;
    vector<bitset<48>> subkeys(16);

    // Parity drop PC-1
    for (int i = 0; i < 56; i++) {
        key56[55 - i] = key[64 - PC_1[i]];
    }

    // Split into two halves
    bitset<28> left = key56.to_ullong() >> 28;
    bitset<28> right = key56.to_ullong() & 0x0FFFFFFF;

    // Generate 16 subkeys
    for (int round = 0; round < 16; round++) {
        // Left circular shift
        left = (left << shift_table[round]) | (left >> (28 - shift_table[round]));
        right = (right << shift_table[round]) | (right >> (28 - shift_table[round]));

        // Combine halves and compress to 48 bits PC-2
        bitset<56> combined = (left.to_ullong() << 28) | right.to_ullong();
        for (int i = 0; i < 48; i++) {
            subkeys[round][47 - i] = combined[56 - PC_2[i]];
        }
    }

    // Prints out all subkeys generated
    for (int i = 0; i < 16; i++)
    {
        cout << "\n" << "key " << i << ":\t" << subkeys[i];
    }

    return subkeys;

}

// Feistel function
bitset<32> Feistel(bitset<32>& R, bitset<48>& key) {
    // Expansion
    bitset<48> expanded_R;
    for (int i = 0; i < 48; i++) {
        expanded_R[47 - i] = R[32 - E[i]];
    }

    // Key mixing with XOR
    expanded_R ^= key;

    // Substitution
    bitset<32> substituted_R;
    for (int i = 0; i < 8; i++) {
        int row = expanded_R[6 * i] * 2 + expanded_R[6 * i + 5];
        int col = expanded_R[6 * i + 1] * 8 + expanded_R[6 * i + 2] * 4 + expanded_R[6 * i + 3] * 2 + expanded_R[6 * i + 4];
        int val = S[i][row][col];
        bitset<4> binary_val(val);

        // Convert the bitset to an unsigned long long, perform the operation, and then convert it back to a bitset.
        substituted_R = bitset<32>(substituted_R.to_ullong() | (binary_val.to_ullong() << (28 - 4 * i)));
    }

    // Permutation
    bitset<32> permuted_R;
    for (int i = 0; i < 32; i++) {
        permuted_R[31 - i] = substituted_R[32 - P[i]];
    }

    return permuted_R;

}

// Function to do the DES Decryption
bitset<64> DES(bitset<64>& plaintext, bitset<64>& key) {

    // Initial permutation
    bitset<64> permuted_pt = initial_permutation(plaintext);

    // Generate subkeys
    vector<bitset<48>> subkeys = generate_subkeys(key);

    // Split into two halves
    bitset<32> left = permuted_pt.to_ullong() >> 32;
    bitset<32> right = permuted_pt.to_ullong() & 0xFFFFFFFF;

    // 16 rounds of DES
    for (int round = 0; round < 16; round++) {
        bitset<32> old_right = right;  // Save the old right half
        right = left ^ Feistel(right, subkeys[round]);  // XOR left half with Feistel function output
        left = old_right;  // Swap halves
    }

    // Combine halves
    bitset<64> combined = (left.to_ullong() << 32) | right.to_ullong();

    // Inverse initial permutation
    bitset<64> ciphertext = inverse_initial_permutation(combined);

    // Convert ciphertext binary to hex
    stringstream ss;
    ss << hex << uppercase << ciphertext.to_ullong();
    string hex_ciphertext = ss.str();

    cout << endl << endl << "Ciphertext base 2:\t" << ciphertext << endl;
    cout << "Ciphertext base 16:\t" << hex_ciphertext << endl;

    return ciphertext;


}

// Function to do the DES Decryption, same function as DES just in reverse
bitset<64> DES_decrypt(bitset<64>& ciphertext, bitset<64>& key) {

    // Initial permutation
    bitset<64> permuted_ct = initial_permutation(ciphertext);

    // Generate subkeys
    vector<bitset<48>> subkeys = generate_subkeys(key);

    // Split into two halves
    bitset<32> left = permuted_ct.to_ullong() >> 32;
    bitset<32> right = permuted_ct.to_ullong() & 0xFFFFFFFF;

    // 16 rounds of DES, but with subkeys applied in reverse order
    //swaps are also reversed because the s-boxes are not reversible
    for (int round = 15; round >= 0; round--) {
        bitset<32> old_left = left;  // Save the old right half
        left = right ^ Feistel(left, subkeys[round]);  // XOR left half with Feistel function output
        right = old_left;  // Swap halves
    }

    // Combine halves
    bitset<64> combined = (left.to_ullong() << 32) | right.to_ullong();

    // Inverse initial permutation
    bitset<64> plaintext = inverse_initial_permutation(combined);

    // Convert plaintext binary to hex
    stringstream ss;
    ss << hex << uppercase << plaintext.to_ullong();
    string hex_plaintext = ss.str();

    cout << endl << endl << "Plaintext base 2:\t" << plaintext << endl;
    cout << "Plaintext base 16:\t" << hex_plaintext << endl;

    return plaintext;
}