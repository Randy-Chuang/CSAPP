#line 145 "bits.c"
int bitXor(int x, int y) {
  int g1=  ~(x &( ~y));
  int g2=  ~((~x) & y);
  return ~(g1 & g2);
}
#line 156
int tmin(void) {
  return (1 << 31);
}
#line 167
int isTmax(int x) {

  x = x + 0x01;
  return !((x + x) |( !x));
}
#line 180
int allOddBits(int x) {
  int mask=  0xAA;
  mask =( mask << 8) + mask;
  mask =( mask << 16) + mask;
  return !((x & mask) ^ mask);
}
#line 193
int negate(int x) {
  return ~x + 1;
}
#line 206
int isAsciiDigit(int x) {
  int a=  !((x >> 4) ^ 0x03);
  int b=(((  x & 0x0E) +( ~0x09)) >> 31);
  return a & b;
}
#line 218
int conditional(int x, int y, int z) {
#line 224
  int mask_x=  !x +( ~0x00);
  return (mask_x & y) +(( ~mask_x) & z);
}
#line 234
int isLessOrEqual(int x, int y) {
#line 240
  int diff_sign=  x ^ y;
  int a=  diff_sign & x;
  int b=  ~(diff_sign |( y +( ~x) + 1));
  return ((a | b) >> 31) & 0x01;
}
#line 254
int logicalNeg(int x) {
#line 259
  return ((x |( ~x + 1)) >> 31) + 1;
}
#line 273
int howManyBits(int x) {
  int c;int m1;int m2;int m3;int m4;int m5;


  x = x ^( x>>31);


  x |=( x >> 1);
  x |=( x >> 2);
  x |=( x >> 4);
  x |=( x >> 8);
  x |=( x >> 16);


  m1 =( 0x55 << 8) + 0x55;
  m1 =( m1 << 16) + m1;
  m2 =( 0x33 << 8) + 0x33;
  m2 =( m2 << 16) + m2;
  m3 =( 0x0F << 8) + 0x0F;
  m3 =( m3 << 16) + m3;
  m4 =( 0xFF << 16) + 0xFF;
  m5 =( 0xFF << 8) +   0xFF;

  c =( x & m1) +(( x >> 1) & m1);
  c =( c & m2) +(( c >> 2) & m2);
  c =( c & m3) +(( c >> 4) & m3);
  c =( c & m4) +(( c >> 8) & m4);
  c =( c & m5) +(( c >> 16)& m5);


  return c + 1;
}
#line 317
unsigned floatScale2(unsigned uf) {
  unsigned     sign=  uf >> 31;
  unsigned     exp=(  uf >> 23) & 0xFF;
  unsigned     frac=  uf & ~(0x1ff << 23);

  if (exp== 0x00) {
    frac <<= 1;

    if (frac& 0x800000) {
      frac &= 0x7FFFFF;
      exp = 0x01;
    }
  }
  else if (exp== 0xFF) {

  }
  else {
    exp++;
  }

  return (sign << 31) |( exp << 23) | frac;
}
#line 351
int floatFloat2Int(unsigned uf) {

  unsigned     sign=  uf >> 31;
  int exp=(  uf >> 23) & 0xFF;
  unsigned     frac=  uf & ~(0x1ff << 23);
  int result;int E;int Bias=127;

  if (exp< Bias) {
    result = 0;
  }
  else if (exp- Bias > 31) {
    result = 0x80000000u;
  }
  else {

    result = 0x800000 + frac;
    E = exp - Bias - 23;

    if (E> 0) {
      result <<= E;
    }
    else if (E< 0) {
      result >>=( -E);
    }


    if (sign) result= -result;
  }
  return result;
}
#line 394
unsigned floatPower2(int x) {

  int Bias=  127;
  int exp=  x + Bias;
  unsigned result;

  if (exp> 254) {
    exp = 0xFF;
    result = exp << 23;
  }
  else if (exp<= -23) {
    result = 0;
  }
  else if (exp> 0) {
    result = exp << 23;
  }
  else {
    result = 1 <<( 22 + exp);
  }

  return result;
}
