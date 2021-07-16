// Copyright 2020 Google LLC
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

#include "platform/impl/windows/bluetooth_classic_server_socket.h"

namespace location {
namespace nearby {
namespace windows {
BluetoothServerSocket::~BluetoothServerSocket() {}

// https://developer.android.com/reference/android/bluetooth/BluetoothServerSocket.html#accept()
//
// Blocks until either:
// - at least one incoming connection request is available, or
// - ServerSocket is closed.
// On success, returns connected socket, ready to exchange data.
// Returns nullptr on error.
// Once error is reported, it is permanent, and ServerSocket has to be closed.
// TODO(b/184975123): replace with real implementation.
std::unique_ptr<api::BluetoothSocket> BluetoothServerSocket::Accept() {
  return nullptr;
}

// https://developer.android.com/reference/android/bluetooth/BluetoothServerSocket.html#close()
//
// Returns Exception::kIo on error, Exception::kSuccess otherwise.
// TODO(b/184975123): replace with real implementation.
Exception BluetoothServerSocket::Close() { return Exception(); }

}  // namespace windows
}  // namespace nearby
}  // namespace location
