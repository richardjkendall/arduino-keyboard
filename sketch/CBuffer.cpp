#include "CBuffer.h"

const int buf_size = 3;

CBuffer::CBuffer() {
  pointer = 0;
  consumed = 0;
  int i;
  for(i = 0;i < buf_size;i++) {
    buf[i] = 0;
  }
}

bool CBuffer::add(long val) {
  buf[consumed % buf_size] = val;
  consumed++;
  return true;
}

long CBuffer::avg() {
  long total = 0;
  int i;
  for(i = 0;i< buf_size;i++) {
    total += buf[i];
  }
  return total / buf_size;
}
