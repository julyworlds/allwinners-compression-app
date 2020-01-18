# allwinners-compression-app
Application to compress / decompress files for allwinners filesystem.

### About this project
Chinese allwinners sdk uses a custom filesystem, this file system uses a also custom compression method. This can be found in some chinese tablets or even wince car radios.

The purpose of this app is to compress or decompress those files in the filesystem, so it can be readed and writed. With the possibility of modifying existing compressed files.

### How it works
Compressed files in this filesystem use the header `AZ10`. It is basically a lzma compression with a strip header version.

How to use:
- compress: AZ10util.exe c uncompressedFile.bin compressedFile.az
- decompress: AZ10util.exe d compressedFile.az uncompressedFile.bin
