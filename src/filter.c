#include "filter.h"
int8_t filter_cnt = 0;
int16_t filter_buf[SHIFT_NUM];

int16_t iirLPF(int16_t in){
  int16_t out=0;
  int8_t i;
  if(filter_cnt < SHIFT_NUM){
    filter_buf[filter_cnt++]=in;
    out=in;
  }else{
    for(i=0;i<SHIFT_NUM-1;i++){
      filter_buf[i]=filter_buf[i+1];
      out+=filter_buf[i]
    }
    filter_buf[i]=in;
    out+=filter_buf[i];
    out/=SHIFT_NUM;
  }

  return out;
}
