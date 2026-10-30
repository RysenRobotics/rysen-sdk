# Rysen ApexHand SDK Python

This directory contains the delivered Python facade for the Rysen ApexHand SDK.
The public class is `Rysen`; its methods use `snake_case` and return an
`ErrorCode` unless the method documentation states otherwise.

## Directory Structure

```
python/
├── example.py
├── __init__.py
├── MANIFEST.in
├── README.md
├── rysen_apexhand_sdk.py
└── setup.py
```

## Installation

### Prerequisites

1. **Python Version**: Python 3.10
2. **System Dependencies**:

   **Quick Installation (Recommended)**:
   ```bash
   # In the root directory of the repository
   chmod +x install_rysen_deps.sh
   ./install_rysen_deps.sh
   ```

   **Manual Installation of Runtime Libraries**:
   ```bash
   sudo apt-get update
   sudo apt-get install -y \
     libspdlog1 \
     libfmt8 \
     libboost-system1.74.0 \
     libboost-thread1.74.0
   sudo ldconfig
   ```

### Installation Steps

1. **Navigate to the Python directory**:
   ```bash
   cd python/
   ```

2. **Install the Python package**:
   ```bash
   pip install -e .
   ```
   
   Or use `pip3`:
   ```bash
   pip3 install -e .
   ```

   After installation, the `rysen_apexhand_sdk` module can be used in Python.

## Usage Examples

### Basic usage

```python
from rysen_apexhand_sdk import ConnectionType, ErrorCode, Rysen

sdk = Rysen()

if sdk.connect("192.168.0.102", ConnectionType.CONNECTION_TYPE_ETHERNET) == ErrorCode.ERROR_CODE_OK:
    sdk.set_all_fingers_enabled()
    # Perform control operations.
    sdk.disconnect()
```

### Run the Example Program

```bash
cd python
python example.py
```

Or use Python 3:
```bash
cd python
python3 example.py
```

## API Documentation

### Connection management

- `connect(address, connection_type=ConnectionType.CONNECTION_TYPE_ETHERNET)`
- `disconnect()`, `is_connected()`, `get_version_info()`, `get_hardware_uid()`
- `set_device_ip_address(ip_address)`

### Finger and position control

- `set_all_fingers_enabled()` / `set_all_fingers_disabled()`
- `set_finger_enabled(fingers)` / `set_finger_disabled(fingers)`
- `move_joint(commands)`: blocking position movement.
- `move_j_position_follow(params)`: asynchronous interpolated position follow.
- `move_j_control_follow(commands)`: asynchronous direct position / velocity /
  acceleration follow, without the position tracker.

### MIT compliant control

MIT operates in **16-axis joint space**, ordered by `MITDof`; it is neither
the 21-element `JointId` order nor a motor/protocol-slot order.  Use
`recommended_mit_config()` as the initial configuration.

- `start_mit_control(config)` starts asynchronous MIT control.  It captures the
  current posture, then ramps gains from zero using
  `config.gain_ramp_duration_ms`.
- `update_mit_reference(reference)` atomically updates 16 position and velocity
  references.
- `update_mit_gains(gains)` atomically updates 16 stiffness and damping gains.
- `stop_mit_control(MITStopMode.DISABLE)` exits MIT.  Use
  `MITStopMode.HOLD_POSITION` to hand off to position control at the measured
  posture.
- `get_mit_control_state()` returns `(ErrorCode, MITControlState)`.

`MITConfig.friction_compensation` defaults to
`FrictionCompensationMode.CALIBRATED_MLP`; set it to `DISABLED` to disable
friction feedforward.

### Firmware 3.2.10 feedback

- `get_hand_runtime_limits()` returns `HandRuntimeLimits` (or `None` when no
  snapshot is available).  The five `finger_limits` entries are in
  thumb-to-little order and expose firmware feedback `_MinTorqueRatio` in
  percent plus `_MinVelAllow` converted to rad/s.  `valid` is `False` on
  firmware older than protocol 3.2.10.

### Fingertip force estimate

- `get_finger_tip_forces()` returns `FingerTipForces` (or `None` if the current
  feedback cannot be used).  Each `force_n` is `[x, y, z]` in N, expressed in
  that fingertip's local frame.  It is inferred from motor-current feedback,
  torque mapping, and kinematics; it is not a direct force-sensor reading.

### State and callbacks

- `get_joint_states()`, `get_motor_states()`, and `get_hand_sensor_image()`
  return the latest cached feedback object or `None`.
- `register_joint_states_callback(callback, freq_hz=100)`
- `register_motor_states_callback(callback, freq_hz=100)`
- `register_hand_sensor_image_callback(callback, freq_hz=100)`
- Matching `unregister_*_callback()` methods stop each callback.

Callbacks periodically read the latest SDK cache.  They are not a reliable
one-callback-per-feedback-frame stream.

### Other public interfaces

- `set_max_joint_speed(speeds)`, `set_max_joint_accel(accels)`, and
  `set_max_finger_torque(torques)` configure SDK command limits.
- `get_hardware_error_code()` and
  `register_hardware_error_event_callback(callback)` report hardware faults.
- `start_tactile_calibration()` / `clear_tactile_calibration()` manage tactile
  calibration.
- `set_robot_model_from_urdf(urdf_path)` loads an optional robot model.

## Notes

1. **Library File Version Compatibility**: The `.so` files in the `../rysen_sdk/lib/**/` directory must match the target Python version
   - Example: `_rysen_sdk.cpython-310-*.so` corresponds to Python 3.10

2. **Connection Method**: Only Ethernet connection (`connection_type=1`) is supported currently
   - Device IP: 192.168.0.102

3. **Network Configuration**: Ensure the robot and host are on the same network, and the firewall allows communication on relevant ports

4. **Real-Time Kernel**: It is recommended to use a real-time kernel (PREEMPT_RT) for better control performance

5. **Error Handling**: Command/configuration APIs return `ErrorCode`; compare it
   with `ErrorCode.ERROR_CODE_OK`.  Latest-state getters return their data
   object, or `None` when the requested snapshot is unavailable.

## Troubleshooting

### Import Errors

If you encounter an `ImportError`, check the following:
1. Whether the Python package is installed correctly: `pip list | grep rysen`
2. Whether the library files exist: `ls ../rysen_sdk/lib/`
3. Whether the system dependencies are installed: `ldconfig -p | grep spdlog`

### Connection Failures

1. Check if the robot IP address is correct
2. Check the network connection: `ping <Robot IP>`
3. Check if the port is occupied: `netstat -an | grep 5856`

### Runtime Errors

1. Ensure the system dependencies are installed correctly
2. Check library file permissions: `ls -l examples/python/lib/`
3. Check the detailed error information and verify Python version compatibility

## More Information

- Complete example code: Refer to `example.py`
- C++ API documentation: Refer to `../cpp/rysen_example.cpp`
- ROS2 interface: Refer to `../ros2/rysen_apexhand/README.md`
