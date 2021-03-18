#ifndef KING_HH
#define KING_HH

#include <iostream>
#include "LinCong.hh"

class King{
public:
  King(std::shared_ptr<LinCong> gen);

  inline unsigned int OrizontalPosition() const {return fOrizontalPosition;};
  inline unsigned int VerticalPosition() const {return fVerticalPosition;};

  void Step();
  
  friend std::ostream &operator << (std::ostream &os, const King &o); //for debbuging
  
private:
  unsigned int fOrizontalPosition, fVerticalPosition;
  std::shared_ptr<LinCong> fGen;
  const float pUp = 5., pRight=2., pDown=1., pLeft=3.;
};
#endif
