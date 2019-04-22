// Copyright (c) 2019 Ramapo College. All rights reserved.
/**
 * \file lmt_print.cpp
 * \brief Example demonstrating the use of LMTs.
 */

#include <sstream>

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
  cluster_opts.comms_options.poll_rate = 3;
  Cluster cluster(cluster_opts);

  LOG << Utils::printClusterInfo(cluster);

  AgentRef agent =
      cluster.findOne([](AgentRef ag) { return ag->getAddress() == 3531; });

  while (true) {
    LOG << "Agent [" << agent->getAddress() << "] "
        << "Awaiting Full State Update..." << LENDL;
    agent->awaitFullStateUpdate();

    LOG << "Agent [" << agent->getAddress() << "] "
        << "Full State Update at " << agent->getLastModifiedTime() << LENDL;

    auto lmts = agent->getPacketsLastModifiedTimes();

    stringstream temp;
    for (int i = 0; i < lmts.size(); i++) {
      temp << i + 3 << ": " << lmts[i] << LENDL;
    }
    LOG << "Agent [" << agent->getAddress() << "] Packet LMTs:" << LENDL
        << temp.str() << LENDL;
  }

  return 0;
}
