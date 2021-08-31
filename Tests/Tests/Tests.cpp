#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;
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

int main()
{
  /*  int64_t N = 0;
    while (true)
    {
        cin >> N;
        int64_t size = N * 1048576;
        int t1 = time(NULL);
        char* A = new char[size];
        int t2 = time(NULL);
        cout << "The byte-by-byte copying (Method 1):\n";
        cout << "Allocation took " << t2 - t1 << " seconds.\n";
        for (int64_t i = (int64_t)0; i < size; i++) A[i] = i % 8;
        t1 = time(NULL);
        cout << "Initialization took " << t1 - t2 << " seconds.\n";
        char* B = new char[size];
        t2 = time(NULL);
        cout << "Allocation took " << t2 - t1 << " seconds.\n";
        for (int64_t i = (int64_t)0; i < size; i++) B[i] = 0;
        t1 = time(NULL);
        cout << "Initialization took " << t1 - t2 << " seconds.\n";

        for (int64_t i = (int64_t)0; i < size; i++) B[i] = A[i];
        t2 = time(NULL);
        cout << "Copying took " << t2 - t1 << " seconds.\n\n";
        delete[] A;

        cout << "The 8byte-by-step copying (Method 2):\n";
        size /= 8;
        t1 = time(NULL);
        int64_t* C = new int64_t[size];
        t2 = time(NULL);
        cout << "Allocation took " << t2 - t1 << " seconds.\n";
        for (int64_t i = (int64_t)0; i < size; i++) C[i] = 0;
        t1 = time(NULL);
        cout << "Initialization took " << t1 - t2 << " seconds.\n";

        int64_t* D = (int64_t*)B;
        for (int64_t i = (int64_t)0; i < size; i++) C[i] = B[i];
        t2 = time(NULL);
        cout << "Copying took " << t2 - t1 << " seconds.\n\n";

        delete[] B;
        delete[] C;
    }*/ /*
    uint32_t size_compressed = 0, size_uncompressed = 8192;
    BYTE* dataCompressed = NULL;
//    char* dataUncompressed = NULL;
    FILE* read;
//    FILE* write;
    int pos = 68;
    fopen_s(&read, "LastReplay.w3g", "rb");
    fseek(read, pos, SEEK_SET);
 //   fopen_s(&write, "converted_1.w3g", "wb");
    fread(&size_compressed, 1, 4, read);
    int size = size_compressed + 12;
        // read the block
        pos = ftell(read);
        pos -= 4;
        fseek(read, pos, SEEK_SET);
        dataCompressed = new BYTE[size];
        fread(dataCompressed, 1, size, read);
 //       pos += size_compressed;
        fclose(read);
 //       for (int i(0); i < 12; i++) dataCompressed[i] = 0;
 /*   char data_piece1[size];
    FILE* read;
    fopen_s(&read, "header", "rb");
    fread(data_piece1, 1, 68, read);
    fclose(read);*//*

    uint32_t table[256];
    crc32::generate_table(table);
    uint32_t crc = crc32::update(table, 0, dataCompressed, size);
    cout << crc << endl;


    delete[] dataCompressed;*/
    /*
    int size = 0, i = 1;
    FILE* read;
    FILE* write;
 //   BYTE* data;
    std::string name_;
    BYTE y[200000];
    while (true)
    {
        int x = 0;
        fopen_s(&read, "TempReplay.w3g", "rb");

        cout << "TempReplay.w3g was opened!\n";
        
 /*       fseek(read, 0, SEEK_END);
        cout << x++ << endl;
        size = ftell(read);
        cout << x++ << endl;
        data = new BYTE[size];
        cout << x++ << endl;
        fseek(read, 0, SEEK_SET);
        cout << x++ << endl;
        fread(data, 1, size, read);
        cout << x++ << endl;
        cout << "TempReplay.w3g was read!\n";
        fclose(read);
        cout << x++ << endl; */
/*
        uint32_t c; // note: int, not char, required to handle EOF
        while ((c = fgetc(read)) != EOF) { // standard C I/O file reading loop
            y[x++] = (BYTE)c;
        }

        if (ferror(read))
            puts("I/O error when reading");
        else if (feof(read))
            puts("End of file reached successfully");

        fclose(read);

        name_ = "TempReplay-" + to_string(time(NULL)) + "-" + to_string(i++);
        const char* name = name_.c_str();
        fopen_s(&write, name, "wb");

        fwrite(y, 1, x, write);
        fclose(write);
        cout << "Copied successfully\n";

//        delete[] data;
        Sleep(5000);
    }*/
    /*
    //    decompression(277, 8192, "last", "last_");
        uint32_t size_compressed = 0, size_uncompressed = 8192;
        BYTE* dataCompressed = NULL;
        FILE* read;
        int pos = 0;
        fopen_s(&read, "testdata", "rb");
        fseek(read, pos, SEEK_SET);
    //    fread(&size_compressed, 1, 4, read);
        int size = 496;
        // read the block
    //    pos = ftell(read);
    //    pos += 9;
     //   fseek(read, pos, SEEK_SET);
        dataCompressed = new BYTE[size];
        fread(dataCompressed, 1, size, read);
        fclose(read);




    //    uLong adler = adler32(0L, Z_NULL, 0);
     //   uLong adler = adler32(1L, dataCompressed, size);
     //     cout << adler << endl;


     //   uLong crc = crc32(1L, dataCompressed, size);
    //    cout << crc << endl;
        BYTE* header = NULL;
        int header_size = 12;
        header = new BYTE[header_size];
        fopen_s(&read, "testheader", "rb");
        fseek(read, 0, SEEK_SET);
        fread(header, 1, header_size, read);
        fclose(read);

        uint32_t check;
        uint16_t check1, check2;
        check1 = xor16(crc32(0L, header, header_size));
        check2 = xor16(crc32(0L, dataCompressed, size));

        check = ((uint32_t)(check1 & 0xFF) << 24) + ((uint32_t)(check1 & 0xFF00) << 8) + ((uint32_t)(check2 & 0xFF) << 8) + ((uint32_t)(check2 & 0xFF00) >> 8);
        cout << hex << check << endl;

        delete[] header;
        delete[] dataCompressed;*/


FILE* write;
if (fopen_s(&write, "folder/1.txt", "wb"))
{
    cout << "File was not opened!\n";
    return 0;
}

fwrite(MapNames, 1, 40, write);
fclose(write);

    system("PAUSE");
}
