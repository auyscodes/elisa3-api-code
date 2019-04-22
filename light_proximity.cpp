
#include <chrono>
#include <thread>
#include <iostream>
using namespace std;


#include <elisa3_lib.hpp>
using namespace elisa3_lib;



int main() {

  // Setup cluster.
  ClusterOptions cluster_opts;
  cluster_opts.agent_addresses = {3558,3572,3607};
  cluster_opts.comms_options.comms_mode = POLLING;
  cluster_opts.comms_options.poll_rate = 30;

  cout << "Initializing cluster..." << endl;
  Cluster cluster(cluster_opts);

  cluster.setObstacleAvoidanceEnabled(true);
  cluster.setCliffAvoidanceEnabled(false);
  cluster.setLeftSpeed(15);
  cluster.setRightSpeed(15);

  int count = 0;
  int max = 0;
  // Run control law.
  while (true) {
    for (auto agent: cluster){
      auto proximityinformation = agent->getProximity();

      for (auto value : proximityinformation){

        if (value.actual>0){
          agent->setSmallLedOn(count, true);
        }
        else{
          agent->setSmallLedOn(count,false);
        }
        if (value.actual>max){
          max= value.actual;
        }
        count++;
      }

      if (max > 25){
        agent->setRGB(255,0,0);
      } else if (max>0){
        agent->setRGB(0,255,0);
      }
      else{
        agent->setRGB(0,0,255);
      }

      // resetting counters
      count = 0;
      max=0;
    }


    this_thread::sleep_for(chrono::milliseconds(500));
  }

  return 0;
}
