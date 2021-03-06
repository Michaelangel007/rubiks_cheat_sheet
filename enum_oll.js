"use strict";

/*
    Last layer bottom facing up:

        20  19  18
       ___ ___ ___
      |   |   |   |
    17| 16| 15| 14|13
      |___|___|___|
      |   |   |   |
    12| 11| 10| 9 | 8
      |___|___|___|
      |   |   |   |
     7| 6 | 5 | 4 | 3
      |___|___|___|
        2   1   0

*/

    // 21 faces
    var SHIFT_LUB_nZ =  20;
    var SHIFT__UB_nZ =  19;
    var SHIFT_RUB_nZ =  18;

    var SHIFT_LUB_nX =  17;
    var SHIFT_LUB_pY =  16;
    var SHIFT__UB_pY =  15;
    var SHIFT_RUB_pY =  14;
    var SHIFT_RUB_pX =  13;

    var SHIFT_LU__nX =  12;
    var SHIFT_LU__pY =  11;
    var SHIFT_MU__pY =  10;
    var SHIFT_RU__pY =   9;
    var SHIFT_RU__pX =   8;

    var SHIFT_LUF_nX =   7;
    var SHIFT_LUF_pY =   6;
    var SHIFT__UF_pY =   5;
    var SHIFT_RUF_pY =   4;
    var SHIFT_RUF_pX =   3;

    var SHIFT_LUF_pZ =   2;
    var SHIFT__UF_pZ =   1;
    var SHIFT_RUF_pZ =   0;

/*
 * @param {Number} i - State between 0 and 2^21-1
 * @return {String} 21 faces as a bit string
 */
function makeBitString( i )
{
    var text = new Array(21);
    var mask = 1 << SHIFT_LUB_nZ;

    for( var face = 0; face < 21; ++face, mask >>= 1 )
        text[ face ] = (i & mask) ? '1' : '0';

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

        parity += ((state >> SHIFT_LUB_nZ) & 1)*2; // LUB CCW
        parity += ((state >> SHIFT_LUB_nX) & 1)  ; // LUB CW

        parity += ((state >> SHIFT_RUB_nZ) & 1)  ; // RUB CW
        parity += ((state >> SHIFT_RUB_pX) & 1)*2; // RUB CCW

        parity += ((state >> SHIFT_LUF_nX) & 1)*2; // LUF CCW
        parity += ((state >> SHIFT_LUF_pZ) & 1)  ; // LUF CW

        parity += ((state >> SHIFT_RUF_pX) & 1)  ; // RUF CW
        parity += ((state >> SHIFT_RUF_pZ) & 1)*2; // RUF CCW

    return parity % 3; // Valid == 0
}

/*
 * @param {Number} state - Last Layer k-factoridic state of cube: 0 .. 2^21
 */
// ========================================================================
function edgeParity( state )
{
    var parity = 0;

        parity += ((state >> SHIFT__UB_nZ) & 1);
        parity += ((state >> SHIFT_LU__nX) & 1);
        parity += ((state >> SHIFT_RU__pX) & 1);
        parity += ((state >> SHIFT__UF_pZ) & 1);

    return parity % 2; // Valid == 0
}

/*
 * Right align text
 */
// ========================================================================
function pad( text, width, spacer )
{
    if (!spacer)
        spacer = ' ';
    return (new Array( width ).join( spacer ) + text).slice( -width );
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
*/
function enum_yellow()
{
    var state, last = (1 << 21);

    var ith = 1, nth = 1296; // 216;
    var bitstring;
    var parityC;
    var parityE;

    var text;
    var optColumn = false; // Markdown table column format
    var optImage  = false;
    var optIndex  = false;
    var optParity = false;
    var args = process.argv.slice(2); // node.js
    for( var iArg = 0; iArg < args.length; ++iArg )
    {
        switch( args[ iArg ] )
        {
            case '-c': optColumn = true; break;
            case '-i': optImage  = true; break;
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

        if( optImage )
        {          //  ![118384](pics/perm/oll_118384.png)
            head1 += '| Image ';
            head2 += '|:------';
        }

        head1 += '|';
        head2 += '|';

        console.log( head1 );
        console.log( head2 );
    }

    for( state = 0; state < last; ++state )
    {
        // if middle is not yellow then cube is not valid
        if( (state >> SHIFT_MU__pY) & 1 )
        {
            // Count # of yellow faces in each corner
            if( invalidCorner( state, SHIFT_LUB_nZ, SHIFT_LUB_pY, SHIFT_LUB_nX ) ) continue; // LUB
            if( invalidCorner( state, SHIFT_RUB_nZ, SHIFT_RUB_pY, SHIFT_RUB_pX ) ) continue; // RUB
            if( invalidCorner( state, SHIFT_LUF_pZ, SHIFT_LUF_pY, SHIFT_LUF_nX ) ) continue; // LUF
            if( invalidCorner( state, SHIFT_RUF_pZ, SHIFT_RUF_pY, SHIFT_RUF_pX ) ) continue; // RUF

            // Count # of yellow faces on each edge
            if( invalidEdge( state, SHIFT__UB_nZ, SHIFT__UB_pY ) ) continue; // UB
            if( invalidEdge( state, SHIFT_LU__nX, SHIFT_LU__pY ) ) continue; // LU
            if( invalidEdge( state, SHIFT_RU__pX, SHIFT_RU__pY ) ) continue; // RU
            if( invalidEdge( state, SHIFT__UF_pZ, SHIFT__UF_pY ) ) continue; // UB

            parityC   = cornerParity ( state );
            parityE   = edgeParity   ( state );
            bitstring = makeBitString( state );

            // TODO: FIXME:
            // * Remove bad Corner Parity
            // * Remove bad Edge Parity
            // * Annotate Reflections
            // * Annotate Rotations
            //if (parityC != 0) continue; // 1296 -> 432
            //if (parityE != 0) continue; //  432 -> 216

         // console.log( "| %s/%d  | %s: | %s |  %d |  %d|", pad(ith,4), nth, pad(state,7), bitstring, parityC, parityE );
         // console.log( "# %s/%d: @ %s: $ %s CP:%d EP:%d" , pad(ith,4), nth, pad(state,7), bitstring, parityC, parityE );
            text = '';

            if( optColumn )
            {
                if( optIndex ) // intentional single then statement
                    text += '| ' + pad( ith  , 4 ) + '/' + nth + ' ';
                    text += '| ' + pad( state, 7 ) + ' ';
                    text += '| ' + bitstring+ ' ';
                if( optParity ) // intentional single then statement
                    text += '| ' + parityC + ' | ' + parityC + ' ';
                if( optImage ) // intentional single then statement
                    text += '| ![' + state + '](pics/perm/oll_' + pad( state, 7, '0' ) + '.png)' + ' ';
                    text += '|';
            }
            else
            {
                if( optIndex ) // intentional 1-statement
                    text += '# ' + pad( ith  , 4 ) + '/' + nth + ' ';
                    text += '@ ' + pad( state, 7 ) + ' ';
                    text += '$ ' + bitstring+ ' ';
                if( optParity )
                    text += 'CP:' + parityC + ' EP:' + parityE;
            }

            console.log ( text );
            ith++;
        }
    }
}

enum_yellow();
