#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <vector>
#include <bitset>
using namespace std;

// Function to do the DES Encryption
bitset<64> DES(bitset<64>& plaintext, bitset<64>& key);

// Function to do the DES Decryption, same function as DES just in reverse
bitset<64> DES_decrypt(bitset<64>& ciphertext, bitset<64>& key);

// Function to generate the subkeys
vector<bitset<48>>generate_subkeys(bitset<64>& key);

// Function to do the initial permutation
bitset<64> initial_permutation(bitset<64>& plaintext);

// Function to do the inverse initial permutation
bitset<64> inverse_initial_permutation(bitset<64>& plaintext);

// Function to do the expansion
bitset<32> Feistel(bitset<32>& R, bitset<48>& key);

#endif // !