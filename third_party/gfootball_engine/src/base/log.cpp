// Copyright 2019 Google LLC & Bastiaan Konings
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// written by bastiaan konings schuiling 2008 - 2014
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#include <chrono>
#include "log.hpp"
#include "../main.hpp"

#include <iostream>
#include <fstream>
#include <string>

std::string now() {
  auto now = std::chrono::system_clock::now();
  time_t tt;
  tt = std::chrono::system_clock::to_time_t(now);
  return ctime(&tt);
}

namespace blunted {

  signal_LogCallback callback;

  std::ofstream logFile;

  void LogOpen() {
    logFile.open("log.txt", std::ios::out);
  }

  void LogClose() {
    if (logFile.is_open()) logFile.close();
  }

  void Log(e_LogType logType, std::string className, std::string methodName, std::string message) {
    std::string logTypeString;

    switch (logType) {
      case e_Notice:
        logTypeString = "Notice";
        if (!GetGameConfig().log_notice) {
          return;
        }
        break;
      case e_Warning:
        logTypeString = "Warning";
        break;
      case e_Error:
        logTypeString = "ERROR";
        break;
      case e_FatalError:
        logTypeString = "FATAL ERROR !!! N00000 !!!";
        break;
    }

    char bla[2048];
    std::string date = now();
    date = date.substr(0, date.length() - 1);
    sprintf(bla, "%s [%s] in [%s::%s]: %s\n", date.c_str(),
            logTypeString.c_str(), className.c_str(), methodName.c_str(),
            message.c_str());
    callback(logType, className.c_str(), methodName.c_str(), message.c_str());

    printf("%s", bla);
    if (logFile.is_open()) logFile << bla;
    logFile.flush();

    if (logType == e_FatalError) {
      LogClose();
      // for gdb backtracing
      int *foo = (int*)-1; // make a bad pointer
      printf("%d\n", *foo); // causes segfault
      print_stacktrace();
      exit(1);
    }
  }

}
