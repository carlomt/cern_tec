#include <memory>
#include <iostream>
#include <cmath>

#include "TCanvas.h"
#include "TH1F.h"

#include "LinCong.hh"
#include "King.hh"

using std::cout;
using std::endl;
using std::cerr;

int main(int argc, char *argv[])
{
  int seed = 1234;
  const size_t n_steps = 250;
  const size_t n_events = 10000;
  const float tau = 10./std::log(2);
  const bool decay = false;
  
  auto gen = std::make_shared<LinCong>(seed);

  auto h = std::make_unique<TH1F>("h","h",250,1,250);
  float nstep_to_success = 0;
  float nstep_to_success2 = 0;
  size_t nsuccess = 0;  
  for(size_t event=0; event<n_events; ++event)
    {
      King king(gen);
      float death_time = -tau*std::log(1. - gen->Get());
      for(size_t step=0; step<n_steps; ++step)
	{
	  if(decay && step>death_time)
	    break;
	  king.Step();
	  if(king.OrizontalPosition()==1 && king.VerticalPosition()==8)
	    {
	      cout<<"event: "<<event<<" king at: "<<king<<" after "<<step<<" steps \n";
	      nstep_to_success += step;
	      nstep_to_success2 += step*step;
	      nsuccess++;
	      h->Fill(step);	      
	      break;
	    }
	}
    }
  nstep_to_success /= nsuccess;
  nstep_to_success2 /= nsuccess;
  float sigma = std::sqrt(nstep_to_success2 - nstep_to_success*nstep_to_success);

  cout<<"number of successes: "<<nsuccess<<endl;
  cout<<"mean number of steps: "<<nstep_to_success<<endl;
  cout<<"stat err: "<<sigma<<endl;  
  cout<<"stat err: "<<sigma/nstep_to_success*100<<" %"<<endl;  

  auto cc = std::make_unique<TCanvas>("cc","cc");
  h->Draw();
  cc->SaveAs("h.pdf");
  return 0;
}
