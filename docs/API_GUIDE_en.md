# Rysen ApexHand SDK API Guide

This guide is for SDK users and covers the public C++ API declared in `sdk/include/rysen_apexhand_sdk.hpp`. Refer to `rysen_apexhand_data.hpp` for complete data-structure and enum definitions.

## 1. General conventions

### Connection and return values

Unless an API explicitly returns an object or `bool`, public command APIs return `ErrorCode`. The only success condition is:

```cpp
if (result == rysen::ErrorCode::ERROR_CODE_OK) {
  // success
}
```

Typical call sequence: `Connect` → read state / configure limits → enable or start a control mode → `Disconnect`.

```cpp
rysen::Rysen hand;
if (hand.Connect("192.168.0.102") != rysen::ErrorCode::ERROR_CODE_OK) {
  return;
}
hand.SetAllFingersEnabled();       // enter position control
// ... control ...
hand.Disconnect();
```

The currently supported connection type is `ConnectionType::CONNECTION_TYPE_ETHERNET`.

### Units and ordering

| Data | Unit / ordering |
|---|---|
| Joint position, velocity, acceleration | rad, rad/s, rad/s² |
| Motor feedback current | mA |
| Fingertip force | N, expressed in each fingertip's local coordinate frame |
| Tactile normal-force image | Pa |
| Finger order | Thumb, Index, Middle, Ring, Little |
| Regular joints | The 21-joint space of `JointId`; `JOINT_ID_INDEX_J3`, `JOINT_ID_MIDDLE_J3`, `JOINT_ID_RING_J3`, and `JOINT_ID_PINKY_J3` are coupled with the corresponding `JOINT_ID_*_J2`. |
| MIT | The 16-axis joint space of `MITDof`, not `MotorId` or protocol-slot order |

## 2. Error codes

| `ErrorCode` | Meaning and recommended action |
|---|---|
| `OK` | The call succeeded. |
| `COMM_ERROR` | Not connected, connection lost, or send/receive failure; check the network and `IsConnected()`. |
| `TIMEOUT` | Timed out while waiting for a firmware response, motion completion, or calibration result. |
| `OUT_OF_RANGE` | A position, velocity, acceleration, or other value is outside its allowed range. |
| `OVER_SPEED` | Velocity or acceleration exceeds the follow/planning limit. |
| `OTHER_ERROR` | An unspecified failure; inspect the SDK log. |
| `INVALID_ARGUMENT` | Invalid call input, such as a null parameter, mismatched count, or invalid enum/frequency. |
| `CONFIG_ERROR` | The SDK or runtime environment does not meet a required configuration. |
| `NOT_IMPLEMENTED` | This capability is not implemented in the current version. |
| `URDF_ERROR` | The URDF path does not exist or the URDF could not be parsed. |
| `HARDWARE_ERROR` | Firmware reported a hardware fault; use the hardware-error callback for detailed codes for all fingers. |
| `NETWORK_LATENCY_HIGH` | TCP latency check before `Connect` exceeded the threshold. |

## 3. Connection, enablement, and configuration

| API | Arguments | Return / behavior |
|---|---|---|
| `Connect(address, type)` | IP address; Ethernet by default | Establishes control and feedback connections. |
| `Disconnect()` | None | Safely exits control and disconnects. |
| `IsConnected()` | None | `bool`; indicates only the SDK communication-connection state. |
| `GetVersionInfo()` | None | `VersionInfo`: SDK, hand, and tactile firmware versions. |
| `GetHardwareUid()` | None | 96-bit UID string; empty when disconnected. |
| `SetDeviceIPAddress(ip)` | IPv4 string | Must be connected; waits for device confirmation. |
| `SetAllFingersEnabled()` | None | Enables every finger and enters position control. |
| `SetAllFingersDisabled()` | None | Disables every finger. |
| `SetFingerEnabled(fingers)` / `SetFingerDisabled(fingers)` | `std::vector<FingerId>` | Operates only on the specified fingers. |
| `IsFingerEnabled(finger)` | `FingerId` | `bool`; reads the latest DD feedback's actual active-control state: `0x07` (position) or `0xCC` (current) is `true`. A successful `Set*FingersEnabled/Disabled` has waited for its corresponding feedback state. |
| `CleanFaults()` | None | Requests firmware fault clearing. |
| `SetLogPath(path)` / `EnableLogging(enable)` | Log directory / switch | Configures SDK logging. |
| `SetRobotModelFromURDF(path)` | URDF path | Can be called before or after connection; failure returns `URDF_ERROR`. |

## 4. Position control and limits

Optionally configure SDK command limits with `SetMax*`, then send position-control commands.

### Fixed joint software limits

The following fixed SDK position software limits are in rad. Endpoints are allowed, and the limits cannot be changed through the public API. They constrain control targets in the SDK; they are not hardware-reachable physical limits or encoder-feedback bounds. For example, feedback at a hard stop may lie slightly outside this range.

| `JointId` | Minimum | Maximum |
|---|---:|---:|
| `JOINT_ID_THUMB_J0` | 0.0 | 1.5708 |
| `JOINT_ID_THUMB_J1` | -0.1745 | 1.0470 |
| `JOINT_ID_THUMB_J2` | 0.0 | 1.3963 |
| `JOINT_ID_THUMB_J3` | -0.3490 | 1.3963 |
| `JOINT_ID_THUMB_J4` | -0.3490 | 1.3963 |
| `JOINT_ID_INDEX_J0`, `JOINT_ID_MIDDLE_J0`, `JOINT_ID_RING_J0`, `JOINT_ID_PINKY_J0` | -0.4360 | 0.4360 |
| `JOINT_ID_INDEX_J1`, `JOINT_ID_MIDDLE_J1`, `JOINT_ID_RING_J1`, `JOINT_ID_PINKY_J1` | -0.3490 | 1.5708 |
| `JOINT_ID_INDEX_J2`, `JOINT_ID_MIDDLE_J2`, `JOINT_ID_RING_J2`, `JOINT_ID_PINKY_J2` | -0.0873 | 1.7453 |
| `JOINT_ID_INDEX_J3`, `JOINT_ID_MIDDLE_J3`, `JOINT_ID_RING_J3`, `JOINT_ID_PINKY_J3` | -0.0873 | 1.7453 |

For an out-of-range target, `MoveJoint`, `StartMITControl`, and `UpdateMITReference` reject the request and return `OUT_OF_RANGE`. `MoveJPositionFollow` and `MoveJControlFollow` clamp the target to the table above and return `OUT_OF_RANGE`; callers should still treat that return value as a failure and correct subsequent targets.

| API | Input structure / fields | Behavior |
|---|---|---|
| `SetMaxJointSpeed(items)` | `MaxJointSpeed{joint_id, speed}` | Maximum velocity per joint, in rad/s. |
| `SetMaxJointAccel(items)` | `MaxJointAccel{joint_id, accel}` | Maximum acceleration per joint, in rad/s². |
| `SetMaxFingerTorque(items)` | `MaxFingerTorque{finger_id, torque}` | Maximum torque ratio per finger, 0–100%. |
| `GetParameters()` | None | Returns the current SDK limit parameters as `ParamInfo`. |
| `MoveJoint(commands)` | `JointControlParam{joint_id, position, velocity, acceleration}` | Blocking position motion until completion or failure. |
| `MoveJPositionFollow(params)` | `MoveJPositionFollowParam{id, position, torque}` | Non-blocking position follow; the SDK interpolates adjacent targets. `torque` is retained only for compatibility, has no effect, and cannot be used for torque control. |
| `MoveJControlFollow(commands)` | List of `JointControlParam` | Non-blocking direct follow; position / velocity / acceleration are sent unchanged as position-control targets, without tracker smoothing. |

Use `MoveJoint` for discrete point-to-point motions. Use `MoveJPositionFollow` or `MoveJControlFollow` when targets must be updated continuously. All position commands must remain within mechanical limits.

## 5. MIT (joint-space compliant / impedance control)

MIT has an asynchronous control lifecycle. After `StartMITControl`, the SDK starts from the current measured pose and ramps gains from zero over `gain_ramp_duration_ms`; reference and gain updates are non-blocking.

```cpp
rysen::MITConfig config = rysen::mit_presets::RecommendedConfig();
config.friction_compensation = rysen::FrictionCompensationMode::CALIBRATED_MLP;

if (hand.StartMITControl(config) == rysen::ErrorCode::ERROR_CODE_OK) {
  rysen::MITReference reference{};
  reference.position_rad[static_cast<size_t>(rysen::MITDof::JOINT_ID_INDEX_J2)] = 0.4;
  hand.UpdateMITReference(reference);
}
```

| API | Arguments | Description |
|---|---|---|
| `StartMITControl(config)` | `MITConfig` | Starts MIT. `gains` is the initial `MITGains`; `mit_presets::RecommendedConfig()` is available. Returns `OUT_OF_RANGE` if any axis stiffness exceeds twice its corresponding recommended stiffness. |
| `UpdateMITReference(reference)` | `MITReference` | Atomically updates the 16-axis `position_rad`, `velocity_rad_s`, and `feedforward_torque_nm`. |
| `UpdateMITGains(gains)` | `MITGains` | Atomically updates the 16-axis `stiffness_nm_per_rad` and `damping_nm_s_per_rad`. Returns `OUT_OF_RANGE` if any axis stiffness exceeds twice its corresponding recommended stiffness; no axis is updated. |
| `StopMITControl(mode)` | `DISABLE` or `HOLD_POSITION` | `DISABLE` disables the hand; `HOLD_POSITION` transitions to position control at the current pose. |
| `GetMITControlState(state)` | Output `MITControlState` | Reads SDK-side state, reference/gain sequence numbers, and recent mode information. |

`MITConfig` fields:

- `reference.position_rad`, `reference.velocity_rad_s`, `reference.feedforward_torque_nm`: target position (rad), target velocity (rad/s), and user feedforward joint torque (N·m), respectively; all are 16-element arrays. Feedforward torque is added directly to the PD output, followed by optional friction compensation. It is not scaled by the startup Kp/Kd ramp; applications must smooth feedforward-torque discontinuities themselves.
- `gains`: stiffness (N·m/rad) and damping (N·m·s/rad), both 16-element arrays. Each axis stiffness must not exceed twice its corresponding value in `mit_presets::kRecommendedGains`; an out-of-range value returns `OUT_OF_RANGE` and is not clamped.
- `friction_compensation`: `DISABLED` or `CALIBRATED_MLP` (default).
- `friction_gain`: scaling factor for friction feedforward.
- `gain_ramp_duration_ms`: gain-ramp duration at startup; 200 ms by default.

MIT arrays are indexed strictly by the `MITDof` enum: first `JOINT_ID_THUMB_J0` through `JOINT_ID_THUMB_J3`, then `JOINT_ID_INDEX_J0` through `JOINT_ID_INDEX_J2`, `JOINT_ID_MIDDLE_J0` through `JOINT_ID_MIDDLE_J2`, `JOINT_ID_RING_J0` through `JOINT_ID_RING_J2`, and `JOINT_ID_PINKY_J0` through `JOINT_ID_PINKY_J2`. `JOINT_ID_THUMB_J4`, `JOINT_ID_INDEX_J3`, `JOINT_ID_MIDDLE_J3`, `JOINT_ID_RING_J3`, and `JOINT_ID_PINKY_J3` cannot be controlled independently.

## 6. Feedback access and callbacks

| API | Successful output | Description |
|---|---|---|
| `GetJointStates(states)` | `JointStates` | Latest joint position, velocity, acceleration, and torque feedback. |
| `GetMotorStates(states)` | `MotorStates` | Latest feedback for 16 motor temperatures and currents (mA). |
| `GetHandRuntimeLimits(limits)` | `HandRuntimeLimits` | Per-finger torque-ratio and velocity limits actually applied by firmware. Each item has `valid=false` on firmware before 3.2.10. |
| `GetFingerTipForces(forces)` | `FingerTipForces` | On-demand estimate of five 3D fingertip forces from current feedback, torque mapping, and kinematics; this is not a direct force-sensor measurement. |
| `GetHandSensorImage(image)` | `HandSensorImage` | Latest tactile image. |

State-callback example:

```cpp
hand.RegisterGetJointStatesCallback(
    [](const rysen::JointStates& states) { /* keep callback short */ }, 100);
```

| Registration / unregistration | Default frequency | Callback argument |
|---|---:|---|
| `RegisterGetJointStatesCallback` / `UnregisterGetJointStatesCallback` | 100 Hz | `const JointStates&` |
| `RegisterGetMotorStatesCallback` / `UnregisterGetMotorStatesCallback` | 100 Hz | `const MotorStates&` |
| `RegisterGetHandSensorImageCallback` / `UnregisterGetHandSensorImageCallback` | 100 Hz | `const HandSensorImage&` |

Callbacks read the SDK's latest cache. A callback is not guaranteed to correspond to exactly one new feedback frame. Callbacks run in a thread context; avoid blocking, lengthy computation, or calling SDK APIs that may block from within a callback.

## 7. Hardware errors and tactile calibration

| API | Usage |
|---|---|
| `GetHardwareErrorCode()` | Returns `OK` when no hardware fault is present; returns `HARDWARE_ERROR` when any device/finger fault is detected. |
| `RegisterHardwareErrorEventCallback(cb)` | Callback argument is `const HardwareErrorCodes&`, containing 64-bit error codes for the device and each of five fingers. |
| `UnregisterHardwareErrorEventCallback()` | Unregisters the preceding callback. |
| `StartTactileCalibration()` | Blocks for approximately 200 ms. Keep all tactile regions free of external contact during this period; calibration takes effect only on success, and returns `TIMEOUT` if no usable sample is obtained. Do not call this from a data callback or high-frequency control loop. |
| `ClearTactileCalibration()` | Clears current tactile zero offsets and tangential bias. |
| `GetHandDir()` | Returns `HandDir::LEFT` or `HandDir::RIGHT`. |

The detailed meanings of hardware-error bits are defined by the firmware protocol. Applications should retain the raw `HardwareErrorCodes` values and interpret them against the corresponding firmware-protocol version.

## 8. Python mapping

The Python package exposes the same capabilities and data structures, with method names converted to `snake_case`: for example, `StartMITControl` maps to `start_mit_control`, and `GetFingerTipForces` maps to `get_finger_tip_forces`. Python state-read APIs return `None` on failure instead of the C++ `ErrorCode + output parameter` form.
