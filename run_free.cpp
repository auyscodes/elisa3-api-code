// Copyright (c) 2019 Ramapo College. All rights reserved.
/**
 * \file run_free.cpp
 * \brief Example utilizing motor control and built-in Agent modes.
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
  cluster_opts.agent_addresses = {3527};
  cluster_opts.comms_options.comms_mode = elisa3_lib::POLLING;
  Cluster cluster(cluster_opts);

  LOG << Utils::printClusterInfo(cluster);

  // Set all bots off to roam freely.
  cluster.setObstacleAvoidanceEnabled(true);
  cluster.setCliffAvoidanceEnabled(true);
  cluster.setLeftSpeed(7);
  cluster.setRightSpeed(7);

  LOG << "Starting free run..." << LENDL;

  // Check on the bots every second.
  while (true) {
    manageBots(cluster);
    this_thread::sleep_for(chrono::milliseconds(1000));
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
