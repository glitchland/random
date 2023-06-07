package main

import (
	"encoding/hex"
	"testing"

	"github.com/stretchr/testify/assert"
)

func assertEqual(t *testing.T, expected, actual interface{}, message string) {
	if expected != actual {
		t.Errorf("%s: Expected: %v, Actual: %v", message, expected, actual)
	}
}

func TestXor128(t *testing.T) {
	// Excrypt With Key
	key, _ := hex.DecodeString("415343455353204752414e5445442100")
	text, _ := hex.DecodeString("415343455353204752414e5445442100")
	textCopy := make([]byte, len(text))
	copy(textCopy, text)

	xor128(key, text)
	xor128(key, text)

	assert.Equal(t, textCopy, text, "XOR128 should be reversible")
}

func TestMixColReverse(t *testing.T) {
	data := []byte{0x63, 0x53, 0xe0, 0x8c}
	dataCopy := make([]byte, len(data))
	copy(dataCopy, data)

	mixColumnEnc(data)
	mixColumnInv(data)

	assert.Equal(t, dataCopy, data, "MixColumn should be reversible")
}

func TestShiftRowsReversable(t *testing.T) {
	data := []byte{0x63, 0x53, 0xe0, 0x8c, 0x09, 0x60, 0xe1, 0x04, 0xcd, 0x70, 0xb7, 0x51, 0xba, 0xca, 0xd0, 0xe7}
	dataCopy := make([]byte, len(data))
	copy(dataCopy, data)

	shiftRowsEncrypt(data)
	shiftRowsDecrypt(data)

	assert.Equal(t, dataCopy, data, "ShiftRows should be reversible")
}

func TestAesReversable(t *testing.T) {
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

	text, _ := hex.DecodeString("414343455353204752414e5445442100")
	textCopy := make([]byte, len(text))
	copy(textCopy, text)

	aes_ecb_encrypt(text, key_data)
	aes_ecb_decrypt(text, key_data)

	assert.Equal(t, textCopy, text, "AES should be reversible")
}

func TestAesReversableCBC(t *testing.T) {
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

	text, _ := hex.DecodeString("414343455353204752414e5445442100")
	textCopy := make([]byte, len(text))
	copy(textCopy, text)

	_ = aes_ecb_encrypt(text, key_data)
	t.Logf("%x\n", text)
	_ = aes_ecb_decrypt(text, key_data)
	t.Logf("%x\n", text)

	assert.Equal(t, textCopy, text, "AES ECB should be reversible")
}
