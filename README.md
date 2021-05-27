# fx3_ov5640

## How to use
- Install FX3_SDK_1.3.4_Linux or later, the workspace may include cyusb_linux_1.0.5, cyfx3sdk, arm-2013.11

- Create a setup_env.sh(below) under the root of the workspace and source it

  ```shell
  PWD=$(pwd)
  export FX3_SDK_ROOT=${PWD}
  export PATH=${PATH}:${PWD}/arm-2013.11/bin:${PWD}/cyfx3sdk/util/elf2img
  export FX3_INSTALL_PATH=${PWD}/cyfx3sdk
  export ARMGCC_INSTALL_PATH=${PWD}/arm-2013.11
  export ARMGCC_VERSION=4.8.1
  ```

- git clone this repo to cyfx3sdk/firmware

- cd into "fx3_ov5640", run make, hopefully the project would be built

- then cd into "tools", build the image and program it(need to build the elf2img separately)

  ```shell
  ./build_image.sh ../fx3_ov5640.elf fx3_ov5640.img 
  ./flashprog.sh fx3_ov5640.img RAM  # program to RAM
  ```

  

