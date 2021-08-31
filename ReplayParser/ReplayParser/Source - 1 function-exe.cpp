#include <cstdio>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include "../include/zlib/zlib.h"

#define CHUNK 9216 // 9 KB

// Thanks shokroot for MapNames list!
const char MapNames[98][50] = {
"0b63ea7e563f02bd0b870bbfd5015a5143e26547",
"(2)ConcealedHill.w3x",
"01f7da7cf4bbef192d8f84a64d3ba29ffee6e423",
"(8)Sanctuary_LV.w3x",
"04f67595a98141f0dbf9951401866fd100367124",
"(3)NomadIsles.w3x",
"4c0ea152ebb56758862587800102fa5bcc69b46f",
"(4)TurtleRock.w3x",
"4d06f81e80864da17fbe55bab5efcc92f4bbd818",
"(4)PhantomGrove_LV.w3x",
"5d7f1e36681acecb09412ecd98bad926f98d6a0c",
"(6)ThunderLake_LV.w3x",
"6cfe9babcc989f2bb920e0f34abf60e208f6846d",
"(2)EchoIsles.w3x",
"7a03d4e8419270792d3b09102f757794fc00861d",
"(6)NeonCity.w3x",
"7b989a3921b84b0243cb57324541dd6365cd062d",
"(4)CentaurGrove.w3x",
"7b993b67bdfe4048d37fab6040fad3f93437f4b2",
"(2)SwampedTemple.w3x",
"08d79e0b6e6e6f0d2b87ab86dfcbd5c66595316c",
"(6)RuinsOfStratholme.w3x",
"8ed916737dc5d566db70723b4962f0c0c2bb90a9",
"(4)HillsbradCreek.w3x",
"9b44e6fd6ae46646d6e99503df03c6c6fb9caa0f",
"(6)GnollWood.w3x",
"30cf1f5c556ad0afab03c1bc64dc3f6eb962a8b5",
"(8)GoldRush.w3x",
"38f93342c0803a02b1c7f5c3cc4e18178f09079c",
"(4)FurbolgMountain.w3x",
"64a9441dd0331cdd4f6d7efc79860b2e4dc8241c",
"(8)BanewoodBog_LV.w3x",
"071c580c16253c9fe233302227172fb1dc00234e",
"(8)BanewoodBog_LV.w3x",
"72af6dbc24405875c1d3b0cf774b02f3ba88e238",
"(4)TwistedMeadows.w3x",
"79ed9f1ef941d7df6f23e4034601512dfc36472f",
"(2)TerenasStand_LV.w3x",
"89b5fe9da379ca2b0e6bd179a22931b01dbfea03",
"(4)TidewaterGlades_LV.w3x",
"93cda6f77054b8b071dae5f60b25d4e08a0e9822",
"(4)FountainOfManipulation.w3x",
"116d65224c9debe25be3a8bcf3c064ff8784a694",
"(8)MarketSquare.w3x",
"287a54279abec56fd358588d149a378d66f16bce",
"(8)GolemsInTheMist_LV.w3x",
"362d0890e5dfa935236501702d9fa791d7b6af10",
"(6)UpperKingdom.w3x",
"415a0f84dafaf91439fb324487336fd240d769c6",
"(2)NorthernIsles.w3x",
"00589eaaff715bf1b92e0caf1350c26dee85242d",
"(6)TimbermawHold.w3x",
"0995d363344e085742dd6437735d63d52c303c2e",
"(8)MarketSquare.w3x",
"3330d3d6feb4e0742f990d5db6852c00e37049d5",
"(8)BanewoodBog_LV.w3x",
"5311e717877849ddc4a3112af33ac8f6f74c695a",
"(8)Battleground_LV.w3x",
"8649aae5ea520dc14f7f695d19ede773f1d0c2ef",
"(8)NerubianPassage.w3x",
"9524abb8e35ce7b158bfa4d4b8734234d6073ca5",
"(2)LastRefuge.w3x",
"20943ef700161c7a0a71720276a8b2faff0d9429",
"(2)Dalaran.w3x",
"211244c7d1def681554f4894a8d8fc35c794ec76",
"(8)TwilightRuins_LV.w3x",
"480273ca4126983d2831f00197da81f1bba00b6c",
"(8)Mur'gulOasis_LV.w3x",
"654585139a331727787a7dc3527839ff1fbc4125",
"(6)StranglethornVale.w3x",
"a678a01668fb3c7a784c5619601961693fb09e27",
"(4)LostTemple_LV.w3x",
"ab2ddfdd2593d95f99a1761614a4e38f487a2e34",
"(4)SynergyBigPaved.w3x",
"b4d7b54d9da0df059460eccec724d0dcb25925f0",
"(6)Monsoon_LV.w3x",
"b77acc95c5e5f28b1607ad877391dc363c75ec11",
"(8)Feralas_LV.w3x",
"b699c9e33372a81313cd942cd5faf47ded2f5e86",
"(6)SilverpineForest.w3x",
"ccc17027c0e5c983ffaeca959f25817812de2db8",
"(6)BloodstoneMesa_LV.w3x",
"cf265a06a16b468e3721b1f4af6311801cd77618",
"(4)Deathrose.w3x",
"ddee38acb17ca0c372e271ef0dbe684a78d42eb4",
"(2)Amazonia.w3x",
"df94bda098e2b0e748cc02a0b716519c0686175d",
"(6)RollingHills.w3x",
"e92ca578018140ef294425e0f70ccecb15225549",
"(4)Avalanche_LV.w3x",
"ede6c4c4f8e258b3d58d3c07d5d8f27d4e3fb703",
"(4)Deathrose.w3x",
"efa8f4ad0a95b1a841955db45ad7b0d5630226dd",
"(6)TimbermawHold.w3x",
"f2842cbe566efbba0d52a0fa3b04cb0f18cd92a9",
"(8)Northshire_LV.w3x",
"fdc2a6192848e6863d53477f81886e126fe3d7f8",
"(8)Deadlock_LV.w3x",
};

using namespace std;

const char replay_name[] = "replay.w3g";
const char replay_name_converted[] = "replay(converted).w3g";
const int block_size = 8192, add_space = 500, compression_level = 1;
uint32_t patch = 10032;
uint16_t build = 6108;

uint16_t xor16(uint32_t x)
{
    return (uint16_t)(x ^ (x >> 16));
}

#pragma region memstream
class membuf : public std::basic_streambuf<char> {
public:
    membuf(const uint8_t* p, size_t l) {
        setg((char*)p, (char*)p, (char*)p + l);
    }
};

class memstream : public std::istream {
public:
    memstream(const uint8_t* p, size_t l) :
        std::istream(&_buffer),
        _buffer(p, l) {
        rdbuf(&_buffer);
    }

private:
    membuf _buffer;
};
#pragma endregion

struct crc32
{
    static void generate_table(uint32_t(&table)[256])
    {
        uint32_t polynomial = 0xEDB88320;
        for (uint32_t i = 0; i < 256; i++)
        {
            uint32_t c = i;
            for (size_t j = 0; j < 8; j++)
            {
                if (c & 1) {
                    c = polynomial ^ (c >> 1);
                }
                else {
                    c >>= 1;
                }
            }
            table[i] = c;
        }
    }

    static uint32_t update(uint32_t(&table)[256], uint32_t initial, const void* buf, size_t len)
    {
        uint32_t c = initial ^ 0xFFFFFFFF;
        const uint8_t* u = static_cast<const uint8_t*>(buf);
        for (size_t i = 0; i < len; ++i)
        {
            c = table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
        }
        return c ^ 0xFFFFFFFF;
    }
};

struct BlockHeader
{
    uint32_t c_size;
    uint32_t u_size;
    uint16_t check1;
    uint16_t check2;

    void compute_checkshum(BYTE* block)
    {
        check1 = xor16(crc32(0L, (BYTE*)this, sizeof BlockHeader));
        check2 = xor16(crc32(0L, block, c_size));
    }
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

    bool read(FILE* f)
    {
        memset(this, 0, sizeof(W3GHeader));
        if (fread(this, 1, 48, f) != 48)
            return false;
        if (memcmp(intro, "Warcraft III recorded game", 26))
            return false;
        if (header_v == 1)
        {
            if (fread(&ident, 1, 4, f) != 4) return false;
            if (fread(&patch_v, 1, 4, f) != 4) return false;
            if (fread(&build_v, 1, 2, f) != 2) return false;
            if (fread(&flags, 1, 2, f) != 2) return false;
            if (fread(&length, 1, 4, f) != 4) return false;
            if (fread(&checksum, 1, 4, f) != 4) return false;
        }
        else return false;
        return true;
    }

    void compute_checksum()
    {
        checksum = 0;
        checksum = crc32(0L, (BYTE*)this, header_size);
    }
};

string to_low_register(const char* input)
{
    string line = input;
    for (int i(0); i < line.size(); i++)
    {
        if ((line[i] >= 65) && (line[i] <= 90)) line[i] += 32;
    }
    return line;
}

string to_low_register(string input)
{
    string line = input;
    for (int i(0); i < line.size(); i++)
    {
        if ((line[i] >= 65) && (line[i] <= 90)) line[i] += 32;
    }
    return line;
}

void real_u_size(BYTE* dataUncompressed, int& size)
{
    int amount_of_nulls = 0, pos = size - 1;
    while (dataUncompressed[pos--] == 0) amount_of_nulls++;
    size -= amount_of_nulls;
}

bool read_replay(FILE* read, W3GHeader& hdr, BYTE* body_c)
{
    int pos = ftell(read);
    fseek(read, 0, SEEK_END);
    if (ftell(read) < hdr.c_size)
    {
        cout << "The data in replay is corrupted!\n";
        return false;
    }
    fseek(read, pos, SEEK_SET);
    fread(body_c, 1, hdr.c_size - hdr.header_size, read);

    return true;
}

bool write_replay(const char* name, W3GHeader& hdr, BYTE* body_c)
{
    FILE* write;
    if (fopen_s(&write, name, "wb"))
    {
        cout << "Replay " << name << " was not created. Writing error!\n";
        return false;
    }
    hdr.build_v = build;
    hdr.compute_checksum();
    fwrite(&hdr, 1, hdr.header_size, write);
    fwrite(body_c, 1, hdr.c_size - hdr.header_size, write);
    fclose(write);

    return true;
}

bool decompress_block(BlockHeader& bhdr, BYTE* dataCompressed, BYTE* dataUncompressed)
{
    z_stream strm;

    // allocate inflate state 
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = bhdr.c_size;
    strm.next_in = Z_NULL;
    if (inflateInit(&strm) != Z_OK)
        return false;

    // decompress until deflate stream ends or end of file 
    strm.next_in = dataCompressed;
    strm.avail_out = bhdr.u_size;
    strm.next_out = dataUncompressed;

    if (inflate(&strm, Z_SYNC_FLUSH) != Z_OK)
        return false;
    (void)inflateEnd(&strm);
    return true;
}

bool compress_block(BlockHeader& bhdr, BYTE* dataCompressed, BYTE* dataUncompressed)
{
    int ret, flush;
    z_stream strm;
    memset(&bhdr, 0, sizeof BlockHeader);
    bhdr.u_size = block_size;

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, compression_level);
    if (ret != Z_OK)
        return false;

    /* compress until end of file */
    strm.avail_in = bhdr.u_size;
    flush = Z_SYNC_FLUSH;
    strm.next_in = dataUncompressed;
    strm.avail_out = CHUNK;
    strm.next_out = dataCompressed;
    deflate(&strm, flush);
    bhdr.c_size = CHUNK - strm.avail_out;

    /* set the block header checksums*/
    bhdr.compute_checkshum(dataCompressed);

    /* clean up and return */
    (void)deflateEnd(&strm);
    return true;
}

void getEncodedString(BYTE* buffer, BYTE* block, int& pos, int& length)
{
    pos = 6;
    while (block[pos++] != 0); // reading the name of the host
    pos += block[pos++];
    while (block[pos++] != 0); // reading the Game Name
    pos++;
    length = -pos;
    while (block[pos++] != 0); // reading the Encoded String
    length += pos;
    memcpy(buffer, block + pos - length, length); 
}

void decodeEncStr(BYTE* EncodedString, int& size)
{
    uint8_t mask;
    int pos = 0;
    int dpos = 0;
    BYTE* Buffer = new BYTE[size];

    while (EncodedString[pos] != 0)
    {
        if (pos % 8 != 0)
        {
            Buffer[dpos++] = EncodedString[pos++] & (0xFFFE | ((mask & (0x1 << (pos % 8))) >> (pos % 8)));
        }
        else mask = EncodedString[pos++];
    }

    for (int i(0); i < dpos; i++)
    {
        EncodedString[i] = Buffer[i];
    }
    delete[] Buffer;
    size = dpos;
}

bool modifyEncodedString(BYTE* EncStr, int& length)
{
    decodeEncStr(EncStr, length);
    uint32_t crc_time = 0xFFFFFFFF;
    memcpy(EncStr + 9, &crc_time, 4); // setting map-crc
    int pos = 13;
    while (EncStr[pos++] != 0); // getting to the end of Map Name
    int name_length = pos - 13;
    string name = "", hash = "";
    do pos--;
    while ((EncStr[pos] != 0x2F) & (EncStr[pos] != 0x5C)); // getting to the start of Map Name
    pos ++;
    while (EncStr[pos] != 0) name += EncStr[pos++];
    for (int i(0); i < 49; i++)
    {
        if (to_low_register(MapNames[2 * i + 1]) == to_low_register(name))
        {
            hash = MapNames[2 * i];
            name = "Maps/" + (string)MapNames[2 * i + 1];
            break;
        }
    }

#pragma region MapWasntFound
    if (hash == "") // in case the map wasn't found
    {
        cout << "It was unable to auto-detect the map for the replay.\n";
        while (true)
        {
            cout << "Please, enter the name of the map replay was played on in a following format:\n";
            cout << "(2)Amazonia.w3x\n";
            cout << "Name of the map: ";
            cin >> name;
            for (int i(0); i < 49; i++)
            {
                if (to_low_register(MapNames[2 * i + 1]) == to_low_register(name))
                {
                    hash = MapNames[2 * i];
                    name = "Maps/" + (string)MapNames[2 * i + 1];
                    break;
                }
            }
            if (hash != "") break;
        }
    }
#pragma endregion
    int namesize = name.size() + 1;
    int old_length = length;
    length += (namesize - name_length); // updated length
    BYTE NewEncStr[add_space];
    BYTE* NewStrPos = NewEncStr;
    memcpy(NewStrPos, EncStr, 13);
    NewStrPos += 13;
    memcpy(NewStrPos, &name[0], name.size());
    NewEncStr[13 + name.size()] = 0;
    NewStrPos += namesize;
    memcpy(NewStrPos, EncStr + 13 + name_length, old_length - 13 - 20 - name_length);
    NewStrPos += old_length - 13 - 20 - name_length;
    uint32_t value = 0;
    string buf = "";
    char buff[8];
    for (int i(0); i < 5; i++)
    {
        memcpy(buff, &hash[i * 8], 8);
        buf = buff;
        stringstream str;
        str << buf;
        str >> std::hex >> value;
        for (int i(0), j(3); i < 4; i++, j--)
        {
            memcpy(NewStrPos + i, (uint8_t*)&value + j, 1);
        }
        NewStrPos += 4;
    }

    memcpy(EncStr, NewEncStr, length);
    return true;
}

void encodeEncStr(BYTE* EncStr, int& length)
{
    uint8_t Mask = 1;
    int old_length = length;
    int new_length = old_length + old_length / 7 + ((old_length % 7) > 0);
    BYTE result[add_space];

    for (int i = 0, j = 1; i < length; i++)
    {
        if ((EncStr[i] % 2) == 0)
            result[i + j] = EncStr[i] + 1;
        else
        {
            result[i + j] = EncStr[i];
            Mask |= 1 << ((i % 7) + 1);
        }

        if (i % 7 == 6 || i == length - 1)
        {
            result[i + j - (i % 7) - 1] = Mask;
            Mask = 1;
            j++;
        }
    }
    length = new_length;
    for (int i(0); i < length; i++)
        EncStr[i] = result[i];

    EncStr[length++] = 0;
}

bool decompress_and_modify(BYTE* dataCompressed, BYTE* dataUncompressed, W3GHeader& hdr)
{
    int u_size_before = hdr.u_size;
    hdr.u_size += block_size;
    BYTE BlockUncompressed[CHUNK];
    int blocks_left = hdr.blocks, pos_c = 0, pos_u = 0;
    BlockHeader bhdr;

    // decompress the first block

    memcpy(&bhdr, dataCompressed + pos_c, sizeof BlockHeader);
    if (!decompress_block(bhdr, dataCompressed + pos_c + sizeof BlockHeader, BlockUncompressed))
    {
        cout << "A decompression mistake. The data inside replay is corrupted!\n";
        return false;
    }
    memcpy(dataUncompressed, BlockUncompressed, block_size);
    pos_c += bhdr.c_size + sizeof BlockHeader;
    blocks_left--;

    // modify the first block

    int EncStrLength = 0;
    BYTE* EncodedString = new BYTE[add_space];
    getEncodedString(EncodedString, dataUncompressed, pos_u, EncStrLength); // DELETE ENCODED STRING?
    int length_before = EncStrLength;
    if (!modifyEncodedString(EncodedString, EncStrLength))
    {
        cout << "A critical mistake in reading the MapName. The data in replay seems to be corrupted!\n";
        return false;
    }
    encodeEncStr(EncodedString, EncStrLength);

    int shift = EncStrLength - length_before;
    memcpy(dataUncompressed + pos_u + shift, dataUncompressed + pos_u, block_size - pos_u);
    pos_u -= length_before;
    memcpy(dataUncompressed + pos_u, EncodedString, EncStrLength);
    pos_u = block_size + shift;
    delete[] EncodedString;

    // other blocks

    while (blocks_left--)
    {
        memcpy(&bhdr, dataCompressed + pos_c, sizeof BlockHeader);
        if (!decompress_block(bhdr, dataCompressed + pos_c + sizeof BlockHeader, BlockUncompressed))
        {
            cout << "Decompression mistake. The data inside replay is corrupted!\n";
            return false;
        }
        memcpy(dataUncompressed + pos_u, BlockUncompressed, block_size);
        pos_c += bhdr.c_size + sizeof BlockHeader;
        pos_u += bhdr.u_size;
    }

    // updating the padding with 0s, amount of blocks and the u_size

    hdr.blocks = pos_u / block_size + ((pos_u % block_size) > 0);
    hdr.u_size = block_size * hdr.blocks;
    memset(dataUncompressed + pos_u, 0, hdr.u_size - pos_u);
    hdr.u_size = u_size_before + shift;

    return true;
}

bool compress(BYTE* dataCompressed, BYTE* dataUncompressed, W3GHeader& hdr)
{
    BYTE BlockCompressed[CHUNK];
    BlockHeader bhdr;
    int blocks_left = hdr.blocks, pos_u = 0, pos_c = 0;
    while (blocks_left--)
    {
        if (!compress_block(bhdr, BlockCompressed, dataUncompressed + pos_u))
        {
            cout << "Compression mistake!\n";
            return false;
        }
        memcpy(dataCompressed + pos_c, &bhdr, sizeof BlockHeader);
        pos_c += sizeof BlockHeader;
        memcpy(dataCompressed + pos_c, BlockCompressed, bhdr.c_size);
        pos_c += bhdr.c_size;
        pos_u += block_size;
    }
    hdr.c_size = pos_c + hdr.header_size;

    return true;
} 

int main()
{
    W3GHeader replay_header;
#pragma region ReadReplay
    FILE* read;
    if (fopen_s(&read, replay_name, "rb"))
    {
        cout << "Replay " << replay_name << " was not found!\n";
        system("PAUSE");
        return -1;
    }
    if (!replay_header.read(read))
    {
        cout << "This is either not a Warcraft III (1.32) replay file or it is corrupted!\n";
        system("PAUSE");
        return -1;
    }
    if (replay_header.patch_v != patch)
    {
        cout << "Replay " << replay_name << " does not belong to a patch you wanna switch!\n";
        system("PAUSE");
        return -1;
    }

    BYTE* replay_c = new BYTE[replay_header.c_size + add_space];
    if (!read_replay(read, replay_header, replay_c))
    {
        system("PAUSE");
        return -1;
    }
    fclose(read);
#pragma endregion

#pragma region ModifyReplay
    BYTE* replay_u = new BYTE[replay_header.u_size + block_size];
    if(!decompress_and_modify(replay_c, replay_u, replay_header))
    {
        system("PAUSE");
        return -1;
    }

    if (!compress(replay_c, replay_u, replay_header))
    {
        system("PAUSE");
        return -1;
    }

    delete[] replay_u;
#pragma endregion

    if (!write_replay(replay_name_converted, replay_header, replay_c))
    {
        system("PAUSE");
        return -1;
    }

    delete[] replay_c;
	return 0;
}
