/*
Michaelangel007
https://github.com/Michaelangel007/rubiks_cheat_sheet
Copyleft (C) 2017
*/

// Includes _______________________________________________________________

    #include <stdio.h>  // printf()
    #include <stdlib.h>
    #include <string.h> // strncpy()

// Defines ________________________________________________________________

    #if defined(_MSC_VER)
        #define PACKED
        #pragma pack(push,1)
    #endif
    #if defined(__GNUC__)
        #define PACKED __attribute__ ((__packed__))
    #endif

// Types __________________________________________________________________

    enum TargaImageType_e
    {
        TARGA_TYPE_RGB = 2
    };

    struct TargaHeader_t
    {// header                         // Addr Bytes
        uint8_t nIdBytes             ; // 00 01 size of ID field that follows 18 byte header (0 usually)
        uint8_t bHasPalette          ; // 01 01
        uint8_t iImageType           ; // 02 01 type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

        int16_t iPaletteFirstColor   ; // 03 02
        int16_t nPaletteColors       ; // 05 02
        uint8_t nPaletteBitsPerEntry ; // 07 01 number of bits per palette entry 15,16,24,32

        int16_t nOriginX             ; // 08 02 image x origin
        int16_t nOriginY             ; // 0A 02 image y origin
        int16_t nWidthPixels         ; // 0C 02
        int16_t nHeightPixels        ; // 0E 02
        uint8_t nBitsPerPixel        ; // 10 01 image bits per pixel 8,16,24,32
        uint8_t iDescriptor          ; // 11 01 image descriptor bits (vh flip bits)

      // pixel data...
      //uint8_t aPixelData[1]        ; // 12 ?? variable length RGB data
    } PACKED;

    char BAD_TARGA_HEADER_SIZE_Compiler_Packing_not_18[ sizeof( TargaHeader_t  ) ==  18       ];

/*
    struct rgba_t
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
*/

    struct bgra_t
    {
        unsigned char b;
        unsigned char r;
        unsigned char g;
        unsigned char a;
    };

/*
 * @param {Number} i - State between 0 and 2^21-1
 * @return {String} 21 faces as a bit string
 */
// ========================================================================
char* makeBitString( int state )
{
    /*  */ char text  [21];
    static char output[32];

    for( int face = 0; face < 21; ++face )
        text[ face ] = ((i >> face) & 1) ? '1' : '0';

    strncpy( output+ 0, text+ 0, 3 ); strncpy( output+ 3, ":", 1 );
    strncpy( output+ 4, text+ 3, 5 ); strncpy( output+ 9, ":", 1 );
    strncpy( output+10, text+ 8, 5 ); strncpy( output+15, ":", 1 );
    strncpy( output+16, text+13, 5 ); strncpy( output+21, ":", 1 );
    strncpy( output+22, text+18, 3 );
    output[25] = 0;

    return output;
}

/**
 * Note: Corner position is one of: 0, 2, 3, 4, 6, 7, 13, 14, 16, 17, 18, 20
 * @param {Number} state   - Last Layer k-factoridic state of cube: 0 .. 2^21
 * @param {Number} corner1 - Corner position 1 to test if yellow sticker
 * @param {Number} corner3 - Corner position 2 to test it yellow sticker
 * @param {Number} corner3 - Corner position 3 to test if yellow sticker
 * @return true if yellow corner faces are valid
 */
// ========================================================================
int invalidCorner( int state, int corner1, int corner2, int corner3 )
{
    int count = 0;
        count += ((state >> corner1) & 1);
        count += ((state >> corner2) & 1);
        count += ((state >> corner3) & 1);
    return (count != 1);
}

/*
 * Note: Edge position is one of: 1, 5, 8, 9, 11, 12, 15, 19
 * @param {Number} state - Last Layer k-factoridic state of cube: 0 .. 2^21
 * @param {Number} edge1 - Edge position 1 to test if yellow sticker
 * @param {Number} edge2 - Edge position 2 to test it yellow sticker
 * @return true if yellow edge faces are valid
 */
// ========================================================================
int invalidEdge( int state, int edge1, int edge2 )
{
    int count = 0;
        count += ((state >> edge1) & 1);
        count += ((state >> edge2) & 1);
    return (count != 1);
}

/*
 * @param {Number} state - Last Layer k-factoridic state of cube: 0 .. 2^21
 */
// ========================================================================
int edgeParity( int state )
{
    int parity = 0;

        parity += (state >>  1) & 1; // UB
        parity += (state >>  5) & 1;

        parity += (state >>  8) & 1; // LU
        parity += (state >>  9) & 1;

        parity += (state >> 11) & 1; // RU
        parity += (state >> 12) & 1;

        parity += (state >> 15) & 1; // UF
        parity += (state >> 19) & 1;

    // Edge Parity % 2 = 0
    return parity % 2;
}


// ========================================================================
bool Targa_Save32( const char *filename, const int width, const int height, void *bitmap )
{
    bool valid = false;

    FILE *pOutFile = fopen( filename, "w+b" );
    if( pOutFile )
    {
        // Write header
        TargaHeader_t  header;
        TargaHeader_t *pTGA = &header;

        memset( &header, 0, sizeof( header ) );

        pTGA->iImageType    = TARGA_TYPE_RGB;
        pTGA->nWidthPixels  = width;
        pTGA->nHeightPixels = height;
        pTGA->nBitsPerPixel = 32;

        size_t dimension = width * height;
        size_t size_head = sizeof( header );
        size_t size_data = sizeof( uint32_t ) * dimension;
        size_t wrote;

        valid = true;
        wrote = fwrite( &header, size_head, 1, pOutFile ); valid &= (wrote == size_head);
        wrote = fwrite(  bitmap, size_data, 1, pOutFile ); valid &= (wrote == size_data);
        fclose( pOutFile );
    }

    return valid;
}

// ========================================================================
void makeBitmapOLL( int state )
{
    const int COLOR_W = 22;
    const int COLOR_K = 23;
    const int COLOR_G = 24;
    const int COLOR_Y = 25;

    bgra_t palette[] =
    {   //blue green  red
        { 0x00, 0x00, 0x00, 0x00 }, // A=0
        { 0x00, 0x00, 0x00, 0x00 }, // B=1
        { 0x00, 0x00, 0x00, 0x00 }, // C=2
        { 0x00, 0x00, 0x00, 0x00 }, // D=3
        { 0x00, 0x00, 0x00, 0x00 }, // D=4
        { 0x00, 0x00, 0x00, 0x00 }, // E=5
        { 0x00, 0x00, 0x00, 0x00 }, // F=6
        { 0x00, 0x00, 0x00, 0x00 }, // G=7
        { 0x00, 0x00, 0x00, 0x00 }, // H=8
        { 0x00, 0x00, 0x00, 0x00 }, // I=9
        { 0x00, 0x00, 0x00, 0x00 }, // J=10
        { 0x00, 0x00, 0x00, 0x00 }, // K=11
        { 0x00, 0x00, 0x00, 0x00 }, // L=12
        { 0x00, 0x00, 0x00, 0x00 }, // M=13
        { 0x00, 0x00, 0x00, 0x00 }, // N=14
        { 0x00, 0x00, 0x00, 0x00 }, // O=15
        { 0x00, 0x00, 0x00, 0x00 }, // P=16
        { 0x00, 0x00, 0x00, 0x00 }, // Q=17
        { 0x00, 0x00, 0x00, 0x00 }, // R=18
        { 0x00, 0x00, 0x00, 0x00 }, // S=19
        { 0x00, 0x00, 0x00, 0x00 }, // T=20
        { 0x00, 0x00, 0x00, 0x00 }, // U=21
        { 0xFF, 0xFF, 0xFF, 0xFF }, // _ = 22 White
        { 0x00, 0x00, 0x00, 0xFF }, // x = 23 blacK
        { 0xA0, 0xA0, 0xA0, 0xFF }, //   = 24 Grey
        { 0x00, 0xFF, 0xFF, 0xFF }  //   = 25 Yellow
    };

    const char *source =
//0        1    1    2         3         4         5         6         7         8         9         0         1         2         3
//1        0    5    0         0         0         0         0         0         0         0         0         0         0         0
"_______________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx____xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx____xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx_______________" //  0
"_______________xAAAAAAAAAAAAAAAAAAAAAAAAAAAAx____xBBBBBBBBBBBBBBBBBBBBBBBBBBBBx____xCCCCCCCCCCCCCCCCCCCCCCCCCCCCx_______________" //  1
"_______________xAAAAAAAAAAAAAAAAAAAAAAAAAAAAx____xBBBBBBBBBBBBBBBBBBBBBBBBBBBBx____xCCCCCCCCCCCCCCCCCCCCCCCCCCCCx_______________" //  2
"_______________xAAAAAAAAAAAAAAAAAAAAAAAAAAAAx____xBBBBBBBBBBBBBBBBBBBBBBBBBBBBx____xCCCCCCCCCCCCCCCCCCCCCCCCCCCCx_______________" //  3
"_______________xAAAAAAAAAAAAAAAAAAAAAAAAAAAAx____xBBBBBBBBBBBBBBBBBBBBBBBBBBBBx____xCCCCCCCCCCCCCCCCCCCCCCCCCCCCx_______________" //  4
"_______________xAAAAAAAAAAAAAAAAAAAAAAAAAAAAx____xBBBBBBBBBBBBBBBBBBBBBBBBBBBBx____xCCCCCCCCCCCCCCCCCCCCCCCCCCCCx_______________" //  5
"_______________xAAAAAAAAAAAAAAAAAAAAAAAAAAAAx____xBBBBBBBBBBBBBBBBBBBBBBBBBBBBx____xCCCCCCCCCCCCCCCCCCCCCCCCCCCCx_______________" //  6
"_______________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx____xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx____xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx_______________" //  7
"________________________________________________________________________________________________________________________________" //  8
"________________________________________________________________________________________________________________________________" //  9
"________________________________________________________________________________________________________________________________" // 10
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 11
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 12
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 13
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 14
"xxxxxxxx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xxxxxxxx" // 15
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 16
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 17
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 18
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 19
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 20
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 21
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 22
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 23
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 24
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 25
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 26
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 27
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 28
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 29
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 30
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 31
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 32
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 33
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 34
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 35
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 36
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 37
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 38
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 39
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 40
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 41
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 42
"xDDDDDDx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xHHHHHHx" // 43
"xxxxxxxx___xxxxEEEEEEEEEEEEEEEEEEEEEEEEEEEEEExxxxFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFxxxxGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGxxxx___xxxxxxxx" // 44
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 45
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 46
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 47
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 48
"xxxxxxxx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xxxxxxxx" // 49
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 50
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 51
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 52
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 53
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 54
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 55
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 56
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 57
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 58
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 59
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 60
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 61
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 62
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 63
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 64
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 65
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 66
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 67
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 68
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 69
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 70
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 71
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 72
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 73
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 74
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 75
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 76
"xIIIIIIx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xMMMMMMx" // 77
"xxxxxxxx___xxxxJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJxxxxKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKxxxxLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLxxxx___xxxxxxxx" // 78
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 79
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 80
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 81
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" // 82
"xxxxxxxx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xxxxxxxx" // 83
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 84
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 85
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 86
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 87
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 88
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 89
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 90
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 91
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 92
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 93
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 94
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 95
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 96
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 97
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 98
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" // 99
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //100
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //101
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //102
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //103
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //104
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //105
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //106
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //107
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //108
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //109
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //110
"xNNNNNNx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xRRRRRRx" //111
"xxxxxxxx___xxxxOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOxxxxPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPxxxxQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQxxxx___xxxxxxxx" //112
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" //113
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" //114
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" //115
"___________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx___________" //116
"________________________________________________________________________________________________________________________________" //117
"________________________________________________________________________________________________________________________________" //118
"________________________________________________________________________________________________________________________________" //119
"_______________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx____xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx____xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx_______________" //120
"_______________xSSSSSSSSSSSSSSSSSSSSSSSSSSSSx____xTTTTTTTTTTTTTTTTTTTTTTTTTTTTx____xUUUUUUUUUUUUUUUUUUUUUUUUUUUUx_______________" //121
"_______________xSSSSSSSSSSSSSSSSSSSSSSSSSSSSx____xTTTTTTTTTTTTTTTTTTTTTTTTTTTTx____xUUUUUUUUUUUUUUUUUUUUUUUUUUUUx_______________" //122
"_______________xSSSSSSSSSSSSSSSSSSSSSSSSSSSSx____xTTTTTTTTTTTTTTTTTTTTTTTTTTTTx____xUUUUUUUUUUUUUUUUUUUUUUUUUUUUx_______________" //123
"_______________xSSSSSSSSSSSSSSSSSSSSSSSSSSSSx____xTTTTTTTTTTTTTTTTTTTTTTTTTTTTx____xUUUUUUUUUUUUUUUUUUUUUUUUUUUUx_______________" //124
"_______________xSSSSSSSSSSSSSSSSSSSSSSSSSSSSx____xTTTTTTTTTTTTTTTTTTTTTTTTTTTTx____xUUUUUUUUUUUUUUUUUUUUUUUUUUUUx_______________" //125
"_______________xSSSSSSSSSSSSSSSSSSSSSSSSSSSSx____xTTTTTTTTTTTTTTTTTTTTTTTTTTTTx____xUUUUUUUUUUUUUUUUUUUUUUUUUUUUx_______________" //126
"_______________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx____xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx____xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx_______________" //127
;
    /* */ bgra_t  bitmap[128][128];
    /* */ bgra_t  pixel;

    const char   *src =  source;
    /* */ bgra_t *dst = &bitmap[0][0];

    // Initialize palette based on current state
    int mask = (1 << 20);
    for( int face = 0; face < 21; face++ )
    {
        if( state & mask )
            palette[ face ] = palette[ COLOR_Y ];
        else
            palette[ face ] = palette[ COLOR_G ];

        mask >>= 1;
    }

    // Copy bitmap
    for( int y = 0; y < 128; y++ )
    {
        for( int x =0 ; x < 128; x++ )
        {
            if( *src == '_' )
                pixel = palette[ COLOR_W ]; // background
            else
            if( *src == 'x' )
                pixel = palette[ COLOR_K ]; // foreground
            else
            if( *src >= 'A' )
                pixel = palette[ *src - 'A' ];

            *dst = pixel;

            src++;
            dst++;
        }
    }

    char filename[ 32 ];
    sprintf( filename, "oll_%d.tga", state );

    Targa_Save32( filename, 128, 128, bitmap );
}

/*
    21 stickers

    * 2^21 yellow states (including bad/impossible states)
    * Removing illegal states = 1,296
    * Removing bad edge parity = 

    Last Layer

        0   1   2
       ___ ___ ___
      |   |   |   |
     3| 4 | 5 | 6 | 7
      |___|___|___|
      |   |   |   |
     8| 9 | 10| 11|12
      |___|___|___|
      |   |   |   |
    13| 14| 15| 16|17
      |___|___|___|
        18  19  20


*/
void enum_yellow()
{
    // 21 faces
    const int SHIFT_LUB_nZ =   0;
    const int SHIFT__UB_nZ =   1;
    const int SHIFT_RUB_nZ =   2;

    const int SHIFT_LUB_nX =   3;
    const int SHIFT_LUB_pY =   4;
    const int SHIFT__UB_pY =   5;
    const int SHIFT_RUB_pY =   6;
    const int SHIFT_RUB_pX =   7;

    const int SHIFT_LU__nX =   8;
    const int SHIFT_LU__pY =   9;
    const int SHIFT_MU__pY =  10;
    const int SHIFT_RU__pY =  11;
    const int SHIFT_RU__pX =  12;

    const int SHIFT_LUF_nX =  13;
    const int SHIFT_LUF_pY =  14;
    const int SHIFT__UF_pY =  15;
    const int SHIFT_RUF_pY =  16;
    const int SHIFT_RUF_pX =  17;

    const int SHIFT_LUF_pZ =  18;
    const int SHIFT__UF_pZ =  19;
    const int SHIFT_RUF_pZ =  20;

    int i, n = (1 << 21);
    int valid = 1;
    char *bitstring;

    printf( "Number of OLL permutations after F2L ..." );

    for( i = 0; i < n; ++i )
    {
        // if middle is not yellow then cube is not valid
        if( (i >> SHIFT_MU__pY) & 1 )
        {
            // Count # of yellow faces in each corner
            if( invalidCorner( i, SHIFT_LUB_nZ, SHIFT_LUB_pY, SHIFT_LUB_nX ) ) continue; // LUB
            if( invalidCorner( i, SHIFT_RUB_nZ, SHIFT_RUB_pY, SHIFT_RUB_pX ) ) continue; // RUB
            if( invalidCorner( i, SHIFT_LUF_pZ, SHIFT_LUF_pY, SHIFT_LUF_nX ) ) continue; // LUF
            if( invalidCorner( i, SHIFT_RUF_pZ, SHIFT_RUF_pY, SHIFT_RUF_pX ) ) continue; // RUF

            // Count # of yellow faces on each edge
            if( invalidEdge( i, SHIFT__UB_nZ, SHIFT__UB_pY ) ) continue; // UB
            if( invalidEdge( i, SHIFT_LU__nX, SHIFT_LU__pY ) ) continue; // LU
            if( invalidEdge( i, SHIFT_RU__pX, SHIFT_RU__pY ) ) continue; // RU
            if( invalidEdge( i, SHIFT__UF_pZ, SHIFT__UF_pY ) ) continue; // UB

            //parity  = edgeParity( i );
            bitstring = makeBitString( i );
            printf( "# %-4d/1296: @ %-d: $ %s\n", valid, i, bitstring );
            makeBitmapOLL( i );
            valid++;
        }
    }
}

int main()
{
    enum_yellow();
    return 0;
}

