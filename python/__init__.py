"""
Rysen ApexHand SDK Python Package

Copyright (c) 2024-2026, Rysen Robotics (Shenzhen) Co. Ltd.
All rights reserved.

Use of this source code is governed by a BSD 3-Clause license that can be
found in the LICENSE file.
"""

from .rysen_apexhand_sdk import _sdk as _native_sdk

__version__ = _native_sdk.__version__
