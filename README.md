# Zube App

A multipurpose, edge connected cube

> ## Note from the future
> 
> This README is wildly out-of-date and in dire need of refactoring. Over the course of the current milestone (%"🧪 Improve testability and cross platform support") this will be gradually change => #108

* [Testing](./test)
* Library Management
  * [Libraries](./lib)
  * [Components](./components)
* [Electrical Circuitry](./circuits/)
* [Configuration](./config)

<!-- old readme below -->

## Brief

The functionality of the cube is intended to be a form of information cube. It is edge-connected and capable of receiving commands via MQTT. These commands can span from simple "display text x" to "switch into mode y"

## Setup

### Cloning

1. The project uses submodules, so you will have to clone recursively
   ```powershell
   git clone --recurse-submodules --remote-submodules <url>
   ```
1. If already cloned
   ```powershell
   git submodule init
   git submodule update --recursive
   ```

### Development Environment

The project uses [PlatformIO](https://platformio.org/) so you will have to install the VSCode plugins. After opening the project, VSCode should offer you to install all recommended plugins. If it does not, please refer to the PlatformIO setup documentation for up-to-date install instructions.


### Filesystem with fonts

To display text onto the display, fonts need to be available to the device. These are stored on its filesystem. This step needs to be done only once with the device plugged in.

Build and upload filesystem image
1. Open PlatformIO CLI
1. ```powershell
   platformio run --target buildfs --environment esp32
   platformio run --target uploadfs --environment esp32
   ```

### Configuration

## Usage configuration

Configuration is done using Platform IO's menuconfig command. Use the IDE or the pio command
`platformio run --target menuconfig`

> In some cases the arrow keys aren't working. Use alternative `J`,`K` bindings
![Configuration settings](./images/configurations.jpg)

## **Development** configuration

Sometimes, libraries include custom configuration values. Unless these libraries are ESP-libraries, this means that you will have to integrate these values into your projects menuconfig. This can be done using a `powershell` script
```powershell
./scripts/Setup-Dependencies.ps1
```


## Running

All the steps necessary are performed using the PlatformIO's sidebar or CLI

```powershell
platformio run --target upload --target monitor --environment esp32
```

## What happens next

After booting:
1. A wireless connection is established using the credentials entered via menuconfig. The IP address is printed to console
   ```txt
   ␛[0;32mI (5135) wifi: WIFI station connected!␛[0m
   W (5175) wifi:<ba-add>idx:0 (ifx:0, 08:ec:f5:90:53:40), tid:0, ssn:2, winSize:64
   I (5195) wifi:AP's beacon interval = 102400 us, DTIM period = 1
   ␛[0;32mI (5775) esp_netif_handlers: sta ip: 192.168.34.235, mask: 255.255.252.0, gw: 192.168.32.1␛[0m
   ␛[0;32mI (5775) wifi: Received station address.␛[0m
   ␛[0;32mI (5775) wifi: IP:192.168.34.235␛[0m
   ␛[0;32mI (5775) wifi: Unregistering wifi handlers␛[0m
   ␛[0;32mI (5785) main: Setting up web server...␛[0m
   ```

## Manual test

The most direct way of testing the devices functionality are to use the provided scripts

```bash
./scripts/set-hud.sh --ip <cube-ip>
./scripts/set-color.sh --ip <cube-ip>
```

## Tooling

- Basic Multimeter
- Dupont Crocodile Clamps for simpler troubleshooting
- Preciva PR-3254 Dupont Ratcheting Crimper Plier for both Dupont- and JST cables

## Troubleshooting

Here are a few common errors maintainers have been facing :)

1. [🐞 Issue: After freshly cloning the project, I am unable to build / flash / upload it to the device](#83)
1. [🐞 Issue: Unable to upload / flash to the device: [upload] Error 134 no device found](#84)
1. [🐞 Bug: PlatformIO headers are not detected](#75)