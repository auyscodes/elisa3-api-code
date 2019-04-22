// Copyright (c) 2019 Ramapo College. All rights reserved.
/**
 * \file utils.hpp
 * \brief Implements utilities for logging and serializing information in
 * examples.
 */

#ifndef ELISA3_EXAMPLES_UTILS
#define ELISA3_EXAMPLES_UTILS

#include "elisa3_lib.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#define LOG                                                \
  std::cout << elisa3_lib::examples::Utils::logLineHeader( \
      elisa3_lib::examples::Utils::LogLevel::INFO)

#define LOGWARN                                            \
  std::cout << elisa3_lib::examples::Utils::logLineHeader( \
      elisa3_lib::examples::Utils::LogLevel::WARNING)

#define LOGERR                                             \
  std::cout << elisa3_lib::examples::Utils::logLineHeader( \
      elisa3_lib::examples::Utils::LogLevel::ERROR)

#define LENDL std::endl

namespace elisa3_lib {
namespace examples {

class Utils {
 public:
  /** \brief The log level of any given log line. */
  enum LogLevel {
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
  };

  /**
   * \brief provides the current UNIX millisecond timestamp.
   * \return The current timestamp.
   */
  static uint64_t getTimestamp();

  /**
   * \brief Generates the metadata at the beginning of a log line.
   * \return The log line metadata.
   */
  static std::string logLineHeader(LogLevel level);

  /**
   * \brief Generates a text serialization of an Agent's entire state.
   * \return The state serialization.
   */
  static std::string printAgentState(const AgentRef& agent);

  /**
   * \brief Generates a text serialization of a Cluster's state.
   * \return The state serialization.
   */
  static std::string printClusterInfo(AgentList& list);
};

}  // namespace examples
}  // namespace elisa3_lib

#endif  // ELISA3_EXAMPLES_UTILS
