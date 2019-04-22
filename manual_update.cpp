// Copyright (c) 2019 Ramapo College. All rights reserved.
/**
 * \file manual_update.cpp
 * \brief Example demonstrating utilizing the Manual Comms mode.
 */

#include <iostream>

#include "elisa3_lib.hpp"
#include "examples/utils/utils.hpp"

using namespace std;
using namespace elisa3_lib;

using Utils = examples::Utils;

int main() {
  LOG << "Initializing cluster..." << LENDL;

  ClusterOptions cluster_opts;
  cluster_opts.agent_addresses = {3531};
  cluster_opts.comms_options.comms_mode = MANUAL;
  Cluster cluster(cluster_opts);

  LOG << Utils::printClusterInfo(cluster);

  while (true) {
    int lspeed;
    do {
      cout << "Enter new left speed: ";
      cin >> lspeed;
    } while (lspeed < -127 || lspeed > 127);

    int rspeed;
    do {
      cout << "Enter new right speed: ";
      cin >> rspeed;
    } while (rspeed < -127 || rspeed > 127);

    cluster.setLeftSpeed(lspeed);
    cluster.setRightSpeed(rspeed);

    LOG << "Updating agents..." << LENDL << LENDL;
    for (auto agent : cluster) {
      cluster.manuallyUpdateAgent(agent);
    }
  }
  return 0;
}
