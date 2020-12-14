/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  // Apply De Morgan's laws: (x & ~y) | (~x & y)
  // return ~(~(x & ~y) & ~(~x & y)); // Costing 8 operators.

  // Thinking in different way, performing not operation on XNOR gate: ~((x & y) | (~x & ~y))
  return ~(x & y) & ~(~x & ~y); // Costing only 7 operators.
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return (1 << 31);
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  // Adding 0x01 to variable and determining is it Tmin or not.
  // (Tmin => left shift 1 bit position would produce 0, except for 0)
  x = x + 0x01;
  // return !(x + x) & !(!x); // Applying De Morgan's law.
  return !((x + x) | (!x));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  // Retaining all the odd bits and determining is it equivalent to 0xAAAAAAAA.
  int mask = 0xAA;
  mask = (mask << 8) + mask;
  mask = (mask << 16) + mask;
  return !((x & mask) ^ mask);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  // 2's complement negation.
  return ~x + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  // AsciiDigit x in range of 0x30 ~ 0x39
  // x - 0x30 >= 0
  // 0x39 - x >= 0
  // return !(((x + ~0x2F) >> 31) | ((0x3A + ~x) >> 31));
  return !(((x + ~0x2F) | (0x3A + ~x)) >> 31);
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /* Mask for variable y (generating process):
   * 1. x                   = non-zero  or  0
   * 2. !x                  = 0         or  1
   * 3. Adding Tmax(0xF..F) = 0xF..F    or  0x0..0
   */
  int mask_x = !x + (~0x00);
  return (mask_x & y) + ((~mask_x) & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  // Checking for only 2 situations:
  // 1. If they have different sign, if so, is x negative? (part a.)
  // 2. They have the same sign, determining if y-x is negative or not? (part b.)
  // Combining part a. and part b.

  // int diff_sign = !(x >> 31) ^ !(y >> 31);
  // int a = diff_sign & (x >> 31);
  // int b = !diff_sign & !((y + (~x) + 1) >> 31); 
  // return a | b;

  int diff_sign = x ^ y;
  int a = diff_sign & x;
  int b = ~(diff_sign | (y + (~x) + 1)); 
  return ((a | b) >> 31) & 0x01;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  // 1. Negative (>>31)=> 0xf..f (adding 1)=> 0
  // 2. Positive (transforming to negative)
  // 3. Zero (negate)=> still 0 (adding 1)=> 1
  // This method aims to spread the sign bit (1) to the whole variable for non-zero input. 
  // And adding 1 to -1 (0xF..F) produces 0. On the other hand, adding 1 to 0 produces 1.
  // return ((x >> 31) | ((~x + 1) >> 31)) + 1;
  return ((x | (~x + 1)) >> 31) + 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int c, m1, m2, m3, m4, m5;

  // Performing bitwise negation on negative value (Non-negative values don't change). Example: -1 => 0
  x = x ^ (x>>31);

  // Set all bits that are lower than highest set bit to 1. Example: 0x50 => 0x7F
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);

  // Bit count (summing all the bits)
  // m1 = 0x55555555
  // m2 = 0x33333333
  // m3 = 0x0F0F0F0F
  // m4 = 0x00FF00FF
  // m5 = 0x0000FFFF
  m1 = (0x55 << 8) + 0x55;
  m1 = (m1 << 16) + m1;
  m2 = (0x33 << 8) + 0x33;
  m2 = (m2 << 16) + m2;
  m3 = (0x0F << 8) + 0x0F;
  m3 = (m3 << 16) + m3;
  // m4 = (0xFF << 16) + 0xFF;
  // m5 = (0xFF << 8) + 0xFF;

  // Counting set bits (Naive version)
  // c = (x & m1) + ((x >> 1) & m1);
  // c = (c & m2) + ((c >> 2) & m2);
  // c = (c & m3) + ((c >> 4) & m3);
  // c = (c & m4) + ((c >> 8) & m4);
  // c = (c & m5) + ((c >> 16)& m5);

  // Counting set bits (Version 2: Ignoring the parts that won't affect the result 
  // and performing AND operation to remove those carrying part at the end of this process)
  c = (x & m1) + ((x >> 1) & m1);
  c = (c & m2) + ((c >> 2) & m2);
  c = (c + (c >> 4)) & m3;
  c += (c >> 8);
  c += (c >> 16);
  c &= 0x3F;

  // Adding the extra sign bit. 
  return c + 1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned int sign = uf >> 31; // unsigned => Logical shift
  unsigned int exp = (uf >> 23) & 0xFF;
  unsigned int frac = uf & ~(0x1ff << 23);
  
  // Denormalized. 
  if(exp == 0x00){
    frac <<= 1;
    // Jumping to normalized section (leading 1 in formula). 
    if(frac & 0x800000){
      frac &= 0x7FFFFF;
      exp = 0x01;
    }
  }
  // Special number. 
  else if(exp == 0xFF){
    // Do nothing! 
  }
  // Normalized. 
  else{
    exp++;
  }

  return (sign << 31) | (exp << 23) | frac;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  // In C and C++, the rule of casting from floating point to integer is rounding off. 
  // (not rounding up or rounding to nearest even)
  unsigned int sign = uf >> 31; // unsigned => Logical shift
  int exp = (uf >> 23) & 0xFF;
  unsigned int frac = uf & ~(0x1ff << 23);
  int result, E, Bias = 127; 

  // Exponent to small. (rounding to 0)
  if(exp < Bias){
    result = 0;
  }
  // Two cases:
  // 1. Exponent too large.
  // 2. Negative value: - 2^31.
  else if(exp - Bias >= 31){
    result = 0x80000000u;
  }
  else{
    // The result has been already been shifted left by 23 bits position. (*2^23)
    result = 0x800000 + frac; // Combining with fraction.
    E = exp - Bias - 23;

    if(E > 0){
      result <<= E;
    }
    else if(E < 0){
      result >>= (-E);
    }
    // Dealing with sign. Tmin (overflew or not) would still be converted to Tmin.
    if(sign) result = -result;
  }

  return result;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  // int sign = 0;
  int Bias = 127;
  int exp = x + Bias;
  unsigned result;

  // Exponent too large to represent. (Infinity)
  if(exp > 254){
    exp = 0xFF;
    result = exp << 23;
  }
  // Exponent too small to represent. (0)
  else if(exp <= -23){
    result = 0;
  }
  else if(exp > 0){
    result = exp << 23;
  }
  // Denormalized. (Non-zero fraction: 0.xxxxxxx)
  else{
    result = 1 << (22 + exp);
  }

  return result;
}
