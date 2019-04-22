// Copyright (c) 2019 Ramapo College. All rights reserved.
/**
 * \file battery_tester.cpp
 * \brief Example in which Agents are monitored for their battery levels.
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
  cluster_opts.agent_addresses = {3531};
  cluster_opts.comms_options.comms_mode = POLLING;
  Cluster cluster(cluster_opts);

  LOG << Utils::printClusterInfo(cluster);
  LOG << "Monitoring battery levels." << LENDL;

  while (true) {
    // Find all bots with low battery.
    auto agents_with_low_batt = cluster.findAll([](const AgentRef &agent) {
      auto level = agent->getBatteryPercent();
      return level != 0 && level < 50;
    });

    // For each robot with low battery...
    for (const auto &agent : agents_with_low_batt) {
      auto level = agent->getBatteryPercent();

      // Notify user.
      if (level < 30) {
        LOGWARN << "Agent[" << agent->getAddress() << "]: "
                << "( " << level << " ) Low battery!" << LENDL;
        agent->setRGB(100, 0, 0);
      } else {
        LOG << "Agent[" << agent->getAddress() << "]: "
            << "( " << level << " ) Battery Warning." << LENDL;
        agent->setRGB(100, 50, 0);
      }
    }

    this_thread::sleep_for(chrono::milliseconds(2000));
  }

  return 0;
}
