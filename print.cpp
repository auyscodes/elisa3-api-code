// Copyright (c) 2019 Ramapo College. All rights reserved.
/**
 * \file print.cpp
 * \brief Example demonstrating setting up a cluster and printing the Agents'
 * state.
 */

#include <chrono>
#include <thread>

#include "elisa3_lib.hpp"
#include "examples/utils/utils.hpp"

using namespace std;
using namespace elisa3_lib;

using Utils = examples::Utils;

int main() {
  LOG << "Initializing cluster..." << LENDL;

  ClusterOptions cluster_opts;
  cluster_opts.agent_addresses = {3527};
  cluster_opts.comms_options.poll_rate = 10;
  cluster_opts.comms_options.comms_mode = POLLING;
  Cluster cluster(cluster_opts);

  LOG << Utils::printClusterInfo(cluster);

  bool rgb_on = false;
  bool alt = false;
  int led_id = 0;

  while (true) {
    if (rgb_on) {
      if (alt) {
        cluster.setRGB(100, 0, 0);
      } else {
        cluster.setRGB(0, 0, 100);
      }
    } else {
      cluster.setRGB(0, 0, 0);
    }

    cluster.setSmallLedsOn(false);
    cluster.setSmallLedOn(led_id, true);

    for (int i = 0; i < 10; i++) {
      for (auto agent : cluster) {
        LOG << Utils::printAgentState(agent);
        this_thread::sleep_for(chrono::milliseconds(100));
      }
    }

    led_id = (led_id + 1) % 8;
    alt = !alt;
  }
  return 0;
}
