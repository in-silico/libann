
#ifndef IMG_EXTRA
#define IMG_EXTRA

#include <cassert>
#include <cstdio>
#include <cmath>
#include "img_lib.h"

struct cpx{
  cpx(){}
  cpx(double aa):a(aa){}
  cpx(double aa, double bb):a(aa),b(bb){}
  double a;
  double b;
  double modsq(void) const {
    return a * a + b * b;
  }
  cpx bar(void) const{
    return cpx(a, -b);
  }
};

cpx operator +(cpx a, cpx b){
  return cpx(a.a + b.a, a.b + b.b);
}

cpx operator *(cpx a, cpx b){
  return cpx(a.a * b.a - a.b * b.b, a.a * b.b + a.b * b.a);
}

cpx operator /(cpx a, cpx b){
  cpx r = a * b.bar();
  return cpx(r.a / b.modsq(), r.b / b.modsq());
}

cpx EXP(double theta){
  return cpx(cos(theta),sin(theta));
}

const double two_pi = 4 * acos(0);

// in:     input array
// out:    output array
// step:   {SET TO 1} (used internally)
// size:   length of the input/output {MUST BE A POWER OF 2}
// dir:    either plus or minus one (direction of the FFT)
// RESULT: out[k] = \sum_{j=0}^{size - 1} in[j] * exp(dir * 2pi * i * j * k / size)
void FFT(cpx *in, cpx *out, int step, int size, int dir){
  if(size < 1) return;
  if(size == 1){
    out[0] = in[0];
    return;
  }
  FFT(in, out, step * 2, size / 2, dir);
  FFT(in + step, out + size / 2, step * 2, size / 2, dir);
  for(int i = 0 ; i < size / 2 ; i++){
    cpx even = out[i];
    cpx odd = out[i + size / 2];
    out[i] = even + EXP(dir * two_pi * i / size) * odd;
    out[i + size / 2] = even + EXP(dir * two_pi * (i + size / 2) / size) * odd;
  }
}

// returns log_2(n) +1;
int log_2(int n){
   int index = 0;
   while(n){
      index++;
      n>>=1;
   }
   return index;
}

void FFT_image(Image<cpx> &org, Image<cpx> &dest){
   dest = org;
   dest.x = (org.x == (1<<(log_2(org.x))-1))? org.x : 1<<(log_2(org.x));
   dest.y = (org.y == (1<<(log_2(org.y))-1))? org.y : 1<<(log_2(org.y));
   for(int i = 0; i < org.rows; ++i)
      for(int j = org.x ; j < dest.x ; ++j) 
         dest(i,j) = 0;
   for(int i = 0; i < org.cols; ++i)
      for(int j = org.y; j < dest.y; ++j)
         dest(i,j) = 0;
         
   for(int i = 0; i < dest.rows ; ++i)
      FFT(dest.getRow(i),dest.getRow(i), 1, dest.rows, 1);
   
         
   
}

#endif
