#if defined(PLATFORM_NATIVE)
    #include "main_native.h"
#elif defined(PLATFORM_ESPRESSIF32)
    #include "main_esp32.h"
#endif  