#ifndef KING_HH
#define KING_HH

#include <iostream>
#include <vector>
#include "LinCong.hh"

class King{
public:
  King(std::shared_ptr<LinCong> gen,
       const float weight_up=1.,
       const float weight_down=1.,
       const float weight_left=1.,
       const float weight_right=1.,
       const unsigned int board_size=8);

  inline unsigned int OrizontalPosition() const {return fOrizontalPosition;};
  inline unsigned int VerticalPosition() const {return fVerticalPosition;};
  inline const std::vector<unsigned int>& OrizontalPositions() const {return fOrizontalPositions;};
  inline const std::vector<unsigned int>& VerticalPositions() const {return fVerticalPositions;};  

  inline void SetPeriodicBoundariesCondition(const bool val) {fPeriodicBoundary=val;};
  
  void Step();
  
  friend std::ostream &operator << (std::ostream &os, const King &o); //for debbuging
  
private:
  unsigned int fOrizontalPosition, fVerticalPosition;
  std::shared_ptr<LinCong> fGen;
  // const float pUp = 5., pRight=2., pDown=1., pLeft=3.;
  const float pUp, pRight, pDown, pLeft;
  const unsigned int fBoardSize;
  bool fPeriodicBoundary;
  std::vector<unsigned int> fOrizontalPositions;
  std::vector<unsigned int> fVerticalPositions;  
  
  void StepWithPeriodicBoundary();
  void SavePosition();
};
#endif
