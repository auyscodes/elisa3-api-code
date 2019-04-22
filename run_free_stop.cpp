// Copyright (c) 2019 Ramapo College. All rights reserved.
/**
 * \file run_free_stop.cpp
 * \brief Example utilizing motor control, built-in Agent modes, as well as,
 * resetting the Agent state.
 */

#include <chrono>
#include <thread>

#include "elisa3_lib.hpp"
#include "examples/utils/utils.hpp"

using namespace std;
using namespace elisa3_lib;

using Utils = examples::Utils;

void manageBots(AgentList &);

int main() {
  LOG << "Initializing cluster..." << LENDL;

  ClusterOptions cluster_opts;
  cluster_opts.agent_addresses = {3531};
  cluster_opts.comms_options.comms_mode = POLLING;
  Cluster cluster(cluster_opts);

  LOG << Utils::printClusterInfo(cluster);

  while (true) {
    // Set all bots off to roam freely.
    cluster.setObstacleAvoidanceEnabled(true);
    cluster.setCliffAvoidanceEnabled(true);
    cluster.setLeftSpeed(7);
    cluster.setRightSpeed(7);

    LOG << "Starting free run..." << LENDL;

    // Check on the bots every second for 10 seconds.
    for (int i = 0; i < 10; i++) {
      manageBots(cluster);
      this_thread::sleep_for(chrono::milliseconds(1000));
    }

    // Reset all the bots and wait for 10 seconds.
    LOG << "Pausing free run for 10s..." << LENDL;
    cluster.resetState();
    this_thread::sleep_for(chrono::seconds(10));
  }

  return 0;
}

void manageBots(AgentList &list) {
  // Print positions of each robot.
  for (auto agent : list) {
    if (agent->isSleepEnabled()) continue;
    auto odom = agent->getOdometry();
    LOG << "Agent [" << agent->getAddress() << "]: (" << odom.xCoord << ", "
        << odom.yCoord << ")" << LENDL;
  }

  // Find all bots with low battery.
  auto agents_with_low_batt = list.findAll([](const AgentRef &agent) {
    if (agent->isSleepEnabled()) {
      return false;
    }
    auto level = agent->getBatteryPercent();
    return level != 0 && level < 30;
  });

  // For each robot low on battery...
  for (auto agent : agents_with_low_batt) {
    // Notify user.
    LOGWARN << "Agent [" << agent->getAddress() << "]: "
            << "Low battery!" << LENDL;

    // Stop the bot.
    agent->resetState();
    agent->setSleepEnabled(true);
    LOGWARN << "Agent [" << agent->getAddress() << "]: "
            << "Set to sleep." << LENDL;
  }
}
