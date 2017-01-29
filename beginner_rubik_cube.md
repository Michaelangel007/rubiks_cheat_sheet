# Table of Contents

* Beginner Cross
  * Place Edges
* Beginner F2L
  * Part 1 - Place Corners
  * Part 2 - Place Edges   RU to RF/RB
* Beginner OLL
  * Part 1 - Rotate
  * Part 2 - Position
* Beginner PLL
  * Part 1 - Position 3 corners
  * Part 2 - Rotate 1 corner in-place
* Summary
* References

========================================================================

# Beginner Cross - Place Edges

Old-school (Top facing up):

  Summary: Place edges in correct _absolute_ position

  Moves:

      TODO:

New-school (Top facing down):

  Summary: Place edges in correct _relative_ position position

  Moves:

      TODO:

========================================================================

# Beginner F2L Part 1 - Place Corners

Summary:

  * Corner: Move RUF to RDF

Note: Top is facing down

When `white` is facing Right, Front, or Up:

|Facing| Axis | Moves                |
|:----:|:----:|:---------------------|
| R    | +X   | `R  U  R'          ` |
| F    | +Z   | `F' U  F           ` | 
| U    | +Y   | `R  U2 R' U' R U R'` |

* https://youtu.be/609nhVzg-5Q?t=543
* https://youtu.be/k-xbcAMfWwM?t=128

------------------------------------------------------------------------

## Beginner F2L - Part 2 - RU to RF/RB

### Old-school (Top facing up):

 |Position |Moves             |
 |:--------|:-----------------|
 |LD to FR | R' D R D F D' F' |

### New-school (Top facing down):

* Middle RU to RF or RB (Top facing down)

 |Position | F2L b Moves                   |
 |:--------|:------------------------------|
 |RU to RF | R' U' R' U' R' U  R  U  R  U  |
 |RU to RB | R  U  R  U  R  U' R' U' R' U' |
 |rotate RF| R  U' R' U  F' U2 F  U2 F' U F|

Alt. Rotate RF

Bob Burton F2L #1
http://www.cubewhiz.com/f2l.php
Corner: FR

    (R U' R') Dw (R' U2) (R U'2) (R' U R)

 or

F2L Cheat Sheet

    R  U2 R' U  R  U2 R' y' R' U' R

Alternate RU to RF:

    R' F' R U R U' R' F

References:

* BadMephisto "Partial Edge Control" http://badmephisto.com/f2l.php
* https://www.reddit.com/r/Cubers/comments/1htp2a/is_partial_edge_control_worth_it/
* https://www.speedsolving.com/wiki/index.php/Partial_Edge_Control
* https://www.speedsolving.com/wiki/index.php/Sledgehammer
* 12:00 https://youtu.be/609nhVzg-5Q?t=720
* F2L Cheat Sheet

------------------------------------------------------------------------

### Beginner F2L Part 2a - RU to RF

Summary:

  * Edge move cycle: UB -> RU -> RF

```
    Before              After

          /\                  /\      
         /  \                /  \     
        /\  /\G             /\  /\O   
       /  \/ Y\            /  \/ Y\   
      /\  /\  /\          /\  /\  /\  
     /  \/  \/  \        /  \/  \/  \ 
    |\  /\y /\  /|      |\  /\y /\  /|
    | \/  \/  \/ |      | \/  \/  \/ |
    | |\  /\ R/| |      | |\  /\ Y/| |
    |\| \/  \/G|/|      |\| \/  \/G|/|
    | + |\  /| + |      | + |\  /| + |
    | |\| \/ |/| |      | |\| \/ |/| |
    |\| + ][ + |/|      |\| + ][ + |/|
    | +r|\][/|g+ |      | +r|\][/|g+ |
    | |\|Y][O|/| |      | |\|R][G|/| |
     \| + ][ + |/        \| + ][ + |/ 
      | |\][/| /          | |\][/| /  
       \| ][ |/            \| ][ |/   
        | ][ /              | ][ /    
         \][/                \][/     
          \/                  \/      
```

  Moves:

    R'
    U' R' U' R'
    U  R  U  R
    U

------------------------------------------------------------------------

### Beginner F2L Part 2b - RU to RF

Summary:

  * Edge move cycle: UB -> RU -> RF

```
    Before              After

          /\                  /\      
         /  \                /  \     
        /\  /\              /\  /\    
       /  \/  \            /  \/  \   
      /\  /\  /\          /\  /\  /\  
     /  \/  \/  \        /  \/  \/  \ 
    |\  /\  /\  /|      |\  /\  /\  /|
    | \/  \/  \/ |      | \/  \/  \/ |
    | |\Y /\ O/| |      | |\Y /\ Y/| |
    |\|G\/  \/G|/|      |\|R\/  \/G|/|
    | + |\  /| +R|Y     | + |\  /| +G|O
    | |\| \/ |/| |      | |\| \/ |/| |
    |\| + ][ + |/|      |\| + ][ + |/|
    | + |\][/| + |      | + |\][/| + |
    | |\| ][ |/| |      | |\| ][ |/| |
     \| + ][ + |/        \| + ][ + |/ 
      | |\][/| /          | |\][/| /  
       \| ][ |/            \| ][ |/   
        | ][ /              | ][ /    
         \][/                \][/     
          \/                  \/
```

  Moves:

    R
    U  R  U  R
    U' R' U' R'
    U'


========================================================================

# Beginner OLL

2-Look OLL Edges Part 1 - Orientate Edges

## 2-Look OLL Edges - Part 1 - Rotate Edges

There are 4 cases of yellow edges facing up:

| # |Shape| OLL a Moves         | Bob Burton |
|:-:|:---:|---------------------|:-----------|
| 4 | `+` | Cont. OLL Part 2    | n/a        |
| 2 | `L` | `F U R U' R' F'   ` | n/a        |
| 2 | `-` | `F   R U  R' U' F'` | #45 'T'    |
| 0 | `.` | use L and -         | n/a        |

## 2-Look OLL Edges - Part 2 - Position Edges

|Shape| OLL b Moves        | Notes            |
|:---:|:-------------------|:-----------------|
| `L` | U R U R' U R U2 R' | 'L' is UF and RU |
| `|` | U R U R' U R U2 R' | Do 'L' twice     |

------------------------------------------------------------------------

### OLL "L"

  Note: 'L' is in top-left LU, RU

  Cycles: 6

```
    Corners             Edges

      ___ ___ ___        ___ ___ ___
     |   |   |   |      |   |   |   |
     | <-------> |      |   | 2 |   |
     |___|___|___|      |___|_^\|___|
     |   |   |   |      |   | | \   |
     |   |   |   |      |   | | |>3 |
     |___|___|___|      |___|_|_/___|
     |   |   |   |      |   | |<|   |
     | <-------> |      |   | 1 |   |
     |___|___|___|      |___|___|___|
      CCW     CCW
```

  Moves:

    F U  R 
      U' R' F'

------------------------------------------------------------------------

### OLL "-" or "-|"

  Cycles: 6

```
    Corners             Edges

      ___ ___ ___        ___ ___ ___
     |   |   |   |      |   |   |   |
     | <-------> |      |   | 3<|   |
     |___|___|___|      |___|_|_\___|
     |   |   |   |      |   | | |\  |
     |   |   |   |      |   | | |>2 |
     |___|___|___|      |___|_|_/___|
     |   |   |   |      |   | v/|   |
     | <-------> |      |   | 1 |   |
     |___|___|___|      |___|___|___|

```

  Moves:

    F R  U 
      R' U' F'


------------------------------------------------------------------------

### OLL Alternate - Part 2 - Helper from PLL - Position Edges

```
    Edges

     ___ ___ ___
    |   |   |   |
    |   |>2\|   |
    |___/__ \___|
    |  /|    >  |
    | 1<------3 |
    |___|___|___|
    |   |   |   |
    |   |   |   |
    |___|___|___|
```
  Moves:

    R' U 
       R' U' 
       R' U' 
    R' U
    R  U R2


========================================================================

# Beginner PLL - Part 1 - Position 3 corners

Number of corners in correct location (may have wrong orientation):

| # | Notes               |
|:-:|:--------------------|
| 0 | Go back to OLL phase|
| 1 | Place in RFU        |
| 2 | Cube unsolvable!    |
| 3 | Cube unsolvable!    |
| 4 | Cont. PLL Part 2    | 

  Summary:

    Corners move cycle: LUB -> LUF -> RUB

```
    Corners

     ___ ___ ___
    |   |   |   |
    | 1<---- >3 |
    |_|_|___/___|
    | | |  /|   |
    | | | / |   |
    |_|_|/__|___|
    | v /   |   |
    | 2/|   |   |
    |___|___|___|
```

  Moves:

    U R  U' L' 
    U R' U' L

  Also see:

    PLL Aa-perm

------------------------------------------------------------------------

# Beginner PLL - Part 2 - Rotate 1 corner in-place

  Notes:

  * Place incorrect corner orientation at RUF
  * Correct corner is at 
  * RUF Yellow faces either
    * Right
    * Front

| RUF Facing | Repeat |
|:-----------|:------:|
| Right      | 1      |
| Front      | 2      |

  Moves:

    R' D' R D
    R' D' R D
    <repeat if yellow is still facing right>
    <U or U' move new incorrect corner into RUF>

========================================================================

# Summary

* F2L

|Facing| Axis | F2L a Moves          |
|:----:|:----:|:---------------------|
| R    | +X   | `R  U  R'          ` |
| F    | +Z   | `F' U  F           ` | 
| U    | +Y   | `R  U2 R' U' R U R'` |

and

|Position | F2L b Moves                   |
|:--------|:------------------------------|
|RU to RF | R' U' R' U' R' U  R  U  R  U  |
|RU to RB | R  U  R  U  R  U' R' U' R' U' |
|rotate RF| R  U' R' U  F' U2 F  U2 F' U F|

* OLL

| # |Shape| OLL a Moves         |
|:-:|:---:|---------------------|
| 2 | `L` | `F U R U' R' F'   ` |
| 2 | `-` | `F   R U  R' U' F'` |

and

|Shape     | OLL b Moves        | Notes            |
|:--------:|:-------------------|:-----------------|
|`\|-`     | U R U R' U R U2 R' | 'L' is UF and RU |
| `|`      | U R U R' U R U2 R' | Do 'L' twice     |
|PLL CW @UF| R' U R' U' R' U' R' U R  U R2 | Cycle UF -> RU -> UB |


* PLL

| Notes                | PLL a and b Moves    |
|:---------------------|:---------------------|
| Corner move CCW @ RUF| U R  U' L' U R' U' L |
| Corner rotate RUF    | R' D' R D R' D' R D  |
| Corner rotate non-RUF| R' U2 R U R' U R U2 then PLL UF |

========================================================================

# References:

* https://solvethecube.com/
