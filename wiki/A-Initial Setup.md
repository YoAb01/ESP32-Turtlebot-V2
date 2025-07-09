# 📁 A - Initial Setup

### **Introduction**
In this session, we will be describing the initial setup process for the ESP32 Turtlebot V2 project.
The goal is to ensure a clean, reproducible and extensible development environment, covering tool installation, dependencies, environment configuration, and project directory structure.

---

### **Goals**
- Setup **ESP-IDF environment**
- Verify toolchain installation
- Create base directory structure using ESP-IDF principles

---

## 1. Environment Setup
In this setup section, we recommend having a setup:
- Running Ubuntu 22.04 LTS or higher (WSL2 or native)
- Zsh shell

## 2. Toolchain Installation
1. **Installation Prerequisites**
- First update your system:
```
sudo apt update
sudo apt upgrade -y 
```
- Next install essential tools:
```
sudo apt install -y git wget flex bison gperf python3 python3-pip cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
```

2. **Clone ESP-IDF** 
- Navigate to home directory, create the ESP directory and clone the ESP-IDF
```
mkdir -p ~/esp/v5.4
cd ~/esp/v5.4
git clone -b v5.4 --recursive https://github.com/espressif/esp-idf.git
```
3. **Install ESP-IDF Tools**
- Run the ESP-IDF install script
```
cd ~/esp/v5.4/esp-idf
./install.sh
```
4. **Set Up Environment Variables**
- Each time you open a new terminal, run:
```
source ~/esp/v5.4/esp-idf/export.sh
```
- Or add it to your **.zshrc**
```
# In ~/.zshrc
source ~/esp/v5.4/esp-idf/export.sh
# Source .zshrc
source ~/.zshrc
```
5. **Verify Installation**
```
idf.py --version
# Should give ESP-IDF v5.x-x
```

| Tool             | Version        | Notes                             |
|------------------|----------------|-----------------------------------|
| ESP-IDF          | v5.4           | Installed at `~/esp/v5.4/esp-idf` |
| Python           | >= 3.8         | Managed by ESP-IDF environment    |
| CMake + Ninja    | As required    | Installed via ESP-IDF scripts     |


### 3. First setup
The first setup focus on generating the structure of our ESP-IDF project.
In a new terminal, with sourced ESP-IDF:
```
idf.py create-project ESP32-Turtlebot
```
This will generate a minimal CMake project with a main.c file to start with. The next step is to implement basic blinking LED using FreeRTOS ESP-IDF concepts.

