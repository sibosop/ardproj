#if 0
#ifndef TRAP_H
#define TRAP_H
class Trap {
public:
  Trap() 
    : count(0)
    , swap(0) 
    , offset(0)
    {}
  void display();
private:
  static const Pos positions[];
  static const Color colors[];
  unsigned int count;
  unsigned int swap;
  unsigned int offset;
};
#endif
#endif