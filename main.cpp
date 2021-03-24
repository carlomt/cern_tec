#include <memory>
#include <iostream>
#include <cmath>
#include <stdexcept>

#ifndef __WITHOUT_ROOT__
#include "TCanvas.h"
#include "TH1F.h"
#endif

#include "LinCong.hh"
#include "King.hh"

using std::cout;
using std::endl;
using std::cerr;

int main(int argc, char *argv[])
{
  int seed = 1234;
  const size_t n_max_steps = 150;
  const size_t n_events = 10000;
  const float weight_up=5.;
  const float weight_down=1.;
  const float weight_left=3.;
  const float weight_right=2.;
  // const float weight_up=1.;
  // const float weight_down=1.;
  // const float weight_left=1.;
  // const float weight_right=1.;  
  const float tau = 10./std::log(2.);
  const bool decay = false;
  const bool periodic_boundary_condition = false;
  const unsigned int chess_board_size = 8;
  const unsigned int vertical_winning_position = 8;
    
  int n_run = 10;
  int n_events_per_run;

  if(n_events%n_run)
    {
      throw std::length_error("It is not possible divide "+std::to_string(n_events)
			      +" events in "+std::to_string(n_run)+" runs.");
    }
  n_events_per_run = n_events/n_run;
  cout<<"dividing "<<n_events<<" events in "<<n_run<<" runs with "<<n_events_per_run<<" events per run."<<endl;

  //the prng is instantiated dynamically to share the same object in all the code
  //for the same reason I am using a shared pointer
  auto gen = std::make_shared<LinCong>(seed);

  //the histogram is allocated dynamically because of root garbage collection
  //but it is a unique pointer to save memory and avoid memory leaks
#ifndef __WITHOUT_ROOT__
  int nbin = 100;
  auto h = std::make_unique<TH1F>("h", "h", nbin, 0., n_max_steps);
#endif
  float nstep_to_success = 0;
  float nstep_to_success2 = 0;
  size_t nsuccess = 0;
  float sigma = 0.;
  float sigma2 = 0.;
  float min_number_of_steps = 0.;
  float min_number_of_steps2 = 0.;
  float n_failed = 0.;
  float n_failed2 = 0.;
  
  cout<<"Starting to simulate"<<endl;
  //loop on the different runs
  for(size_t run=0; run<n_run; ++run)
    {
      float nstep_to_success_thisrun = 0.;
      float nstep_to_success2_thisrun = 0.;
      size_t nsuccess_thisrun = 0;
      size_t min_number_of_steps_thisrun = n_max_steps;
      size_t n_steps = n_max_steps;
      
      for(size_t event=0; event<n_events_per_run; ++event)
	{
	  King king(gen, weight_up, weight_down,
		    weight_left, weight_right,
		    chess_board_size);
	  king.SetPeriodicBoundariesCondition(periodic_boundary_condition);

	  if(decay)
	    {
	      n_steps = (size_t) std::floor(-tau*std::log(1. - gen->Get()));
	    }
	  for(size_t step=0; step<n_steps; ++step)
	    {
	      king.Step();
	      if(king.OrizontalPosition()==1 && king.VerticalPosition()==vertical_winning_position)
		{
		  step++;
		  nstep_to_success_thisrun += step;
		  nstep_to_success2_thisrun += step*step;
		  nsuccess_thisrun++;
		  if( step < min_number_of_steps_thisrun )
		    {
		      min_number_of_steps_thisrun = step;
		      const auto& vp = king.VerticalPositions();
		      const auto& op = king.OrizontalPositions();
		    }
#ifndef __WITHOUT_ROOT__		  
		  h->Fill(step);
#endif
		  break;
		}
	    }//end loop on steps
	}//end loop on events

      nstep_to_success_thisrun /= nsuccess_thisrun;
      nstep_to_success2_thisrun /= nsuccess_thisrun;

      const int n_failed_thisrun = (n_events_per_run - nsuccess_thisrun);
      n_failed  += n_failed_thisrun;
      n_failed2 += n_failed_thisrun*n_failed_thisrun;
	
      float sigma_thisrun = std::sqrt(nstep_to_success2_thisrun -
				      nstep_to_success_thisrun*nstep_to_success_thisrun);
      cout<<"Run "<<run<<"\n";
      cout<<"  number of successes:  "<<nsuccess_thisrun<<"\n";
      cout<<"  mean number of steps: "<<nstep_to_success_thisrun<<"\n";
      cout<<"  var:                  "<<sigma_thisrun<<"\n";
      cout<<"  min number of steps:  "<< min_number_of_steps_thisrun << endl;
      
      nstep_to_success     += nstep_to_success_thisrun;
      nstep_to_success2    += nstep_to_success_thisrun*nstep_to_success_thisrun;      
      sigma                += sigma_thisrun;
      sigma2               += sigma_thisrun*sigma_thisrun;
      nsuccess             += nsuccess_thisrun;
      min_number_of_steps  += min_number_of_steps_thisrun;
      min_number_of_steps2 += min_number_of_steps_thisrun*min_number_of_steps_thisrun;
    }//end loop on runs
  
    nstep_to_success     /= n_run;
    nstep_to_success2    /= n_run;
    sigma                /= n_run;
    sigma2               /= n_run;
    min_number_of_steps  /= n_run;
    min_number_of_steps2 /= n_run;
    n_failed             /= n_run;
    n_failed2            /= n_run;

    float err_nstep_to_success = std::sqrt(nstep_to_success2 - nstep_to_success*nstep_to_success);
    float err_sigma = std::sqrt(sigma2 - sigma*sigma);
    float err_min_number_of_steps = std::sqrt(min_number_of_steps2 - min_number_of_steps*min_number_of_steps);
    float err_n_failed = std::sqrt(n_failed2 - n_failed*n_failed);
    cout<<"####################### Results #######################\n";
    cout<<"  number of successes:  "<< nsuccess <<"\n";
    cout<<"  mean number of steps: "<< nstep_to_success <<" +/- "<< err_nstep_to_success <<"\n";
    cout<<"  var:                  "<< sigma <<" +/- "<< err_sigma <<"\n";
    cout<<"  min number of steps:  "<< min_number_of_steps <<" +/- "<< err_min_number_of_steps << "\n";
    cout<<"  n failed:             "<< n_failed <<" +/- "<< err_n_failed << endl;    
    cout<<"#######################################################\n";

#ifndef __WITHOUT_ROOT__    
    auto cc = std::make_unique<TCanvas>("cc","cc");
    cc->SetLogy();
    h->Draw();
    cc->SaveAs("h.pdf");
    cc->SaveAs("h.C");
#endif
    
    return 0;
}
