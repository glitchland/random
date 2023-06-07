TODO: check if byte pair orderings need to be swapped

```text
800000063041c26436953f8f3cadf1442fc218b185051ab6c20853a45f093fc32a
df31529d05a5ec3e96a9e41ed9ad1b14dcbdb98e50e37a7ddc3d595b867807ed16
05f2070e
```

```
800000063041c26436953f8f3cadf1442fc218b185051ab6c20853a45f093fc32adf31529d05a5ec3e96a9e41ed9ad1b14dcbdb98e50e37a7ddc3d595b867807ed1605f2070e
```


Load address outside allowed range of 0x8000-0xF000



Example test input:
8081010662636465666768696A6B6C6D6E6F707172737475767778797A4142434445464748494A4B4C4D4E4F505152535455565758595A00

[0..1][2][3][data]
  |     \  \- byte 4 is some sort of offset byte
  |      byte 3 is verifier type
   \- first 2 bytes load address

[8000]
[01] verifier type 1 == sha512, everything else is ed25519
[04] payload len? 0x06-0xff (underflow and error when < 0x06)
     so it copies everything after that len as the sig to check?

### Len check on payload length/offset byte 

```c
offset = offset - 6; // if < 6 this wraps to ~0xffff
if (offset >= 0x3bb) {
    goto error;
}
```

// copy the "signature" to check
memcpy(user_input+payloadlen,stack_buffer,0x40)

// pass the offset, and the pointer to the user input to the sha512 function
r14 == payloadlen 
r15 == start of user input (#0x2420)
call	#0x4566 <sha512>


In the call to SHA512, set the syscall id so that it calls sha1 and sha256 instead of sha512

Test input: 
```
8081010662636465666768696A6B6C6D6E6F707172737475767778797A4142434445464748494A4B4C4D4E4F505152535455565758595A414141414141414141414141414141
```

```c
let 43b6 = 30 // sha1
let 43b8 = 31 // sha256
let 43ba = 32 // sha512
let 43ba = 33 // ed25519
```

```
```

```
```

```
defghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZAAAAAAAAAAAAAAA
```

```c
8081010662634cf95d6ac044341779df9e8fb37b9b0704b7f97d7a094a066bba44381929743e08e274dafb4cd400093a8cac7554533169df5cb94f6a44d1955cb206bcb936b9
```

```c

printf "\x80\x81\x01\x06\x62\x63" | sha1sum

// sha1 (20 bytes)
5c92 ce95 0ec2 9085 ab03 0add 6816 bc58 537e 8fb9

// sha256

// sha512

```

compare the hashed output, to the user supplied input hash at specified offset 
with constant 0x40 length

if it all matches, then pop the 8081 off the stack into r11
but then it checks if r15 is 1, and if it is not, it throws an error
so inside of the memcmp, we need to exploit this compare:
```
4608:  4f4b           mov.b	r11, r15
460a:  4e4c           mov.b	r12, r14
460c:  0f8e           sub	r14, r15
```
so that r15 == 1

so for example, if the compared byte is 4c then we set the input to 4b.
```

```
808101069c9a1cb8afef1eb9a8ca0a95ea154d1063ff545e5270ef3868690c150b4ea55a9c59ae1593c532f21b56e16a21001fadaabafa1a04c224e7b0007b25b015c5a2
```


hashes N bytes from the start of the user input


8081010a62634cf95d6ac044341779df9e8fb37b9b0704b7f97d7a094a066bba44381929743e08e274dafb4cd400093a8cac7554533169df5cb94f6a44d1955cb206bcb936b9

```
r13 == copied payload buffer 
r14 == payloadlen 
r15 == start of user input (#0x2420)
call	#0x4566 <sha512>
```

then copies this into the buffer at 8081, and jumps to it to execute it

```

// set r15 to syscall to open lock
// jmp to INT 

// Calling interrupt 0x7f will do the trick.
push #0x007f
call #0x452a 

30127f00b0122a45
let ff80 = 1230
let ff82 = 007f
let ff84 = 12b0
let ff86 = 452a

let pc = ff80

8000016162636465666768696A6B6C6D6E6F707172737475767778797A4142434445464748494A4B4C4D4E4F505152535455565758595A




// the payload
80000106000030127f00b0122a45

f000010e0b9b30127f00b0122a45036D6E6F707172737475767778797A4142434445464748494A4B4C4D4E4F505152535455565758595A

4cf95d6ac044341779df9e8fb37b9b0704b7f97d7a094a066bba44381929743e08e274dafb4cd400093a8cac7554533169df5cb94f6a44d1955cb206bcb936b9

//
let f000 = 9090
let f002 = 1230
let f004 = 007f
let f006 = 12b0
let f008 = 452a

let pc = f000










Read N from byte 4 and add to base:
44aa:  084a           mov	r10, r8
44ac:  3850 2024      add	#0x2420, r8

Copy 0x40 bytes from base + offset to stack buffer.

```
44c0:  0d41           mov	sp, r13            // output
44c2:  0e4a           mov	r10, r14           // size? offset? specified by byte 4
44c4:  3f40 2024      mov	#0x2420, r15       // pointer to input
44c8:  b012 6645      call	#0x4566 <sha512>
```
makes a sha512 of the full input and saves it to sp 

then compares 

r13 == 0x40 (64, size of a sha512 hash)
r14 == ?? looks like a pointer to some memory, maybe controlled by the offset on the input?
r15 == the sha512 hash of the input

// copy 64 bytes of input data, from input + offset, into a buffer
```
44cc:  3d40 4000      mov	#0x40, r13      // 64 bytes
44d0:  0e48           mov	r8, r14         // from base of input + offset 
44d2:  0f41           mov	sp, r15         // output is buffer on stack
44d4:  b012 fa45      call	#0x45fa <memcmp>
```

``` 64 bytes
242f 6d6e 6f70 7172 7374 7576 7778 797a 4142  mnopqrstuvwxyzAB
243f 4344 4546 4748 494a 4b4c 4d4e 4f50 5152  CDEFGHIJKLMNOPQR
244f 5354 5556 5758 595a 0000 0000 0000 0000  STUVWXYZ........
245f 0000 0000 0000 0000 0000 0000 0000 0000  ................
```




Ideas : 
 1. null byte in the middle of the input
 2. maybe memory is not reset between runs, so we can use the previous run to set the hash to something we want

```
44aa:  084a           mov	r10, r8     // 8 is an offset (this is also used later for the sha512)
44ac:  3850 2024      add	#0x2420, r8 // 2420 is the start of the input
```


8000010F62636465666768696A6B6C6D6E6F707172737475767778797A4142434445464748494A4B4C4D4E4F505152535455565758595A00


printf "\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x35\x55\x56\x57\x58\x59\x5a\x00" | sha512sum


[8000][01][0F]62636465666768696A6B6C6D6E6F707172737475767778797A4142434445464748494A4B4C4D4E4F505152535455565758595A00

[8000]
[01] verifier type 1 == sha512, 
[04] payload len 0x06-0xff (weird behavior when lower)

8000010662636465666768696A6B6C6D6E6F707172737475767778797A4142434445464748494A4B4C4D4E4F505152535455565758595A00


memcpy(user_input+payloadlen,stack_buffer,0x40)

```
r13 == copied payload buffer 
r14 == payloadlen 
r15 == start of user input (#0x2420)
call	#0x4566 <sha512>
```


Before SHA512 :
```
43c0: 6465 6667 6869 6a6b 6c6d 6e6f 7071 7273   defghijklmnopqrs
43d0: 7475 7677 7879 7a41 4243 4445 4647 4849   tuvwxyzABCDEFGHI
43e0: 4a4b 4c4d 4e4f 5051 5253 5455 5657 5859   JKLMNOPQRSTUVWXY
43f0: 5a00 0000 0000 0000 0000 0000 0000 0000   Z...............
4400: 5542 5c01 35d0 085a 8245 2028 3140 0044   UB\.5..Z.E (1@.D
-----
```

After SHA512 :
```
43b0: 3e45 0300 7445 3200 2024 0600 c043 cc44   >E..tE2. $...C.D
43c0: cf9f 8971 c4a8 7291 0bf5 8cb0 a972 e4e0   ...q..r......r..
43d0: e500 eb0f 01fc b23f 4512 9987 c6ee 7b51   .......?E.....{Q
43e0: 983c 0d88 d22a dcd7 3ddb 9e24 9738 9d87   .<...*..=..$.8..
43f0: f7b1 eb74 19f2 2c1d 926b e534 c027 e628   ...t..,..k.4.'.(
4400: 5542 5c01 35d0 085a 8245 2028 3140 0044   UB\.5..Z.E (1@.D
-----
```

So the length doesnt truncate the output, I'm not sure what it 
is used for in the SHA512 function. There is a sha1, sha256, and 
sha512 function in the firmware, but only sha512 is used. Perhaps
this is a hint that there is a bug in the sha512 function that is
also present in the other functions.

4542 <sha1>
4554 <sha256>
4566 <sha512>

```
let 44c8 = b0 
let 44c9 = 12
let 44ca = 66
let 44cb = 45

//sha1
let 44c8 = b0 
let 44c9 = 12
let 44ca = 42
let 44cb = 45

```