#include "utils.hpp"

uint64_t elisa3_lib::examples::Utils::getTimestamp() {
  auto current_time = std::chrono::system_clock::now();
  auto duration =
      std::chrono::time_point_cast<std::chrono::milliseconds>(current_time);
  return duration.time_since_epoch().count();
}

std::string elisa3_lib::examples::Utils::logLineHeader(
    elisa3_lib::examples::Utils::LogLevel level) {
  std::stringstream out;
  // Set the log line metadata color.
  switch (level) {
    case ERROR: {
      // Print in red.
      out << "\033[31m";
      break;
    }
    case WARNING: {
      // Print in yellow.
      out << "\033[33m";
      break;
    }
    case INFO:  // Fall-through intended.
    default: {
      // Print in green.
      out << "\033[32m";
      break;
    }
  }

  // Print current timestamp. Separate milliseconds from timestamp.
  auto ts = getTimestamp();
  out << (ts / 1000) << "." << std::setfill('0') << std::setw(3) << std::right
      << (ts % 1000);

  // Print log level.
  switch (level) {
    case ERROR: {
      out << " ERROR: ";
      break;
    }
    case WARNING: {
      // Print in yellow.
      out << " WARNING: ";
      break;
    }
    case INFO:  // Fall-through intended.
    default: {
      out << " INFO: ";
      break;
    }
  }

  out << "\033[0m";
  return out.str();
}

std::string elisa3_lib::examples::Utils::printAgentState(
    const elisa3_lib::AgentRef& agent) {
  std::stringstream out;
  out << "Agent State:" << std::endl;
  int selector = agent->getSelector();
  int remote = agent->getRemoteControlCommand();
  out << "Agent [" << agent->getAddress() << "]" << std::endl;
  out << "Battery: " << agent->getBatteryPercent() << "% ("
      << agent->getBatteryAdc() << ")" << std::endl;
  out << "Packet Error Rate: " << agent->getPacketErrorRate() << "%"
      << std::endl;
  out << "LMT: " << agent->getLastModifiedTime() << std::endl;
  std::ios::fmtflags flags(out.flags());
  out << std::hex << std::showbase;
  out << "Selector: " << selector << std::endl;
  out << "Remote: " << remote << std::endl;
  out.flags(flags);
  out << "Flags: " << (agent->isButtonPressed() ? " BUTTON" : "")
      << (agent->isCharged() ? " CHARGED" : "")
      << (agent->isCharging() ? " CHARGING" : "")
      << (agent->isSleepEnabled() ? " SLEEPING" : "")
      << (agent->isObstacleAvoidanceEnabled() ? " OBST_AVOID_ON" : "")
      << (agent->isCliffAvoidanceEnabled() ? " CLIFF_AVOID_ON" : "")
      << (agent->isRemoteControlEnabled() ? " REMOTE_ON" : "")
      << (agent->isStateClean() ? "" : " STATE_DIRTY") << std::endl;
  out << std::endl;

  auto accel = agent->getAccelerometer();
  out << "Accelerometer: " << std::endl;
  out << "\tX: " << accel.xValue << std::endl;
  out << "\tY: " << accel.yValue << std::endl;
  out << "\tZ: " << accel.zValue << std::endl;
  out << std::endl;

  auto pose = agent->getOdometry();
  out << "Odometry: " << std::endl;
  out << "\tX: " << pose.xCoord << std::endl;
  out << "\tY: " << pose.yCoord << std::endl;
  out << "\tT: " << pose.theta << std::endl;
  out << "\tSteps [L]: " << agent->getLeftMotSteps() << std::endl;
  out << "\tSteps [R]: " << agent->getRightMotSteps() << std::endl;
  out << std::endl;

  auto prox = agent->getProximity();
  out << "Proximity: " << std::endl;
  for (auto& measure : prox) {
    out << "\tActual: " << std::setw(5) << std::left << measure.actual
        << " Ambient: " << std::setw(5) << std::left << measure.ambient
        << std::endl;
  }
  out << std::endl;

  auto ground = agent->getGround();
  out << "Ground: " << std::endl;
  for (auto& measure : ground) {
    out << "\tActual: " << std::setw(5) << std::left << measure.actual
        << " Ambient: " << std::setw(5) << std::left << measure.ambient
        << std::endl;
  }
  out << std::endl;

  return out.str();
}

std::string elisa3_lib::examples::Utils::printClusterInfo(
    elisa3_lib::AgentList& list) {
  std::stringstream out;
  out << "Cluster Info:" << std::endl;
  out << "Agents Count: " << list.size() << std::endl;
  out << "Agents by Address: ";
  for (const auto& agent : list) {
    out << " " << agent->getAddress();
  }
  out << std::endl;
  return out.str();
}
