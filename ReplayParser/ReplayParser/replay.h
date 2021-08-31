#pragma once
#include <cstdio>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include "../include/zlib/zlib.h"

using namespace std;

#define CHUNK 9216 // 9 KB

struct BlockHeader
{
    uint32_t c_size;
    uint32_t u_size;
    uint16_t check1;
    uint16_t check2;

    void compute_checkshum(BYTE* block);
};

struct W3GHeader
{
    char intro[28];
    uint32_t header_size;
    uint32_t c_size;
    uint32_t header_v;
    uint32_t u_size;
    uint32_t blocks;
    uint32_t ident;
    uint32_t patch_v;
    uint16_t build_v;
    uint16_t flags;
    uint32_t length;
    uint32_t checksum;

    bool read(FILE* f);
    void compute_checksum();
};

uint16_t xor16(uint32_t x);
string to_low_register(string input);
void real_u_size(BYTE* dataUncompressed, int& size);

bool read_replay(FILE* read, W3GHeader& hdr, BYTE* body_c);
bool write_replay(const char* name, W3GHeader& hdr, BYTE* body_c);
bool decompress_block(BlockHeader& bhdr, BYTE* dataCompressed, BYTE* dataUncompressed);
bool compress_block(BlockHeader& bhdr, BYTE* dataCompressed, BYTE* dataUncompressed);
void getEncodedString(BYTE* buffer, BYTE* block, int& pos, int& length);
void decodeEncStr(BYTE* EncodedString, int& size);
bool modifyEncodedString(BYTE* EncStr, int& length);
void encodeEncStr(BYTE* EncStr, int& length);
bool decompress_and_modify(BYTE* dataCompressed, BYTE* dataUncompressed, W3GHeader& hdr);
bool compress(BYTE* dataCompressed, BYTE* dataUncompressed, W3GHeader& hdr);
bool repack_replay(const char* replay_name, const char* converted_name, uint32_t patch, uint16_t build, bool change_map);