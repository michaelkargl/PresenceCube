# Components Folder

PlatformIO supports the use of ESP-IDF components in their projects:
* https://docs.platformio.org/en/latest/frameworks/espidf.html#esp-idf-components

This has caveats:

1. PlatformIO only searches first-level children of `/components` folder for `Kconfig` files
   * This means that we'll have to tinker if we wanna have configuration sections in our libraries

## Submodule workaround

Untested yet - as long as git is configured to support symlinks - it should workd to create a symlink from `./lib/lib1` to `./components/lib1`

## Kconfig workaround

1. in the `[platform.ini](../platformio.ini)`, mark this `/components` directory as an `lib_extra_dirs`
   * This was a PlatformIO maintainer recommendation
1. create library folders within the `/components` containing `library.json` files pointing to the components files.

For example:
   
```txt
-components
   - hagl
      - Kconfig
      - src
      - include
   - hagl_lib
      - library.json
```

```json
// rough library.json
{
   "name": "hagl_lib",
   "srcDir": "../hagl/src",
   "build": {
      "includeDir": "../hagl/include" 
   }
}
```