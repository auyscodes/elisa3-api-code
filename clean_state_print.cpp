// Copyright (c) 2019 Ramapo College. All rights reserved.
/**
 * \file clean_state_print.cpp
 * \brief Example demonstrating awaiting for a clean state.
 */

#include "elisa3_lib.hpp"
#include "examples/utils/utils.hpp"

using namespace std;
using namespace elisa3_lib;

using Utils = examples::Utils;

int main() {
  LOG << "Initializing cluster..." << LENDL;

  ClusterOptions cluster_opts;
  cluster_opts.agent_addresses = {3572};
  cluster_opts.comms_options.comms_mode = POLLING;
  cluster_opts.comms_options.poll_rate = 1;
  Cluster cluster(cluster_opts);

  LOG << Utils::printClusterInfo(cluster);

  AgentRef agent =
      cluster.findOne([](AgentRef ag) { return ag->getAddress() == 3572; });

  int led_id = 0;

  while (true) {
    cluster.setSmallLedsOn(false);
    cluster.setSmallLedOn(led_id, true);
    led_id = (led_id + 1) % 8;

    agent->awaitCleanState();
    LOG << "Agent [" << agent->getAddress() << "] State Pushed. "
        << "Partial LMT: " << agent->getLastPartialModifiedTime() << LENDL;
  }

  return 0;
}
