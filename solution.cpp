// Copyright (c) 2019 Ramapo College. All rights reserved.

/**
 * \file solution.cpp
 * \brief The main file of the project.
 */
#include <chrono>
#include <thread>
#include <iostream>
#include <sstream>
using namespace std;

// Include the Elisa3 Library.
#include <elisa3_lib.hpp>
using namespace elisa3_lib;

// #include "util.hpp"

// using namespace sample_project;

int main() {
  // Get the Agent address from the user.
  short unsigned int agent_address;
  do {
    cout << "Enter the address of the Agent you wish to use: ";
    cin >> agent_address;
  } while (agent_address < 1000 || agent_address > 9999);

  // Setup cluster.
  ClusterOptions cluster_opts;
  cluster_opts.agent_addresses = {agent_address};
  cluster_opts.comms_options.comms_mode = POLLING;
  cluster_opts.comms_options.poll_rate = 3;

  cout << "Initializing cluster..." << endl;
  Cluster cluster(cluster_opts);
  AgentRef agent = cluster.findOne(
      [=](AgentRef ag) { return ag->getAddress() == agent_address; });
  agent->calibrateSensors();
  int count = 0;
  int max = 0;
  // Run control law.
  while (true) {

    // cout << "Agent [" << agent->getAddress() << "] "
    //      << "Awaiting Full State Update..." << endl;
    // agent->awaitFullStateUpdate();
    //
    // cout << "Agent [" << agent->getAddress() << "] "
    //      << "Full State Update at " << agent->getLastModifiedTime() << endl;
    auto proximityinformation = agent->getProximity();

    for (auto value : proximityinformation){
      cout<< "Index : " << count << "  " <<value.actual << endl;
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

    if (max > 500){
      
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
    // cout << "Agent [" << agent->getAddress() << "] Packet LMTs:" << endl
    //      << Util::printLMTs(agent->getPacketsLastModifiedTimes()) << endl;
    this_thread::sleep_for(chrono::milliseconds(2000));
  }

  return 0;
}
