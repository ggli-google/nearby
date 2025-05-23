// Copyright 2022-2023 Google LLC
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

#ifndef ANALYTICS_ANALYTICS_RECORDER_H_
#define ANALYTICS_ANALYTICS_RECORDER_H_

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/base/thread_annotations.h"
#include "absl/container/btree_map.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"
#include "connections/implementation/analytics/advertising_metadata_params.h"
#include "connections/implementation/analytics/connection_attempt_metadata_params.h"
#include "connections/implementation/analytics/discovery_metadata_params.h"
#include "connections/payload_type.h"
#include "connections/strategy.h"
#include "internal/analytics/event_logger.h"
#include "internal/platform/error_code_params.h"
#include "internal/platform/implementation/system_clock.h"
#include "internal/platform/mutex.h"
#include "internal/proto/analytics/connections_log.pb.h"
#include "proto/connections_enums.pb.h"

namespace nearby {
namespace analytics {

class AnalyticsRecorder {
 public:
  explicit AnalyticsRecorder(::nearby::analytics::EventLogger *event_logger);
  // For testing only.
  AnalyticsRecorder(::nearby::analytics::EventLogger *event_logger,
                    bool no_record_time_millis);
  virtual ~AnalyticsRecorder();

  // Advertising phase
  void OnStartAdvertising(
      connections::Strategy strategy,
      const std::vector<location::nearby::proto::connections::Medium> &mediums,
      AdvertisingMetadataParams *advertising_metadata_params)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnStopAdvertising() ABSL_LOCKS_EXCLUDED(mutex_);

  // In case the client calls the {@link BasePcp#updateAdvertisingOptions()}
  // multiple times, adds one index value to group the mediums results within
  // the same UpdateAdvertisingOptions call, this API is to return the largest
  // index value in current_advertising_phase.
  int GetNextAdvertisingUpdateIndex() ABSL_LOCKS_EXCLUDED(mutex_);

  // Connection listening
  void OnStartedIncomingConnectionListening(connections::Strategy strategy)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnStoppedIncomingConnectionListening() ABSL_LOCKS_EXCLUDED(mutex_);

  // Discovery phase
  void OnStartDiscovery(
      connections::Strategy strategy,
      const std::vector<location::nearby::proto::connections::Medium> &mediums,
      DiscoveryMetadataParams *discovery_metadata_params)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnStopDiscovery() ABSL_LOCKS_EXCLUDED(mutex_);

  // In case the client calls the {@link BasePcp#updateDiscoveryOptions()}
  // multiple times, adds one index value to group the medium results within the
  // same UpdateDiscoveryOptions call, this
  // API is to return the latest index value in current_discovery_phase.
  int GetNextDiscoveryUpdateIndex() ABSL_LOCKS_EXCLUDED(mutex_);
  void OnEndpointFound(location::nearby::proto::connections::Medium medium)
      ABSL_LOCKS_EXCLUDED(mutex_);

  // Connection request
  void OnRequestConnection(const connections::Strategy &strategy,
                           const std::string &endpoint_id)
      ABSL_LOCKS_EXCLUDED(mutex_);

  void OnConnectionRequestReceived(const std::string &remote_endpoint_id)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnConnectionRequestSent(const std::string &remote_endpoint_id)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnRemoteEndpointAccepted(const std::string &remote_endpoint_id)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnLocalEndpointAccepted(const std::string &remote_endpoint_id)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnRemoteEndpointRejected(const std::string &remote_endpoint_id)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnLocalEndpointRejected(const std::string &remote_endpoint_id)
      ABSL_LOCKS_EXCLUDED(mutex_);

  // Connection attempt
  // Records an attempt with meta data at establishing an incoming physical
  // connection.
  void OnIncomingConnectionAttempt(
      location::nearby::proto::connections::ConnectionAttemptType type,
      location::nearby::proto::connections::Medium medium,
      location::nearby::proto::connections::ConnectionAttemptResult result,
      absl::Duration duration, const std::string &connection_token,
      ConnectionAttemptMetadataParams *connection_attempt_metadata_params)
      ABSL_LOCKS_EXCLUDED(mutex_);
  // Records an attempt with meta data at establishing an outgoing physical
  // connection.
  void OnOutgoingConnectionAttempt(
      const std::string &remote_endpoint_id,
      location::nearby::proto::connections::ConnectionAttemptType type,
      location::nearby::proto::connections::Medium medium,
      location::nearby::proto::connections::ConnectionAttemptResult result,
      absl::Duration duration, const std::string &connection_token,
      ConnectionAttemptMetadataParams *connection_attempt_metadata_params)
      ABSL_LOCKS_EXCLUDED(mutex_);
  static std::unique_ptr<AdvertisingMetadataParams>
  BuildAdvertisingMetadataParams(
      bool is_extended_advertisement_supported = false,
      int connected_ap_frequency = 0, bool is_nfc_available = false,
      const std::vector<location::nearby::analytics::proto::ConnectionsLog::
                            OperationResultWithMedium>
          &operation_result_with_mediums = {});
  static std::unique_ptr<DiscoveryMetadataParams> BuildDiscoveryMetadataParams(
      bool is_extended_advertisement_supported = false,
      int connected_ap_frequency = 0, bool is_nfc_available = false,
      const std::vector<location::nearby::analytics::proto::ConnectionsLog::
                            OperationResultWithMedium>
          &operation_result_with_mediums = {});

  static std::unique_ptr<ConnectionAttemptMetadataParams>
  BuildConnectionAttemptMetadataParams(
      location::nearby::proto::connections::ConnectionTechnology technology,
      location::nearby::proto::connections::ConnectionBand band, int frequency,
      int try_count, const std::string &network_operator = {},
      const std::string &country_code = {}, bool is_tdls_used = false,
      bool wifi_hotspot_enabled = false, int max_wifi_tx_speed = 0,
      int max_wifi_rx_speed = 0, int channel_width = -1,
      location::nearby::proto::connections::OperationResultCode
          operation_result_code = location::nearby::proto::connections::
              OperationResultCode::DETAIL_UNKNOWN);
  static location::nearby::proto::connections::OperationResultCode
  GetChannelIoErrorResultCodeFromMedium(
      location::nearby::proto::connections::Medium medium);

  // Connection establishedSafeDisconnectionResult
  void OnConnectionEstablished(
      const std::string &endpoint_id,
      location::nearby::proto::connections::Medium medium,
      const std::string &connection_token) ABSL_LOCKS_EXCLUDED(mutex_);
  void OnConnectionClosed(
      const std::string &endpoint_id,
      location::nearby::proto::connections::Medium medium,
      location::nearby::proto::connections::DisconnectionReason reason,
      location::nearby::analytics::proto::ConnectionsLog::
          EstablishedConnection::SafeDisconnectionResult result)
      ABSL_LOCKS_EXCLUDED(mutex_);

  // Payload
  void OnIncomingPayloadStarted(const std::string &endpoint_id,
                                std::int64_t payload_id,
                                connections::PayloadType type,
                                std::int64_t total_size_bytes)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnPayloadChunkReceived(const std::string &endpoint_id,
                              std::int64_t payload_id,
                              std::int64_t chunk_size_bytes)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnIncomingPayloadDone(
      const std::string &endpoint_id, std::int64_t payload_id,
      location::nearby::proto::connections::PayloadStatus status,
      location::nearby::proto::connections::OperationResultCode
          operation_result_code) ABSL_LOCKS_EXCLUDED(mutex_);
  void OnOutgoingPayloadStarted(const std::vector<std::string> &endpoint_ids,
                                std::int64_t payload_id,
                                connections::PayloadType type,
                                std::int64_t total_size_bytes)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnPayloadChunkSent(const std::string &endpoint_id,
                          std::int64_t payload_id,
                          std::int64_t chunk_size_bytes)
      ABSL_LOCKS_EXCLUDED(mutex_);
  void OnOutgoingPayloadDone(
      const std::string &endpoint_id, std::int64_t payload_id,
      location::nearby::proto::connections::PayloadStatus status,
      location::nearby::proto::connections::OperationResultCode
          operation_result_code) ABSL_LOCKS_EXCLUDED(mutex_);

  // BandwidthUpgrade
  void OnBandwidthUpgradeStarted(
      const std::string &endpoint_id,
      location::nearby::proto::connections::Medium from_medium,
      location::nearby::proto::connections::Medium to_medium,
      location::nearby::proto::connections::ConnectionAttemptDirection
          direction,
      const std::string &connection_token) ABSL_LOCKS_EXCLUDED(mutex_);
  void OnBandwidthUpgradeError(
      const std::string &endpoint_id,
      location::nearby::proto::connections::BandwidthUpgradeResult result,
      location::nearby::proto::connections::BandwidthUpgradeErrorStage
          error_stage,
      location::nearby::proto::connections::OperationResultCode
          operation_result_code) ABSL_LOCKS_EXCLUDED(mutex_);
  void OnBandwidthUpgradeSuccess(const std::string &endpoint_id)
      ABSL_LOCKS_EXCLUDED(mutex_);

  // Error Code
  void OnErrorCode(const ErrorCodeParams &params);

  // Log the start client session event with start client session logging
  // resources setup (e.g. client_session_, started_client_session_time_)
  void LogStartSession() ABSL_LOCKS_EXCLUDED(mutex_);

  // Invokes event_logger_.Log() at the end of life of client. Log action is
  // called in a separate thread to allow synchronous potentially lengthy
  // execution.
  void LogSession() ABSL_LOCKS_EXCLUDED(mutex_);

  bool IsSessionLogged();

  location::nearby::proto::connections::OperationResultCategory
  GetOperationResultCategory(
      location::nearby::proto::connections::OperationResultCode result_code);

  // Waits until all logs are sent to the backend.
  // For testing only.
  void Sync();

 private:
  // Tracks the chunks and duration of a Payload on a particular medium.
  class PendingPayload {
   public:
    PendingPayload(location::nearby::proto::connections::PayloadType type,
                   std::int64_t total_size_bytes, bool no_record_time_millis)
        : PendingPayload(type, total_size_bytes, no_record_time_millis,
                         location::nearby::proto::connections::
                             OperationResultCode::DETAIL_UNKNOWN) {}
    PendingPayload(location::nearby::proto::connections::PayloadType type,
                   std::int64_t total_size_bytes, bool no_record_time_millis,
                   location::nearby::proto::connections::OperationResultCode
                       operation_result_code)
        : start_time_(SystemClock::ElapsedRealtime()),
          type_(type),
          total_size_bytes_(total_size_bytes),
          num_bytes_transferred_(0),
          num_chunks_(0),
          operation_result_code_(operation_result_code),
          no_record_time_millis_(no_record_time_millis) {}
    ~PendingPayload() = default;

    void AddChunk(std::int64_t chunk_size_bytes);

    location::nearby::analytics::proto::ConnectionsLog::Payload GetProtoPayload(
        location::nearby::proto::connections::PayloadStatus status);

    location::nearby::proto::connections::PayloadType type() const {
      return type_;
    }

    std::int64_t total_size_bytes() const { return total_size_bytes_; }

    void SetOperationResultCode(
        location::nearby::proto::connections::OperationResultCode
            operation_result_code) {
      operation_result_code_ = operation_result_code;
    }

   private:
    absl::Time start_time_;
    location::nearby::proto::connections::PayloadType type_;
    std::int64_t total_size_bytes_;
    std::int64_t num_bytes_transferred_;
    int num_chunks_;
    location::nearby::proto::connections::OperationResultCode
        operation_result_code_ = location::nearby::proto::connections::
            OperationResultCode::DETAIL_UNKNOWN;
    // For testing only.
    bool no_record_time_millis_ = false;
  };

  class LogicalConnection {
   public:
    LogicalConnection(
        location::nearby::proto::connections::Medium initial_medium,
        const std::string &connection_token, bool no_record_time_millis)
        : no_record_time_millis_(no_record_time_millis) {
      PhysicalConnectionEstablished(initial_medium, connection_token);
    }
    LogicalConnection(const LogicalConnection &) = delete;
    LogicalConnection(LogicalConnection &&other)
        : current_medium_(std::move(other.current_medium_)),
          physical_connections_(std::move(other.physical_connections_)),
          incoming_payloads_(std::move(other.incoming_payloads_)),
          outgoing_payloads_(std::move(other.outgoing_payloads_)) {}
    LogicalConnection &operator=(const LogicalConnection &) = delete;
    LogicalConnection &&operator=(LogicalConnection &&) = delete;
    ~LogicalConnection() = default;

    void PhysicalConnectionEstablished(
        location::nearby::proto::connections::Medium medium,
        const std::string &connection_token);
    void PhysicalConnectionClosed(
        location::nearby::proto::connections::Medium medium,
        location::nearby::proto::connections::DisconnectionReason reason,
        location::nearby::analytics::proto::ConnectionsLog::
            EstablishedConnection::SafeDisconnectionResult result);
    void CloseAllPhysicalConnections();

    void IncomingPayloadStarted(
        std::int64_t payload_id,
        location::nearby::proto::connections::PayloadType type,
        std::int64_t total_size_bytes);
    void ChunkReceived(std::int64_t payload_id, std::int64_t size_bytes);
    void IncomingPayloadDone(
        std::int64_t payload_id,
        location::nearby::proto::connections::PayloadStatus status,
        location::nearby::proto::connections::OperationResultCode
            operation_result_code);
    void OutgoingPayloadStarted(
        std::int64_t payload_id,
        location::nearby::proto::connections::PayloadType type,
        std::int64_t total_size_bytes);
    void ChunkSent(std::int64_t payload_id, std::int64_t size_bytes);
    void OutgoingPayloadDone(
        std::int64_t payload_id,
        location::nearby::proto::connections::PayloadStatus status,
        location::nearby::proto::connections::OperationResultCode
            operation_result_code);

    std::vector<location::nearby::analytics::proto::ConnectionsLog::
                    EstablishedConnection>
    GetEstablisedConnections();

   private:
    void FinishPhysicalConnection(
        location::nearby::analytics::proto::ConnectionsLog::
            EstablishedConnection *established_connection,
        location::nearby::proto::connections::DisconnectionReason reason,
        location::nearby::analytics::proto::ConnectionsLog::
            EstablishedConnection::SafeDisconnectionResult result);
    std::vector<location::nearby::analytics::proto::ConnectionsLog::Payload>
    ResolvePendingPayloads(
        absl::btree_map<std::int64_t, std::unique_ptr<PendingPayload>>
            &pending_payloads,
        location::nearby::proto::connections::DisconnectionReason reason);
    location::nearby::proto::connections::OperationResultCode
    GetPendingPayloadResultCodeFromReason(
        location::nearby::proto::connections::DisconnectionReason reason);

    location::nearby::proto::connections::Medium current_medium_ =
        location::nearby::proto::connections::UNKNOWN_MEDIUM;
    absl::btree_map<location::nearby::proto::connections::Medium,
                    std::unique_ptr<location::nearby::analytics::proto::
                                        ConnectionsLog::EstablishedConnection>>
        physical_connections_;
    absl::btree_map<std::int64_t, std::unique_ptr<PendingPayload>>
        incoming_payloads_;
    absl::btree_map<std::int64_t, std::unique_ptr<PendingPayload>>
        outgoing_payloads_;
    // For testing only.
    bool no_record_time_millis_ = false;
  };

  bool CanRecordAnalyticsLocked(absl::string_view method_name)
      ABSL_SHARED_LOCKS_REQUIRED(mutex_);

  // Callbacks the ConnectionsLog proto byte array data to the EventLogger with
  // ClientSession sub-proto.
  void LogClientSessionLocked() ABSL_EXCLUSIVE_LOCKS_REQUIRED(mutex_);
  // Callbacks the ConnectionsLog proto byte array data to the EventLogger.
  void LogEvent(location::nearby::proto::connections::EventType event_type);

  void UpdateStrategySessionLocked(
      connections::Strategy strategy,
      location::nearby::proto::connections::SessionRole role)
      ABSL_EXCLUSIVE_LOCKS_REQUIRED(mutex_);
  void RecordAdvertisingPhaseDurationAndReasonLocked(bool on_stop) const
      ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  void FinishAdvertisingPhaseLocked() ABSL_EXCLUSIVE_LOCKS_REQUIRED(mutex_);
  void RecordDiscoveryPhaseDurationAndReasonLocked(bool on_stop) const
      ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  void FinishDiscoveryPhaseLocked() ABSL_EXCLUSIVE_LOCKS_REQUIRED(mutex_);
  bool UpdateAdvertiserConnectionRequestLocked(
      location::nearby::analytics::proto::ConnectionsLog::ConnectionRequest
          *request) ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  bool UpdateDiscovererConnectionRequestLocked(
      location::nearby::analytics::proto::ConnectionsLog::ConnectionRequest
          *request) ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  bool BothEndpointsRespondedLocked(
      location::nearby::analytics::proto::ConnectionsLog::ConnectionRequest
          *request) ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  void LocalEndpointRespondedLocked(
      const std::string &remote_endpoint_id,
      location::nearby::proto::connections::ConnectionRequestResponse response)
      ABSL_EXCLUSIVE_LOCKS_REQUIRED(mutex_);
  void RemoteEndpointRespondedLocked(
      const std::string &remote_endpoint_id,
      location::nearby::proto::connections::ConnectionRequestResponse response)
      ABSL_EXCLUSIVE_LOCKS_REQUIRED(mutex_);
  void MarkConnectionRequestIgnoredLocked(
      location::nearby::analytics::proto::ConnectionsLog::ConnectionRequest
          *request) ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  void OnIncomingConnectionAttemptLocked(
      location::nearby::proto::connections::ConnectionAttemptType type,
      location::nearby::proto::connections::Medium medium,
      location::nearby::proto::connections::ConnectionAttemptResult result,
      absl::Duration duration, const std::string &connection_token,
      ConnectionAttemptMetadataParams *connection_attempt_metadata_params)
      ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  void OnOutgoingConnectionAttemptLocked(
      const std::string &remote_endpoint_id,
      location::nearby::proto::connections::ConnectionAttemptType type,
      location::nearby::proto::connections::Medium medium,
      location::nearby::proto::connections::ConnectionAttemptResult result,
      absl::Duration duration, const std::string &connection_token,
      ConnectionAttemptMetadataParams *connection_attempt_metadata_params)
      ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  bool ConnectionAttemptResultCodeExistedLocked(
      location::nearby::proto::connections::Medium medium,
      location::nearby::proto::connections::ConnectionAttemptDirection
          direction,
      const std::string &connection_token,
      location::nearby::proto::connections::ConnectionAttemptType type,
      location::nearby::proto::connections::OperationResultCode
          operation_result_code) ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  bool EraseIfBandwidthUpgradeRecordExistedLocked(
      const std::string &endpoint_id,
      location::nearby::proto::connections::BandwidthUpgradeResult result,
      location::nearby::proto::connections::BandwidthUpgradeErrorStage
          error_stage,
      location::nearby::proto::connections::OperationResultCode
          operation_result_code) ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  void FinishUpgradeAttemptLocked(
      const std::string &endpoint_id,
      location::nearby::proto::connections::BandwidthUpgradeResult result,
      location::nearby::proto::connections::BandwidthUpgradeErrorStage
          error_stage,
      location::nearby::proto::connections::OperationResultCode
          operation_result_code,
      bool erase_item = true) ABSL_SHARED_LOCKS_REQUIRED(mutex_);
  void FinishStrategySessionLocked() ABSL_EXCLUSIVE_LOCKS_REQUIRED(mutex_);

  int GetLatestUpdateIndexLocked(
      const std::vector<location::nearby::analytics::proto::ConnectionsLog::
                            OperationResultWithMedium> &list)
      ABSL_EXCLUSIVE_LOCKS_REQUIRED(mutex_);

  location::nearby::proto::connections::ConnectionsStrategy
  StrategyToConnectionStrategy(connections::Strategy strategy);
  location::nearby::proto::connections::PayloadType
  PayloadTypeToProtoPayloadType(connections::PayloadType type);

  // Not owned by AnalyticsRecorder. Pointer must refer to a valid object
  // that outlives the one constructed.
  ::nearby::analytics::EventLogger *event_logger_;

  // Protects all sub-protos reading and writing in ConnectionLog.
  Mutex mutex_;

  // For testing only.
  bool no_record_time_millis_ = false;

  // ClientSession
  std::unique_ptr<
      location::nearby::analytics::proto::ConnectionsLog::ClientSession>
      client_session_;
  absl::Time started_client_session_time_;
  bool session_was_logged_ ABSL_GUARDED_BY(mutex_) = false;
  bool start_client_session_was_logged_ ABSL_GUARDED_BY(mutex_) = false;

  // Current StrategySession
  connections::Strategy current_strategy_ ABSL_GUARDED_BY(mutex_) =
      connections::Strategy::kNone;
  std::unique_ptr<
      location::nearby::analytics::proto::ConnectionsLog::StrategySession>
      current_strategy_session_ ABSL_GUARDED_BY(mutex_);
  absl::Time started_strategy_session_time_ ABSL_GUARDED_BY(mutex_);

  // Current AdvertisingPhase
  std::unique_ptr<
      location::nearby::analytics::proto::ConnectionsLog::AdvertisingPhase>
      current_advertising_phase_;
  absl::Time started_advertising_phase_time_ = absl::InfinitePast();

  // Current DiscoveryPhase
  std::unique_ptr<
      location::nearby::analytics::proto::ConnectionsLog::DiscoveryPhase>
      current_discovery_phase_;
  absl::Time started_discovery_phase_time_ = absl::InfinitePast();

  absl::btree_map<std::string,
                  std::unique_ptr<location::nearby::analytics::proto::
                                      ConnectionsLog::ConnectionRequest>>
      incoming_connection_requests_ ABSL_GUARDED_BY(mutex_);
  absl::btree_map<std::string,
                  std::unique_ptr<location::nearby::analytics::proto::
                                      ConnectionsLog::ConnectionRequest>>
      outgoing_connection_requests_ ABSL_GUARDED_BY(mutex_);
  absl::btree_map<std::string, std::unique_ptr<LogicalConnection>>
      active_connections_ ABSL_GUARDED_BY(mutex_);
  absl::btree_map<std::string,
                  std::unique_ptr<location::nearby::analytics::proto::
                                      ConnectionsLog::BandwidthUpgradeAttempt>>
      bandwidth_upgrade_attempts_ ABSL_GUARDED_BY(mutex_);
};

}  // namespace analytics
}  // namespace nearby

#endif  // ANALYTICS_ANALYTICS_RECORDER_H_
