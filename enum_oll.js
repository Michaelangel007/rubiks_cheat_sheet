"use strict";

/*
 * @param {Number} i - State between 0 and 2^21-1
 * @return {String} 21 faces as a bit string
 */
function makeBitString( i )
{
    var text = new Array(21);

    for( var face = 0; face < 21; ++face )
        text[ face ] = ((i >> face) & 1) ? '1' : '0';

    var output = ''
        + text.slice( 0, 3).join('') + ':'
        + text.slice( 3, 8).join('') + ':'
        + text.slice( 8,13).join('') + ':'
        + text.slice(13,18).join('') + ':'
        + text.slice(18,21).join('');
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
function invalidCorner( state, corner1, corner2, corner3 )
{
    var count = 0;
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
function invalidEdge( state, edge1, edge2 )
{
    var count = 0;
        count += ((state >> edge1) & 1);
        count += ((state >> edge2) & 1);
    return (count != 1);
}

// https://ruwix.com/the-rubiks-cube/unsolvable-rubiks-cube-invalid-scramble/
// ========================================================================
function cornerParity( state )
{
    var parity = 0;

        parity += ((state <<  1) & 2); // LUB CCW
        parity += ((state >>  3) & 1); // LUB CW

        parity += ((state >>  2) & 1); // RUB CW
        parity += ((state >>  6) & 2); // RUB CCW

        parity += ((state >> 12) & 2); // LUF CCW
        parity += ((state >> 18) & 1); // LUF CW

        parity += ((state >> 17) & 1); // RUF CW
        parity += ((state >> 19) & 2); // RUF CCW

    return parity % 3;
}

/*
 * @param {Number} state - Last Layer k-factoridic state of cube: 0 .. 2^21
 */
// ========================================================================
function edgeParity( state )
{
    var parity = 0;

        parity += ((state >>  1) & 1); // UB
        parity += ((state >>  8) & 1); // LU
        parity += ((state >> 12) & 1); // RU
        parity += ((state >> 19) & 1); // UF

    // Edge Parity % 2 = 0
    return parity % 2;
}

/*
 * Right align text
 */
// ========================================================================
function pad( text, width )
{
    return (new Array(width).join(' ') + text).slice( -width );
}

/*
    21 stickers

    * 2^21 yellow states (including bad/impossible states)
    * Removing illegal states = 1,296
    * Removing bad corner parity MOD 3 != 0
    * Removing bad edge   parity MOD 2 != 0

    http://math.stackexchange.com/questions/127577/how-to-tell-if-a-rubiks-cube-is-solvable

    Last Layer Permutations
    = 4!*3^4 * 4!*2^4 / (perm parity * corner parity * edge parity)
    = 4! * 3^4 * 4! * 2^4 / (2*3*2)
    = 4! * 3^3 * 4! * 2^2
    = 24 * 27 * 24 * 4
    = 62,208

    Yellow OLL Permutations
    = Corner Positions * Edge Positions
    = 3^4 * 2^4
    = 1296

    Valid Yellow OLL Permutations
    = 1296 / Parity
    = 1296 / (2*3*2)
    = 108

    OLL Algoriths = 58

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
function enum_yellow()
{
    // 21 faces
    var SHIFT_LUB_nZ =   0;
    var SHIFT__UB_nZ =   1;
    var SHIFT_RUB_nZ =   2;

    var SHIFT_LUB_nX =   3;
    var SHIFT_LUB_pY =   4;
    var SHIFT__UB_pY =   5;
    var SHIFT_RUB_pY =   6;
    var SHIFT_RUB_pX =   7;

    var SHIFT_LU__nX =   8;
    var SHIFT_LU__pY =   9;
    var SHIFT_MU__pY =  10;
    var SHIFT_RU__pY =  11;
    var SHIFT_RU__pX =  12;

    var SHIFT_LUF_nX =  13;
    var SHIFT_LUF_pY =  14;
    var SHIFT__UF_pY =  15;
    var SHIFT_RUF_pY =  16;
    var SHIFT_RUF_pX =  17;

    var SHIFT_LUF_pZ =  18;
    var SHIFT__UF_pZ =  19;
    var SHIFT_RUF_pZ =  20;

    var i, n = (1 << 21);
    var valid = 1, nth = 1296; // 216;
    var bitstring;
    var parityC;
    var parityE;

    var text;
    var optColumn = false;
    var optIndex  = false;
    var optParity = false;
    var args = process.argv.slice(2); // node.js
    for( var iArg = 0; iArg < args.length; ++iArg )
    {
        switch( args[ iArg ] )
        {
            case '-c': optColumn = true; break;
            case '-n': optIndex  = true; break;
            case '-p': optParity = true; break;
            case '-?':
            default:
                console.log( " -c  Disable column markdown" );
                console.log( " -n  Show i'th / n'th column" );
                console.log( " -p  Show Corner and Edge parity" );
                return;
        }
    }

    console.log( "Number of OLL permutations after F2L ..." );
    if( optColumn )
    {
        var head1 = '';
        var head2 = '';

        if( optIndex )
        {           //# 1296/1296 '
            head1 += "|# i'th/n'th";
            head2 += "|----------:";
        }

        //        @  118384 $ 000:01110:01110:01110:000 CP:0 EP:0
        head1 += '| @ ID    | $ Bit String              |';
        head2 += '|--------:|:--------------------------|';

        if( optParity )
        {
            head1 += 'CP |EP ';
            head2 += ':-:|:-:';
        }

        head1 += '|';
        head2 += '|';

        console.log( head1 );
        console.log( head2 );
    }

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

            parityC   = cornerParity ( i );
            parityE   = edgeParity   ( i );
            bitstring = makeBitString( i );

            //if (parityC != 0) continue; // 1296 -> 432
            //if (parityE != 0) continue; //  432 -> 216
         // console.log( "| %s/%d  | %s: | %s |  %d |  %d|", pad(valid,4), nth, pad(i,7), bitstring, parityC, parityE );
         // console.log( "# %s/%d: @ %s: $ %s CP:%d EP:%d" , pad(valid,4), nth, pad(i,7), bitstring, parityC, parityE );

            text = '';

            if( optColumn )
            {
                if( optIndex ) // intentional single then statement
                    text += '| ' + pad(valid,4) + '/' + nth + ' ';
                    text += '| ' + pad(i,7) + ' ';
                    text += '| ' + bitstring+ ' ';
                if( optParity ) // intentional single then statement
                    text += '| ' + parityC + ' | ' + parityC + ' ';
                    text += '|';
            }
            else
            {
                if( optIndex ) // intentional 1-statement
                    text += '# ' + pad(valid,4) + '/' + nth + ' ';
                    text += '@ ' + pad(i,7) + ' ';
                    text += '$ ' + bitstring+ ' ';
                if( optParity )
                    text += 'CP:' + parityC + ' EP:' + parityE;
            }

            console.log ( text );
            valid++;
        }
    }
}

enum_yellow();
