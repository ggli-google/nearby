// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef THIRD_PARTY_NEARBY_FASTPAIR_INTERNAL_PUBLIC_TASK_RUNNER_H_
#define THIRD_PARTY_NEARBY_FASTPAIR_INTERNAL_PUBLIC_TASK_RUNNER_H_

#include <functional>

#include "absl/time/time.h"

namespace location {
namespace nearby {
namespace fastpair {

// Task runner is an implementation to run tasks immediately or with a delay.
// The current implementation does not allow running nested tasks.
class TaskRunner {
 public:
  virtual ~TaskRunner() = default;

  // Posts a task to task runner. The task runs immediately or not depends on
  // the implementation of class. If the implementation supports multiple
  // threads, posted tasks could run concurrently.
  virtual bool PostTask(std::function<void()> task) = 0;

  // Posts a task to run with delay. Multiple tasks can be scheduled. Tasks will
  // execute in the order of their delay expiring, not in the order they were
  // posted.
  virtual bool PostDelayedTask(absl::Duration delay,
                               std::function<void()> task) = 0;
};

}  // namespace fastpair
}  // namespace nearby
}  // namespace location

#endif  // THIRD_PARTY_NEARBY_FASTPAIR_INTERNAL_PUBLIC_TASK_RUNNER_H_