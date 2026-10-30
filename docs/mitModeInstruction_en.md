# MIT Mode User Guide

## 0. Overview

MIT (Mixed Integrated Torque) mode is a hybrid torque control mode that combines position, velocity, and torque control within a single control framework. Its general control law is:

$$
\tau_J = K_p(q_{ref} - q) + K_d(\dot{q}_{ref} - \dot{q}) + \tau_{ff}(q, \dot{q})
$$

## 1. Torque Control Mode

When both $K_p$ and $K_d$ are set to 0, the controller operates in torque control mode.

The maximum output torque of each joint is limited by the maximum output current of its motor. The table below lists the theoretical maximum single-axis joint torques derived from the transmission parameters. These values assume that no other joint is requesting torque and that the available current headroom has not been consumed by PD control, spring compensation, or friction compensation.

| Finger/Joint | Maximum Absolute Joint Torque (N·m) | Equivalent Value (N·mm) |
|---|---:|---:|
| THUMB_J0 | 0.600 | 600 |
| THUMB_J1 | 0.600 | 600 |
| THUMB_J2 | 0.918 | 918 |
| THUMB_J3 / THUMB_J4 coupled total torque | 0.923 | 923 |
| INDEX_J0 | 1.818 | 1818 |
| INDEX_J1 | 2.272 | 2272 |
| INDEX_J2 / INDEX_J3 coupled total torque | 1.492 | 1492 |
| MIDDLE_J0 | 1.818 | 1818 |
| MIDDLE_J1 | 2.272 | 2272 |
| MIDDLE_J2 / MIDDLE_J3 coupled total torque | 1.492 | 1492 |
| RING_J0 | 1.818 | 1818 |
| RING_J1 | 2.272 | 2272 |
| RING_J2 / RING_J3 coupled total torque | 1.492 | 1492 |
| PINKY_J0 | 1.818 | 1818 |
| PINKY_J1 | 2.272 | 2272 |
| PINKY_J2 / PINKY_J3 coupled total torque | 1.492 | 1492 |

Please note the following when using these values:

1. The PIP/DIP joints of the four non-thumb fingers and the MCP/IP joints of the thumb use coupled transmissions. The table specifies the total available torque of the corresponding tendon, not a limit that each of the two joints can reach simultaneously.
2. From the index finger through the little finger, J0 (MCP_ABD) and J1 (MCP_FLEX) are jointly driven by the same pair of motors and share their output capacity. Each value in the table is the theoretical maximum for one joint acting independently while the associated joint requests no torque. Do not set $\tau_{ff}$ for both joints to their respective listed limits at the same time. When J0 and J1 output torque simultaneously, consider both torque requests and the shared available headroom.
3. Thumb J0 (CMC_ABD) and J1 (CMC_ROT) also share part of their output capacity. Their listed values are likewise theoretical limits for independent single-joint operation and must not be applied simultaneously to both joints as $\tau_{ff}$ limits. When J1 requests a high torque, the torque available to J0 may be significantly reduced. Pay particular attention to the output headroom of J0 and reduce its feedforward torque as appropriate.
4. The listed values must not be treated as unconditional fixed limits for $\tau_{ff}$. They represent theoretical torque headroom before any capacity is consumed by PD control, spring compensation, or friction compensation.

## 2. Impedance Control Mode

The joint position gain $K_p$ and joint velocity gain $K_d$ can be adjusted for the intended application. The following values are provided as a reference:

| MIT Index | Finger | Joint | Kp (N·m/rad) | Kd (N·m·s/rad) |
|---:|---|---|---:|---:|
| 0 | Thumb | THUMB_J0 | 0.600 | 0.0120 |
| 1 | Thumb | THUMB_J1 | 0.600 | 0.0120 |
| 2 | Thumb | THUMB_J2 | 0.800 | 0.0060 |
| 3 | Thumb | THUMB_J3 | 0.400 | 0.0095 |
| 4 | Index | INDEX_J0 | 0.600 | 0.0250 |
| 5 | Index | INDEX_J1 | 0.800 | 0.0350 |
| 6 | Index | INDEX_J2 | 0.300 | 0.0150 |
| 7 | Middle | MIDDLE_J0 | 0.500 | 0.0250 |
| 8 | Middle | MIDDLE_J1 | 0.800 | 0.0350 |
| 9 | Middle | MIDDLE_J2 | 0.300 | 0.0150 |
| 10 | Ring | RING_J0 | 0.500 | 0.0250 |
| 11 | Ring | RING_J1 | 0.800 | 0.0350 |
| 12 | Ring | RING_J2 | 0.300 | 0.0150 |
| 13 | Little | PINKY_J0 | 0.500 | 0.0250 |
| 14 | Little | PINKY_J1 | 0.800 | 0.0350 |
| 15 | Little | PINKY_J2 | 0.300 | 0.0150 |

Performance may vary between devices because of assembly tolerances and other unit-specific factors. The same $K_p$ and $K_d$ values may therefore produce different responses on different devices. Adjust these parameters based on the behavior of the device being used.

### 2.1 Friction Compensation

MIT mode provides optional built-in friction compensation. This feature uses a general-purpose compensation model constructed from measured calibration data collected on a representative device. Without changing $K_p$ or $K_d$, it can reduce the effect of transmission friction, helping to reduce tracking error and improve control smoothness.

Friction characteristics can vary normally between devices because of assembly, break-in condition, and operating conditions. The degree of improvement provided by the built-in compensation may therefore differ between devices. Compare performance with compensation enabled and disabled on the actual device under the intended operating conditions, and select the setting that best meets the application requirements.

Users may enable the built-in friction compensation directly, or disable it and apply device-specific compensation through feedforward torque. If built-in friction compensation and custom feedforward torque are used together, avoid compensating for the same friction component twice.

## 3. Precautions

The currently released MIT control mode is still in beta and has the following known limitations and operating constraints:

### 3.1 Avoid Flexing Coupled Joints with External Force

Do not use an external force to flex the coupled THUMB_J3 / THUMB_J4 joints or the coupled J2 (PIP_FLEX) / J3 (DIP_FLEX) joints of the other four fingers. Under these conditions, the opposing torque calculated by the impedance controller may cause reverse tensioning of the tendon.

**Planned improvement:** This issue has been identified as a priority for future releases. We will continue to improve the control strategy and protection mechanisms for coupled joints. Until the relevant improvements are released, strictly follow the operating requirement above.

### 3.2 Avoid High-Stiffness Operation

Operating MIT mode under high-stiffness conditions is not currently recommended, as the system may exhibit non-converging oscillations.

### 3.3 Consider the Elasticity of the Thumb Sleeve

The thumb sleeve is elastic and therefore introduces additional external stiffness into the system, which may cause jitter. Depending on the application, control performance may be improved through feedforward torque compensation or variable impedance control by dynamically adjusting $K_p$ and $K_d$.

### 3.4 Avoid Large-Angle Step Commands

Do not send abrupt commands involving large changes in position or angle directly to a joint. An instantaneous change in the target position creates a large position error and may cause a torque step, impact, or oscillation. For large-range motion, use the upper-level controller to plan a trajectory, interpolate target positions smoothly, and apply appropriate velocity and acceleration limits to improve control smoothness.

### 3.5 Tune Parameters Gradually

Begin tuning in an unloaded condition and a safe posture, using low $K_p$, $K_d$, and feedforward torque values. Follow a single-joint, small-amplitude, low-speed approach. Increase $K_p$ gradually while adjusting $K_d$ based on oscillation and overshoot. Make only small changes to a limited number of parameters at a time. Verify stable single-joint response before testing coupled joints or coordinated multi-joint motion.

For J0 (MCP_ABD) and J1 (MCP_FLEX) of the index through little fingers, coordinate the control gains and avoid large differences between them. Tune each joint independently with small-amplitude motions before testing combined motion, and reduce the gains as appropriate if significant interaction is observed.

For the thumb, tune J1 (CMC_ROT) first, then tune J0 (CMC_ABD) using the typical J1 parameters and operating range. When J1 is heavily loaded, use more conservative control gains and motion limits for J0. Complete a combined J0/J1 motion test before finalizing the parameters.

When tuning coupled joints, monitor the associated joint motion, relevant motor currents, and tendon condition even if only one joint is being excited. If sustained oscillation, significant impact, or abnormal heating occurs, immediately reduce the gains or stop control.

## 4. Fingertip Force Estimation

The SDK currently provides a beta fingertip-force estimation feature. Based on the device operating state and an internal model, it can continuously output a three-dimensional force estimate for each finger in its fingertip-local coordinate frame without requiring additional force sensors.

At this stage, the feature is primarily intended to characterize fingertip force direction, relative magnitude, and trends over time. On the same device and under similar postures and operating conditions, the estimates can be used for contact-state detection, grasp monitoring, and control-strategy development.

Device-specific characteristics, assembly conditions, transmission friction, and dynamic motion may cause differences between the estimates and readings from a calibrated external force sensor. For precise force measurement or safety-critical decisions, calibrate the feature for the specific device and application and cross-check the results with an external force sensor. Do not use the estimated force as the sole basis for such decisions.

**Planned improvement:** We will continue to improve fingertip-force estimation performance, with a focus on estimation accuracy, consistency between devices, and stability across different postures and dynamic operating conditions, further expanding the range of supported applications.
