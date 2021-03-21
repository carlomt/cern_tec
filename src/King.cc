#include <memory>
#include "King.hh"

King::King(std::shared_ptr<LinCong> gen,
	   const float weight_up,
	   const float weight_down,
	   const float weight_right,
	   const float weight_left,
	   const unsigned int board_size) :
  fGen(gen), 
  fOrizontalPosition(5),
  fVerticalPosition(1),
  pUp(weight_up),
  pDown(weight_down),
  pLeft(weight_left),
  pRight(weight_right),
  fBoardSize(board_size),
  fPeriodicBoundary(false)
{};


void King::Step()
{
  if(fPeriodicBoundary)
    {
      return StepWithPeriodicBoundary();
    }
  while(true)
    {
      float r = fGen->Get();
      float tot = pUp + pRight + pDown + pLeft;
      if (r < pUp/tot)
	{
	  if(fVerticalPosition<fBoardSize)
	    {
	      fVerticalPosition++;
	      return;
	    }
	}
      else if (r < (pUp+pRight)/tot)
	{
	  if(fOrizontalPosition<fBoardSize)
	    {
	      fOrizontalPosition++;
	      return;
	    }
	}
      else if (r < (pUp+pRight+pDown)/tot)
	{
	  if(fVerticalPosition>1)
	    {
	      fVerticalPosition--;
	      return;
	    }
	}
      else
	{
	  if(fOrizontalPosition>1)
	    {
	      fOrizontalPosition--;
	      return;
	    }
	}
    }//end loop
}

void King::StepWithPeriodicBoundary()
{

  float r = fGen->Get();
  float tot = pUp + pRight + pDown + pLeft;
  if (r < pUp/tot)
    {
      fVerticalPosition++;
      if(fVerticalPosition>fBoardSize)
	{
	  fVerticalPosition = 1;
	}
    }
  else if (r < (pUp+pRight)/tot)
    {
      fOrizontalPosition++;
      if(fOrizontalPosition>fBoardSize)
	{
	  fOrizontalPosition = 1;
	}
    }
  else if (r < (pUp+pRight+pDown)/tot)
    {
      fVerticalPosition--;
      if(fVerticalPosition<1)
	{
	  fVerticalPosition = fBoardSize;
	}
    }
  else
    {      
      fOrizontalPosition--;
      if(fOrizontalPosition<1)
	{
	  fOrizontalPosition = fBoardSize;
	}
    }
  return;
}

std::ostream &operator<<(std::ostream &os, const King &o)
{
  os << "(" << o.OrizontalPosition() << ", " << o.VerticalPosition() << ")";
  return os;
}
