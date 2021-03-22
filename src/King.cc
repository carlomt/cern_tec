#include <memory>
#include "King.hh"

King::King(std::shared_ptr<LinCong> gen,
	   const float weight_up,
	   const float weight_down,
	   const float weight_left,
	   const float weight_right,
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
	      SavePosition();	      
	      return;
	    }
	}
      else if (r < (pUp+pRight)/tot)
	{
	  if(fOrizontalPosition<fBoardSize)
	    {
	      fOrizontalPosition++;
	      SavePosition();	      
	      return;
	    }
	}
      else if (r < (pUp+pRight+pDown)/tot)
	{
	  if(fVerticalPosition>1)
	    {
	      fVerticalPosition--;
	      SavePosition();
	      return;
	    }
	}
      else
	{
	  if(fOrizontalPosition>1)
	    {
	      fOrizontalPosition--;
	      SavePosition();
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
  SavePosition();
  return;
}

void King::SavePosition()
{
  fOrizontalPositions.push_back(fOrizontalPosition);
  fVerticalPositions.push_back(fVerticalPosition);
}

std::ostream &operator<<(std::ostream &os, const King &o)
{
  os << "(" << o.OrizontalPosition() << ", " << o.VerticalPosition() << ")";
  return os;
}
