#include "functions.h"
#include "tables.h"
#include <iostream>
#include <bitset>
#include <string>
#include <regex>

using namespace std;

int main() {

	string user_input_pt, user_input_key;
	bitset<64> plaintext, key; // **Comment out to run test code**
	regex hex_pattern("^[0-9a-fA-F]{16}$");  // Regular expression to match 16 digit hexadecimal number

	// **Comment out while loop to run test code**
	while (true) {
		cout << "Please enter a 16 digit hexadecimal plaintext (without the 0x prefix): ";
		cin >> user_input_pt;

		cout << "Please enter a 16 digit hexadecimal key (without the 0x prefix): ";
		cin >> user_input_key;

		// Check if the input is a valid 16 digit hexadecimal number
		if (!regex_match(user_input_pt, hex_pattern) || !regex_match(user_input_key, hex_pattern)) {
			cout << "Error: Input must be a 16 digit hexadecimal number." << endl;
			continue;
		}

		// Prepend "0x" to the start of the string
		user_input_pt = "0x" + user_input_pt;
		user_input_key = "0x" + user_input_key;

		try {
			// Converts hex string to unsigned long long integer binary bitset
			plaintext = bitset<64>(stoull(user_input_pt, nullptr, 16));
			key = bitset<64>(stoull(user_input_key, nullptr, 16));
			break;  // If the conversion is successful, break the loop
		}
		catch (const invalid_argument& ia) {
			cout << "Error: Invalid hexadecimal number." << endl;
		}
		catch (const out_of_range& oor) {
			cout << "Error: Number out of range." << endl;
		}
	}

	//bitset<64> original_plaintext = bitset<64>(0xABCDE6ABCD132536); // **un-comment to test code**
	//bitset<64> plaintext = bitset<64>(0xABCDE6ABCD132536); // **un-comment to test code**
	//bitset<64> key = bitset<64>(0xAABB09182736CCDD); // **un-comment to test code**

	bitset<64> ciphertext = DES(plaintext, key);
	bitset<64> decrypted_plaintext = DES_decrypt(ciphertext, key);

	// **un-comment test code below to check if the encryption and decryption are working**
	/*if (original_plaintext == decrypted_plaintext) {
		cout << "Decryption successful: original and decrypted plaintexts match." << endl;
	}
	else {
		cout << "Decryption failed: original and decrypted plaintexts do not match." << endl;
	}*/

	return 0;
}