#ifndef TRAP_H
#define TRAP_H
class Trap {
public:
  Trap() 
    : count(0)
    , swap(0) 
    {}
  void display();
private:
  static const Pos positions[4];
  static const Color colors[4];
  unsigned int count;
  unsigned int swap;
};
#endif