// Copyright (c) 2019 Ramapo College. All rights reserved.
/**
 * \file periodic_checkup.cpp
 * \brief Example demonstrating utilizing the Agent's sleep mode and await
 * methods.
 */

#include "elisa3_lib.hpp"
#include "examples/utils/utils.hpp"

using namespace std;
using namespace elisa3_lib;

using Utils = examples::Utils;

int main() {
  LOG << "Initializing cluster..." << LENDL;

  ClusterOptions cluster_opts;
  cluster_opts.agent_addresses = {3558};
  cluster_opts.comms_options.poll_rate = 10;
  cluster_opts.comms_options.comms_mode = POLLING;
  Cluster cluster(cluster_opts);

  LOG << Utils::printClusterInfo(cluster);

  AgentRef agent = *(cluster.begin());
  agent->setRGB(255, 0, 0);

  while (true) {
    while (!agent->awaitFullStateUpdate(5000)) {
      LOGWARN << "Awaiting full update for 5 more seconds..." << LENDL;
    }
    LOG << Utils::printAgentState(agent);

    LOG << "Setting agent to sleep." << LENDL;
    agent->setSleepEnabled(true);
    while (!agent->awaitCleanState(5000)) {
      LOGWARN << "Awaiting response for 5 more seconds..." << LENDL;
    }

    auto sleep_time_s = 30;
    cluster.pauseComms();
    LOG << "Pausing comms for " << sleep_time_s << " seconds." << LENDL;
    this_thread::sleep_for(chrono::seconds(sleep_time_s));
    cluster.unpauseComms();

    LOG << "Setting agent to running." << LENDL;
    agent->setSleepEnabled(false);
    while (!agent->awaitCleanState(5000)) {
      LOGWARN << "Awaiting response for 5 more seconds..." << LENDL;
    }
  }

  return 0;
}
