#ifndef LINCONG_HH
#define LINCONG_HH

class LinCong{
public:
  LinCong(const unsigned long int seed) : fX(seed) {};
  virtual ~LinCong() = default;
  
  virtual float Get()
  {
    fX = (fA * fX + fC ) % fM;    
    return (float)fX/fM;
  }

  
protected:
  unsigned long int fX; //long to have at least 32 bit
  const unsigned long int fA = 1664525;
  const unsigned long int fC = 1013904223;
  const unsigned long int fM = 4294967296; //2^32
};
#endif
