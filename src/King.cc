#include <memory>
#include "King.hh"

King::King(std::shared_ptr<LinCong> gen) :
  fGen(gen), 
  fOrizontalPosition(5),
  fVerticalPosition(1)
{};


void King::Step()
{
  while(true)
    {
      float r = fGen->Get();
      float tot = pUp + pRight + pDown + pLeft;
      if (r < pUp/tot)
	{
	  if(fVerticalPosition<8)
	    {
	      fVerticalPosition++;
	      return;
	    }
	}
      else if (r < (pUp+pRight)/tot)
	{
	  if(fOrizontalPosition<8)
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

std::ostream &operator<<(std::ostream &os, const King &o)
{
  os << "(" << o.OrizontalPosition() << ", " << o.VerticalPosition() << ")";
  return os;
}
