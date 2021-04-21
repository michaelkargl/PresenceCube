# Zube App

A multipurpose, edge connected cube

## Brief

The functionality of the cube is intended to be a form of information cube. It is edge-connected and capable of receiving commands via MQTT. These commands can span from simple "display text x" to "switch into mode y"

## Setup

### Cloning

The project uses submodules, so you will have to clone recursively
```
git clone --recurse-submodules --remote-submodules <url>
```

### Development Environment

The project uses [PlatformIO](https://platformio.org/) so you will have to install the VSCode plugins. After opening the project, VSCode should offer you to install all recommended plugins. If it does not, please refer to the PlatformIO setup documentation for up-to-date install instructions.
```

### User configuration

Some libraries require additional configuration. Most of it will be handled through scripts placed within the [scripts directory](./scripts). If need be, this section here will be updated accordingly.

1. Upload filesystem with fonts
   * To display text onto the display, fonts need to be available to the device. These are stored on its filesystem. This step needs to be done only once.
   * First `Build Filesystem Image` using the platformio sidebar
   * Then `Upload Filesystem image`

### Development configuration

Sometimes, new libraries add new configuration values. This means that you will have to reconfigure your application. Tasks that can be automated, should be automated to increase CI/CD capability in the future.

```
# Executes basic setup tasks
./Setup-Dependencies
```

Configuration is done using Platform IO's menuconfig command. Use the IDE or the pio command
`pio run -t menuconfig`

In some cases the arrow keys aren't working. Use alternative `J`,`K` bindings
![Configuration settings](./images/configurations.jpg)

## Running

All the steps necessary are performed using the PlatformIO's sidebar.
1. `Build`
1. `Flash`
1. `Monitor`

## What happens next

After booting:
1. A wireless connection, using the credentials entered within menuconfig, will be setup
1. A connection to the MQTT broker established

There are two topics used for MQTT messaging:
* Control: Listens for control-messages / subscribes to the broker
* Data:    Outgoing messages are sent on this channel

You can use the public mqtt-broker for testing purposes
* https://www.emqx.io/mqtt/public-mqtt5-broker