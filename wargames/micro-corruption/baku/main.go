package main

import (
	"encoding/hex"
	"fmt"
)

// FIPS-197 Figure 7. S-box substitution values in hexadecimal format.
var sbox = [256]byte{
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
}

// FIPS-197 Figure 14.  Inverse S-box substitution values in hexadecimal format.
var sboxInv = [256]byte{
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,
}

func xtime(b byte) byte {
	c := uint32(b)
	c <<= 1
	c ^= ((c >> 8) & 1) * 0x1b
	return byte(c)
}

// encrypt
func mixColumnEnc(col []byte) {
	a := col[0]
	b := col[1]
	c := col[2]
	d := col[3]
	e := a ^ b ^ c ^ d

	col[0] ^= e ^ xtime(a^b)
	col[1] ^= e ^ xtime(b^c)
	col[2] ^= e ^ xtime(c^d)
	col[3] ^= e ^ xtime(d^a)
}

// decrypt
func mixColumnInv(col []byte) {
	a := col[0]
	b := col[1]
	c := col[2]
	d := col[3]
	e := a ^ b ^ c ^ d
	z := xtime(e)
	x := e ^ xtime(xtime(z^a^c))
	y := e ^ xtime(xtime(z^b^d))

	col[0] ^= x ^ xtime(a^b)
	col[1] ^= y ^ xtime(b^c)
	col[2] ^= x ^ xtime(c^d)
	col[3] ^= y ^ xtime(d^a)
}

// shift rows function for encryption
func shiftRowsEncrypt(userInput []byte) {
	// Shift first row
	temp := userInput[1]
	userInput[1] = userInput[5]
	userInput[5] = userInput[9]
	userInput[9] = userInput[13]
	userInput[13] = temp

	// Shift second row
	temp = userInput[2]
	userInput[2] = userInput[10]
	userInput[10] = temp

	// Shift third row
	temp = userInput[6]
	userInput[6] = userInput[14]
	userInput[14] = temp

	// Shift fourth row
	temp = userInput[3]
	userInput[3] = userInput[15]
	userInput[15] = userInput[11]
	userInput[11] = userInput[7]
	userInput[7] = temp
}

// shift rows function for decryption
func shiftRowsDecrypt(userInput []byte) {
	// Shift first row
	temp := userInput[13]
	userInput[13] = userInput[9]
	userInput[9] = userInput[5]
	userInput[5] = userInput[1]
	userInput[1] = temp

	// Shift second row
	temp = userInput[2]
	userInput[2] = userInput[10]
	userInput[10] = temp

	// Shift third row
	temp = userInput[6]
	userInput[6] = userInput[14]
	userInput[14] = temp

	// Shift fourth row
	temp = userInput[3]
	userInput[3] = userInput[7]
	userInput[7] = userInput[11]
	userInput[11] = userInput[15]
	userInput[15] = temp
}

// encrypts user input in place
func aesenc128(keyPointer, userInput []byte) {

	// XOR with key
	xor128(keyPointer, userInput)

	// Perform the MixColumns operation
	for i := 0; i < 16; i += 4 {
		mixColumnEnc(userInput[i:])
	}

	// Apply S-box
	for i := range userInput {
		userInput[i] = sbox[userInput[i]]
	}

	// Shift rows
	shiftRowsEncrypt(userInput)

}

// decrypts user input in place
func aesdec128(keyPointer, userInput []byte) {

	// Shift rows
	shiftRowsDecrypt(userInput)

	// Apply inverse S-box
	for i := range userInput {
		userInput[i] = sboxInv[userInput[i]]
	}

	// Perform the inverse MixColumns operation
	for i := 0; i < 16; i += 4 {
		mixColumnInv(userInput[i:])
	}

	// Xor with key
	xor128(keyPointer, userInput)
}

func printsp(sp [12]uint16) {
	for i := 0; i < len(sp); i++ {
		fmt.Printf("%04x ", sp[i])
	}
	fmt.Println()
}

// this is good
func xor128(key_pointer, user_input []byte) {
	counter := 0

	for counter != 0x10 {
		user_input[counter] ^= key_pointer[counter]
		counter = counter + 1
	}
}

// print hex in 4 byte chunks
func printhex(prefix string, data []byte) {
	fmt.Printf("%s: ", prefix)
	for i, b := range data {
		if i > 0 && i%2 == 0 {
			fmt.Printf(" ")
		}
		fmt.Printf("%02x", b)
	}
	fmt.Println()
}

func aes_ecb_encrypt(user_input []byte, key_pointer []byte) []byte {

	xor128(key_pointer[0xa0:], user_input)

	// Apply S-box
	for i := range user_input {
		user_input[i] = sbox[user_input[i]]
	}

	// Shift rows
	shiftRowsEncrypt(user_input)
	printhex("enc shiftrows", user_input)

	// Perform the MixColumns and XOR operations
	for i := 0x90; i >= 0x10; i -= 0x10 {
		// Perform encryption
		aesenc128(key_pointer[i:], user_input)
		printhex("mixcols aesenc128", user_input)
	}

	// XOR with the input
	xor128(key_pointer, user_input)
	printhex("enc xor", user_input)

	return user_input
}

// custom aes ecb decrypt function for microcorruption baku
func aes_ecb_decrypt(user_input []byte, key_pointer []byte) []byte {

	// xor the input
	xor128(key_pointer, user_input)
	printhex("dec xor", user_input)

	// Perform the inverse MixColumns and XOR operations
	for i := 0x10; i <= 0x90; i += 0x10 {
		//printhex(user_input) // print before decryption
		aesdec128(key_pointer[i:], user_input)
		printhex("mixcols aesdec128", user_input)
		//user_input_dec := make([]byte, len(user_input))
		//_ = copy(user_input_dec, user_input)
		//aesenc128(key_pointer[i:], user_input_dec)

		//printhex(user_input_dec) // print after encryption
	}

	// Shift rows
	shiftRowsDecrypt(user_input)
	printhex("dec shiftrows", user_input)

	// Apply inverse S-box
	for i := range user_input {
		user_input[i] = sboxInv[user_input[i]]
	}

	// Xor
	xor128(key_pointer[0xa0:], user_input)
	printhex("dec xor", user_input)

	return user_input
}

// address of the data is 43f0 [read 43f0 16]
// address of the key

// 43f0 : User input, 16 bytes
// 48ae : xor key, 16 bytes
// 48be : aes key, 16 bytes

//
// rsbox https://github.com/bonybrown/tiny-AES128-C/blob/master/aes.c#L101
// at read 495e 16

func main() {

	key_data, _ := hex.DecodeString(
		"7f7875e0c977d30ce85eca19d02211f7" + // 0
			"4b530b31b5cd58d3f59dc5a9c583c4f3" + // 1
			"6f1af5bbfe9e53e240509d7a301e015a" + // 2
			"6259a7399184a659becece98704e9c20" + // 3
			"539345a8f3dd01602f4a68c1ce8052b8" + // 4
			"70076c8ba04e44c8dc9769a1e1ca3a79" + // 5
			"ff47b02ed04928437cd92d693d5d53d8" + // 6
			"d980482f2f0e986dac90052a41847eb1" + // 7
			"7dcd0f8ef68ed042839e9d47ed147b9b" + // 8
			"f2138f148b43dfcc75104d056e8ae6dc" + // 9
			"7b2f0d188af1fa20493cd251f10bbcb5") // 10

	// cleartext: ACCESS GRANTED! -> 415343455353204752414e5445442100

	// Encrypt
	clear_text, _ := hex.DecodeString("414343455353204752414e5445442100")
	fmt.Printf("------------------\n")

	clear_text_cpy := make([]byte, len(clear_text))
	_ = copy(clear_text_cpy, clear_text)

	//input, _ := hex.DecodeString("41414141414141414141414141414141")
	//decrypt := aes_ecb_decrypt(input, key_data)
	printhex("clear text:", clear_text)

	// encrypt
	text_cpy_for_enc := make([]byte, len(clear_text))
	_ = copy(text_cpy_for_enc, clear_text)
	eo := aes_ecb_encrypt(text_cpy_for_enc, key_data)
	printhex("encrypted text:", text_cpy_for_enc)

	fmt.Printf("------------------\n")
	// decrypt
	text_cpy_for_dec := make([]byte, len(clear_text))
	_ = copy(text_cpy_for_dec, text_cpy_for_enc)
	do := aes_ecb_decrypt(text_cpy_for_dec, key_data)
	printhex("decrypted text:", text_cpy_for_dec)
	// Encrypt string called input
	// Take encrypted string, decrypt it
	// see if encrypted string matches input

	// f27379cded095b5fe3a8a6425af42bbb
	// Decrypt

	//o := aes_ecb_decrypt(input, key_data)
	printhex("zzz:", eo)
	printhex("zzz:", do)
	printhex("original clear text:", clear_text_cpy)
}
