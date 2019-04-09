# How to generate a hexdump

Following command generates a hexdump file from a source file.
For example in the src/staticweb.h file. I have zipped the iro.js file and make a hexdump out of it. After that i can provide the resulting data in the header file.

> const uint8_t iro_js_gz[] PROGMEM = {
  0x1f, 0x8b, 0x08, 0x08, 0x31, 0x12, 0x4b, 0x5c, 0x00, 0x03, 0x69, 0x72,...

#### Under linux:

```
xxd -i file.ext
```
