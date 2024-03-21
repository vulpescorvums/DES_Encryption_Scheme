#pragma once
#ifndef TABLES_H
#define TABLES_H

// Initial Permutation Table
extern int initial_perm[64];

// Inverse Initial Permutation Table
extern int inverse_initial_perm[64];

// PC-1 Table
extern int PC_1[56];

// PC-2 Table
extern int PC_2[48];

// Number of bit shifts 
extern int shift_table[16];

// Expansion table
extern int E[48];

// S-boxes
extern int S[8][4][16];

// P-box
extern int P[32];

#endif