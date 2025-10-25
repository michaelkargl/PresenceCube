#if defined(PLATFORM_NATIVE)
    #include "main_native.h"
#elif defined(PLATFORM_ESPRESSIF32)
    // Forward declaration - implementation in presence-cube.esp.main
    extern int app_main();
#endif  