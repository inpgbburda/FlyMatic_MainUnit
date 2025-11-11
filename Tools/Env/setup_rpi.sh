#!/usr/bin/env bash
# ==========================================================
# Setup script for Raspberry Pi development environment
# This installs tools needed to build and run project
# directly on Raspberry Pi OS (Linux).
# ==========================================================

set -e

echo "🚩 1. Check for RT patched kernel"
if uname -a | grep -qE "PREEMPT_RT|-rt"; then
    echo "✅ Real-Time (RT) patched kernel detected."
else
    echo "❌ No RT patched kernel detected!"
    echo "This script requires a Real-Time Linux kernel (PREEMPT_RT)."
    exit 1
fi

echo "🚩 2. Updating package list..."
sudo apt update && sudo apt upgrade -y

echo "🚩 3. Installing basic build tools..."
sudo apt install -y build-essential cmake git pkg-config

echo "🚩 4. Installing common development libraries..."
sudo apt install -y libssl-dev libffi-dev

echo "🚩 5. Installing WiringPi (GPIO library)..."
# WiringPi is deprecated in official repos, but still usable
# Install from maintained fork
git clone https://github.com/WiringPi/WiringPi.git /tmp/WiringPi
cd /tmp/WiringPi
./build
cd ~
rm -rf /tmp/WiringPi

# ==========================================================
# Detect correct config.txt path (depends on Raspberry Pi OS)
# ==========================================================
if [ -f "/boot/firmware/config.txt" ]; then
    CONFIG_FILE="/boot/firmware/config.txt"
elif [ -f "/boot/config.txt" ]; then
    CONFIG_FILE="/boot/config.txt"
else
    echo "❌ Could not find config.txt (tried /boot/firmware/config.txt and /boot/config.txt)"
    exit 1
fi

echo "🔧 Using config file: $CONFIG_FILE"

NEED_REBOOT=0

# Configure I²C
echo "🚩 6. Install the I²C and enable if not already done"
sudo apt-get install -y libi2c-dev i2c-tools

if ! grep -q "^dtparam=i2c_arm=on" "$CONFIG_FILE"; then
    echo "🔧 Enabling I²C..."
    echo "dtparam=i2c_arm=on" | sudo tee -a "$CONFIG_FILE"
    echo "i2c-dev" | sudo tee -a /etc/modules > /dev/null
    echo "✅ I²C enabled."
    NEED_REBOOT=1
else
    echo "✅ I²C already enabled."
fi

# Enable SPI (with 2 chip selects)
if ! grep -q "^dtparam=spi=on" "$CONFIG_FILE"; then
    echo "🔧 Enabling SPI..."
    echo "dtparam=spi=on" | sudo tee -a "$CONFIG_FILE"
    echo "dtoverlay=spi0-2cs" | sudo tee -a "$CONFIG_FILE"
    echo "✅ SPI enabled."
    NEED_REBOOT=1
else
    echo "✅ SPI already enabled."
fi

# Ask for reboot at the end
if [ "$NEED_REBOOT" -eq 1 ]; then
    echo ""
    echo "ℹ️ A reboot is required for SPI/I²C changes to take effect."
    read -p "Reboot now? (y/N): " yn
    case $yn in
        [Yy]* ) sudo reboot;;
        * ) echo "Please reboot manually later. Once you reboot, you're good to go!";;
    esac
else
    echo "🎉 Everything is already set up. No reboot needed. You're good to go!"
fi
