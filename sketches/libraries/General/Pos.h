#ifndef POS
#define POS
#include <stdint.h>
class Pos
{
public:
  uint8_t row;
  uint8_t col;
  Pos(int r, int c)
    : row(r)
    , col(c)
    {}
  Pos()
    : row(0)
    , col(0)
    {}
  Pos(const Pos& p)
    : row(p.row)
    , col(p.col)
    {}
  bool operator==(const Pos& p) const
  {
    return (p.row==row && p.col==col);
  }
};

extern const Pos *  number[10] ;

#endif