// Copyright (c) 2019 Ramapo College. All rights reserved.
/**
 * \file sensor_calibration.cpp
 * \brief Example demonstrating Agent sensor calibration.
 */

#include <iostream>

#include "elisa3_lib.hpp"
#include "examples/utils/utils.hpp"

using namespace std;
using namespace elisa3_lib;

using Utils = examples::Utils;

int main() {
  LOG << "Initializing cluster..." << endl;

  ClusterOptions cluster_opts;
  cluster_opts.agent_addresses = {3531};
  cluster_opts.comms_options.poll_rate = 10;
  cluster_opts.comms_options.comms_mode = POLLING;
  Cluster cluster(cluster_opts);

  LOG << Utils::printClusterInfo(cluster);

  cout << "Enter [p] to print state and [c] to calibrate sensors." << endl;

  while (true) {
    char input;
    cout << "> ";
    cin >> input;
    switch (input) {
      case 'p': {
        for (auto agent : cluster) {
          LOG << Utils::printAgentState(agent);
        }
        break;
      }
      case 'c': {
        for (auto agent : cluster) {
          LOG << "Agent [" << agent->getAddress() << "]: "
              << "Calibrating sensors." << endl;
          agent->calibrateSensors();
        }
        break;
      }
      default: {
        cout << "Invalid input." << endl;
        break;
      }
    }
  }
  return 0;
}
