/**
 * @file    rysen_apexhand_data.hpp
 * @brief   Public data structures for the rysen_apexhand_sdk.
 *
 * Copyright (c) 2024-2026, Rysen Robotics (Shenzhen) Co. Ltd.
 * All rights reserved.
 *
 * Use of this source code is governed by a BSD 3-Clause license that can be
 * found in the LICENSE file.
 */

#ifndef RYSEN_APEXHAND_SDK_INCLUDE_RYSEN_APEXHAND_DATA_H_
#define RYSEN_APEXHAND_SDK_INCLUDE_RYSEN_APEXHAND_DATA_H_

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace rysen {

enum class LogLevel : uint8_t {
  LOG_LEVEL_DEBUG = 0,
  LOG_LEVEL_INFO = 1,
  LOG_LEVEL_WARN = 2,
  LOG_LEVEL_ERROR = 3,
  LOG_LEVEL_FATAL = 4

};

// 连接类型 rs485 ethernet
enum class ConnectionType : uint8_t { CONNECTION_TYPE_RS485 = 0, CONNECTION_TYPE_ETHERNET = 1 };
// 错误码
enum class ErrorCode : uint64_t {
  ERROR_CODE_OK = 0,                    // 操作成功
  ERROR_CODE_COMM_ERROR = 1,            // 通讯错误（连接中断、发送/接收失败等）
  ERROR_CODE_TIMEOUT = 2,               // 超时错误（等待设备响应超时）
  ERROR_CODE_OUT_OF_RANGE = 3,          // 参数超出允许范围（位置/速度/加速度等）
  ERROR_CODE_OVER_SPEED = 4,            // 速度/加速度过大（超过规划/跟随允许范围）
  ERROR_CODE_OTHER_ERROR = 5,           // 其他错误
  ERROR_CODE_INVALID_ARGUMENT = 6,      // 非法参数（数量不匹配、频率不合法、空列表等）
  ERROR_CODE_CONFIG_ERROR = 7,          // 配置/环境错误（日志配置失败、传感器不可用等）
  ERROR_CODE_NOT_IMPLEMENTED = 8,       // 功能未实现（预留接口）
  ERROR_CODE_URDF_ERROR = 9,            // URDF 文件错误（文件不存在、解析失败等）
  ERROR_CODE_HARDWARE_ERROR = 10,       // 硬件错误（通过 HardwareErrorCodes 检测到硬件故障）
  ERROR_CODE_NETWORK_LATENCY_HIGH = 11  // 预连接 TCP 延迟测量超过阈值（Connect 前检查）
};

// 左右手
enum class HandDir {
  LEFT = 0,
  RIGHT = 1,
};

// 手指标识（命名见 NAMING_CONVENTION.md）
enum class FingerId : uint8_t {
  FINGER_ID_THUMB = 0,
  FINGER_ID_INDEX = 1,
  FINGER_ID_MIDDLE = 2,
  FINGER_ID_RING = 3,
  FINGER_ID_LITTLE = 4
};

// 电机标识 16 个电机（命名与顺序见 NAMING_CONVENTION.md / ROS motor_states）
enum class MotorId : uint8_t {
  MOTOR_ID_THUMB_CMC_ABD = 0,           // f0_motor0 拇指腕掌关节 侧摆
  MOTOR_ID_THUMB_CMC_ROT = 1,           // f0_motor1 拇指腕掌关节 旋转
  MOTOR_ID_THUMB_CMC_FLEX = 2,          // f0_motor2 拇指腕掌关节 弯曲
  MOTOR_ID_THUMB_MCP_FLEX = 3,          // f0_motor3 拇指掌指关节 弯曲
  MOTOR_ID_INDEX_MCP_ABD_FLEX_0 = 4,    // f1_motor0 食指掌指关节 侧摆 弯曲
  MOTOR_ID_INDEX_MCP_ABD_FLEX_1 = 5,    // f1_motor1 食指掌指关节 侧摆 弯曲
  MOTOR_ID_INDEX_PIP_FLEX = 6,          // f1_motor2 食指近端指间关节 弯曲
  MOTOR_ID_MIDDLE_MCP_ABD_FLEX_0 = 7,   // f2_motor0 中指掌指关节 侧摆 弯曲
  MOTOR_ID_MIDDLE_MCP_ABD_FLEX_1 = 8,   // f2_motor1 中指掌指关节 侧摆 弯曲
  MOTOR_ID_MIDDLE_PIP_FLEX = 9,         // f2_motor2 中指近端指间关节 弯曲
  MOTOR_ID_RING_MCP_ABD_FLEX_0 = 10,    // f3_motor0 无名指掌指关节 侧摆 弯曲
  MOTOR_ID_RING_MCP_ABD_FLEX_1 = 11,    // f3_motor1 无名指掌指关节 侧摆 弯曲
  MOTOR_ID_RING_PIP_FLEX = 12,          // f3_motor2 无名指近端指间关节 弯曲
  MOTOR_ID_LITTLE_MCP_ABD_FLEX_0 = 13,  // f4_motor0 小指掌指关节 侧摆 弯曲
  MOTOR_ID_LITTLE_MCP_ABD_FLEX_1 = 14,  // f4_motor1 小指掌指关节 侧摆 弯曲
  MOTOR_ID_LITTLE_PIP_FLEX = 15,        // f4_motor2 小指近端指间关节 弯曲
};

// 关节标识 21 个关节
enum class JointId : uint8_t {
  JOINT_ID_THUMB_J0 = 0,  // f0_joint0 拇指腕掌关节 侧摆
  JOINT_ID_THUMB_J1 = 1,  // f0_joint1 拇指腕掌关节 旋转
  JOINT_ID_THUMB_J2 = 2,  // f0_joint2 拇指腕掌关节 弯曲
  JOINT_ID_THUMB_J3 = 3,  // f0_joint3 拇指掌指关节 弯曲
  JOINT_ID_THUMB_J4 = 4,  // f0_joint4 拇指指间关节 弯曲

  JOINT_ID_INDEX_J0 = 5,  // f1_joint0 食指掌指关节 侧摆
  JOINT_ID_INDEX_J1 = 6,  // f1_joint1 食指掌指关节 弯曲
  JOINT_ID_INDEX_J2 = 7,  // f1_joint2 食指近端指间关节 弯曲
  JOINT_ID_INDEX_J3 = 8,  // f1_joint3 食指远端指间关节 弯曲

  JOINT_ID_MIDDLE_J0 = 9,   // f2_joint0 中指掌指关节 侧摆
  JOINT_ID_MIDDLE_J1 = 10,  // f2_joint1 中指掌指关节 弯曲
  JOINT_ID_MIDDLE_J2 = 11,  // f2_joint2 中指近端指间关节 弯曲
  JOINT_ID_MIDDLE_J3 = 12,  // f2_joint3 中指远端指间关节 弯曲

  JOINT_ID_RING_J0 = 13,  // f3_joint0 无名指掌指关节 侧摆
  JOINT_ID_RING_J1 = 14,  // f3_joint1 无名指掌指关节 弯曲
  JOINT_ID_RING_J2 = 15,  // f3_joint2 无名指近端指间关节 弯曲
  JOINT_ID_RING_J3 = 16,  // f3_joint3 无名指远端指间关节 弯曲

  JOINT_ID_PINKY_J0 = 17,  // f4_joint0 小指掌指关节 侧摆
  JOINT_ID_PINKY_J1 = 18,  // f4_joint1 小指掌指关节 弯曲
  JOINT_ID_PINKY_J2 = 19,  // f4_joint2 小指近端指间关节 弯曲
  JOINT_ID_PINKY_J3 = 20,  // f4_joint3 小指远端指间关节 弯曲

  JOINT_ID_THUMB_CMC_ABD = JOINT_ID_THUMB_J0,
  JOINT_ID_THUMB_CMC_ROT = JOINT_ID_THUMB_J1,
  JOINT_ID_THUMB_CMC_FLEX = JOINT_ID_THUMB_J2,
  JOINT_ID_THUMB_MCP_FLEX = JOINT_ID_THUMB_J3,
  JOINT_ID_THUMB_IP_FLEX = JOINT_ID_THUMB_J4,

  JOINT_ID_INDEX_MCP_ABD = JOINT_ID_INDEX_J0,
  JOINT_ID_INDEX_MCP_FLEX = JOINT_ID_INDEX_J1,
  JOINT_ID_INDEX_PIP_FLEX = JOINT_ID_INDEX_J2,
  JOINT_ID_INDEX_DIP_FLEX = JOINT_ID_INDEX_J3,

  JOINT_ID_MIDDLE_MCP_ABD = JOINT_ID_MIDDLE_J0,
  JOINT_ID_MIDDLE_MCP_FLEX = JOINT_ID_MIDDLE_J1,
  JOINT_ID_MIDDLE_PIP_FLEX = JOINT_ID_MIDDLE_J2,
  JOINT_ID_MIDDLE_DIP_FLEX = JOINT_ID_MIDDLE_J3,

  JOINT_ID_RING_MCP_ABD = JOINT_ID_RING_J0,
  JOINT_ID_RING_MCP_FLEX = JOINT_ID_RING_J1,
  JOINT_ID_RING_PIP_FLEX = JOINT_ID_RING_J2,
  JOINT_ID_RING_DIP_FLEX = JOINT_ID_RING_J3,

  JOINT_ID_LITTLE_MCP_ABD = JOINT_ID_PINKY_J0,
  JOINT_ID_LITTLE_MCP_FLEX = JOINT_ID_PINKY_J1,
  JOINT_ID_LITTLE_PIP_FLEX = JOINT_ID_PINKY_J2,
  JOINT_ID_LITTLE_DIP_FLEX = JOINT_ID_PINKY_J3,
};

// 固件/库版本信息
struct VersionInfo {
  std::string touch_sensor_version;
  std::string hand_firmware_version;
  std::string sdk_version;
};

// 参数查询结构
struct ParamInfo {
  std::vector<double> max_speed{};
  std::vector<double> max_accel{};
  std::vector<double> max_current{};
  // TODO
};

// 单关节状态
struct JointState {
  JointId joint_id;
  double position = 0.0;
  double velocity = 0.0;
  double acceleration = 0.0;
  double torque = 0.0;
};

// 一组关节状态数据（包含时间戳）
struct JointStates {
  std::chrono::steady_clock::time_point timestamp;
  std::vector<JointState> joint_states;
};

// 单关节控制参数
struct JointControlParam {
  JointId joint_id;
  double position = 0.0;
  double velocity = 0.0;
  double acceleration = 0.0;
};

// MoveJPositionFollow 的参数
struct MoveJPositionFollowParam {
  JointId id;             // 关节ID
  double position = 0.0;  // 目标关节位置
  // 保留该字段仅为源代码兼容；MoveJPositionFollow 当前不读取它，用户不应将其
  // 视为力矩/力控制命令。
  double torque = 200.0;
};

// 阻抗控制只暴露机械上可独立驱动的 16 个关节轴。拇指 IP 与四指 DIP
// 是耦合轴，不作为独立阻抗通道；SDK 在内部完成 21 逻辑关节/16 电机映射。
enum class MITDof : uint8_t {
  // Names intentionally mirror the independently driven JointId values.
  // Coupled thumb IP and four-finger DIP joints have no MIT axis.
  JOINT_ID_THUMB_J0 = 0,
  JOINT_ID_THUMB_J1 = 1,
  JOINT_ID_THUMB_J2 = 2,
  JOINT_ID_THUMB_J3 = 3,
  JOINT_ID_INDEX_J0 = 4,
  JOINT_ID_INDEX_J1 = 5,
  JOINT_ID_INDEX_J2 = 6,
  JOINT_ID_MIDDLE_J0 = 7,
  JOINT_ID_MIDDLE_J1 = 8,
  JOINT_ID_MIDDLE_J2 = 9,
  JOINT_ID_RING_J0 = 10,
  JOINT_ID_RING_J1 = 11,
  JOINT_ID_RING_J2 = 12,
  JOINT_ID_PINKY_J0 = 13,
  JOINT_ID_PINKY_J1 = 14,
  JOINT_ID_PINKY_J2 = 15,
};

constexpr std::size_t kMITDofCount = 16;
using MITDofValues = std::array<double, kMITDofCount>;

// Reference and gains are joint-space quantities in SI units. Each array is
// indexed by MITDof, never by MotorId or protocol slot.
struct MITReference {
  MITDofValues position_rad{};
  MITDofValues velocity_rad_s{};
  // Additive joint-space torque applied after the PD terms. Unit: N m.
  // This user-supplied value is independent of, and summed with, optional
  // friction-model feedforward. Unlike Kp/Kd, it is not gain-ramped.
  MITDofValues feedforward_torque_nm{};
};

struct MITGains {
  // Each stiffness entry must not exceed twice the corresponding entry in
  // mit_presets::kRecommendedGains; StartMITControl and UpdateMITGains return
  // ERROR_CODE_OUT_OF_RANGE otherwise.
  MITDofValues stiffness_nm_per_rad{};
  MITDofValues damping_nm_s_per_rad{};
};

enum class FrictionCompensationMode : uint8_t {
  DISABLED = 0,
   CALIBRATED_MLP= 1,
};

// The controller starts from the measured posture and ramps its gains from
// zero. This keeps StartMITControl safe even before the caller has
// published the first reference.
struct MITConfig {
  MITGains gains{};
  FrictionCompensationMode friction_compensation = FrictionCompensationMode::CALIBRATED_MLP;
  double friction_gain = 1.0;
  uint32_t gain_ramp_duration_ms = 200;
};

// A starting point for the public 16-axis impedance API.  These values were
// converted from the legacy Python MIT example: that example stores joint
// gains in mN m/rad and mN m s/rad, while the public SDK consistently uses
// N m/rad and N m s/rad.  Thumb IP and each four-finger DIP are coupled axes,
// so their legacy entries are intentionally not exposed here.
//
// This is an explicit recommendation, not an implicit MITConfig
// default. Applications may copy it and tune any individual axis; public MIT
// APIs allow stiffness up to twice the corresponding recommended value.
namespace mit_presets {

inline constexpr MITGains kRecommendedGains = {
    {{0.6, 0.6, 0.8, 0.4,  // Thumb CMC ABD, ROT, FLEX, MCP FLEX
      0.6, 0.8, 0.3,       // Index MCP ABD, FLEX, PIP FLEX
      0.5, 0.8, 0.3,       // Middle MCP ABD, FLEX, PIP FLEX
      0.5, 0.8, 0.3,       // Ring MCP ABD, FLEX, PIP FLEX
      0.5, 0.8, 0.3}},     // Little MCP ABD, FLEX, PIP FLEX
    {{0.012, 0.012, 0.006, 0.0095,  // Thumb CMC ABD, ROT, FLEX, MCP FLEX
      0.025, 0.035, 0.015,           // Index MCP ABD, FLEX, PIP FLEX
      0.025, 0.035, 0.015,           // Middle MCP ABD, FLEX, PIP FLEX
      0.025, 0.035, 0.015,           // Ring MCP ABD, FLEX, PIP FLEX
      0.025, 0.035, 0.015}}};        // Little MCP ABD, FLEX, PIP FLEX

inline MITConfig RecommendedConfig() {
  MITConfig config;
  config.gains = kRecommendedGains;
  return config;
}

}  // namespace mit_presets

enum class MITStopMode : uint8_t {
  DISABLE = 0,
  HOLD_POSITION = 1,
};

// Host-side observation only: the latest control frame sent by the SDK.
// This is intentionally not labelled as firmware feedback; the protocol
// feedback _Cmd field is not yet part of the public state data model.
enum class MITControlMode : uint8_t {
  IDLE = 0,
  POSITION = 1,
  CURRENT = 2,
};

struct MITControlState {
  bool active = false;
  bool friction_compensation_requested = false;
  bool friction_model_available = false;
  uint64_t reference_sequence = 0;
  uint64_t gains_sequence = 0;
  // Exact firmware feedback _Cmd values in Thumb, Index, Middle, Ring,
  // Little order.  They are protocol observations (for example, 0x01 idle,
  // 0x07 position and 0xCC current), not host-side requested modes.
  std::array<uint8_t, 5> firmware_feedback_commands{};
  // Position targets from the most recently transmitted POSITION frame, in
  // the 16-axis public order.  This is useful when diagnosing a mode handoff.
  bool has_last_sent_position_target = false;
  MITDofValues last_sent_position_target_rad{};
  MITControlMode last_sent_control_mode = MITControlMode::IDLE;
};

// 触觉切向力 方向 和大小
struct TangentialForce {
  double theta = 0.0;      // 切向力方向 0-2pi
  double magnitude = 0.0;  // 切向力大小
};

// 二维力图像
struct TactileImage {
  uint32_t width = 0;
  uint32_t height = 0;
  std::vector<uint32_t> gray_image;   // 法向力，单位 Pa
  TangentialForce tangential_forces;  // 切向力数据
};

// 四指：对应 NAMING_CONVENTION 中 <finger>_prox_pad / mid_pad / dist_pad
struct CommonFingerSensorImage {
  TactileImage prox_pad;
  TactileImage mid_pad;
  TactileImage dist_pad;
};

// 拇指：thumb_prox_pad / thumb_mid_pad / thumb_dist_pad
struct ThumbFingerSensorImage {
  TactileImage prox_pad;
  TactileImage mid_pad;
  TactileImage dist_pad;
};

struct HandSensorImage {
  std::chrono::steady_clock::time_point timestamp;
  CommonFingerSensorImage index_image;   // 食指
  CommonFingerSensorImage middle_image;  // 中指
  CommonFingerSensorImage ring_image;    // 无名指
  CommonFingerSensorImage little_image;  // 小拇指
  ThumbFingerSensorImage thumb_image;    // 大拇指
  TactileImage palm_center;              // 手掌中心区域
};

struct MotorState {
  MotorId motor_id;
  double temperature = 0.0;
  double current = 0.0;  // mA
};

// 一组电机状态数据（包含时间戳）
struct MotorStates {
  std::chrono::steady_clock::time_point timestamp;
  std::vector<MotorState> motors;
};

// Estimated contact force at one fingertip.  This is derived from motor
// current feedback, the hand torque model, and the fingertip Jacobian; it is
// not a direct force-sensor measurement.
struct FingerTipForce {
  FingerId finger_id = FingerId::FINGER_ID_THUMB;
  // [x, y, z] in the current local coordinate frame of this fingertip. Unit: N.
  std::array<double, 3> force_n{};
};

// Latest on-demand fingertip-force estimate, in Thumb, Index, Middle, Ring,
// Little order.  The timestamp is that of the joint-state feedback used for
// the kinematic solve.
struct FingerTipForces {
  std::chrono::steady_clock::time_point timestamp;
  std::array<FingerTipForce, 5> finger_forces{};
};

// Runtime limits currently applied by one finger controller.  These values
// are feedback from firmware, not SDK-configured command limits.
struct FingerRuntimeLimits {
  FingerId finger_id = FingerId::FINGER_ID_THUMB;
  // False when the connected firmware does not provide these fields (for
  // example, protocol versions older than 3.2.10).
  bool valid = false;
  // Firmware feedback field _MinTorqueRatio. Unit: percent.
  uint8_t torque_limit_percent = 0;
  // Firmware feedback field _MinVelAllow, converted from deg/s to rad/s.
  double joint_velocity_limit_rad_s = 0.0;
};

// Latest runtime-limit snapshot in Thumb, Index, Middle, Ring, Little order.
// It is independent of the active host-side control mode (position, MIT, or
// idle), because it represents firmware feedback.
struct HandRuntimeLimits {
  std::chrono::steady_clock::time_point timestamp;
  std::array<FingerRuntimeLimits, 5> finger_limits{};
};

struct MaxJointSpeed {
  JointId joint_id;
  double speed = 0.0;
};
struct MaxJointAccel {
  JointId joint_id;
  double accel = 0.0;
};
struct MaxFingerTorque {
  FingerId finger_id;
  double torque = 0.0;
};

// 硬件错误码结构
struct HardwareErrorCodes {
  uint64_t device_error_code = 0;  // 设备错误码
  uint64_t thumb_error_code = 0;   // 大拇指错误码
  uint64_t index_error_code = 0;   // 食指错误码
  uint64_t middle_error_code = 0;  // 中指错误码
  uint64_t ring_error_code = 0;    // 无名指错误码
  uint64_t little_error_code = 0;  // 小拇指错误码
};

}  // namespace rysen

#endif  // RYSEN_APEXHAND_SDK_INCLUDE_RYSEN_APEXHAND_DATA_H_
