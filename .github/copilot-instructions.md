# PresenceCube - AI Coding Agent Instructions

## Project Overview
Network-connected IoT information cube with RGB LED lighting, built for **ESP32** (embedded) and **native** (desktop) platforms using C with PlatformIO.

The project follows a **.NET-style solution structure** where each platform has its own "solution" folder containing related sub-projects.

### Architecture Pattern: Hexagonal/Clean Architecture
- **Domain Layer** (`presence-cube.domain`): Platform-agnostic business logic
- **Use Cases** (`presence-cube.usecases`): Application logic (e.g., `get_led`, `set_led`)
- **Infrastructure**: Platform-specific implementations organized as .NET-style projects:
  - `presence-cube.esp/infrastructure/` - ESP32 infrastructure
  - `presence-cube.esp/logging/` - ESP32 logging implementation
  - `presence-cube.esp/main/` - ESP32 entry point
  - `presence-cube.native/infrastructure/` - Native infrastructure
  - `presence-cube.native/logging/` - Native logging implementation
  - `presence-cube.native/main/` - Native entry point
- **Abstractions** (`*.repositories.abstractions`): Interface definitions with `.iface.h` suffix

**Key Principle**: Dependencies point inward. Domain knows nothing about platforms. Platforms implement domain interfaces.

## Naming Conventions (Critical)

### Public API Naming
All public functions/variables MUST use `module__identifier` format:
```c
// ✅ Correct
int math_util__add(int left, int right);
const rgb_led_diode_bag_t* get_led_repository__get_leds();

// ❌ Wrong - no context
int add(int left, int right);
```

### Private/Static Naming
Static functions have NO prefix (anything without `module__` is private):
```c
// ✅ Correct
static bool initialized = false;
static void initialize_hardware();

// ❌ Wrong - reserved identifiers
static bool _initialized;  // Leading underscore is reserved by C standard
static void __init();      // Double underscore is reserved
```

### Interface Headers
Headers requiring platform-specific implementations use `.iface.h` suffix:
- `logger.iface.h` - Implemented by `logging.esp` (ESP32) or `presence-cube.native/logging` (native)
- Tells developers "this needs a concrete implementation before use"

### File Naming
- Use `snake_case` for all files
- Test files: `test_<module>_<type>.c` (e.g., `test_led_store_unit.c`)

## Library Management

### PlatformIO Configuration
Library dependency finding is **DISABLED** (`lib_ldf_mode = off`) to enforce explicit architecture:
- Prevents accidental cross-platform pollution
- Forces conscious dependency decisions
- All dependencies declared in `library.json` files
- **Critical**: With `lib_ldf_mode = off`, dependencies in `library.json` are IGNORED
- Libraries are found ONLY via `lib_extra_dirs` in `platformio.ini`
- Both the parent folder AND all sub-project folders must be explicitly listed in `lib_extra_dirs`

### Library Structure
Each library has `library.json` defining:
```json
{
  "name": "presence-cube.domain",
  "build": {
    "srcDir": "./",
    "includeDir": "./"
  },
  "dependencies": {
    "presence-cube.shared": "*",
    "cJSON": "*"
  }
}
```

### Platform-Specific Libraries
- **Any platform**: `/lib/presence-cube.domain`, `/lib/presence-cube.usecases`
- **ESP32 only**: `/lib/platforms/espressif32`, `/lib/presence-cube.esp/`
  - `infrastructure/` - Hardware abstractions, repositories
  - `logging/` - ESP32 logging implementation
  - `main/` - Application entry point
- **Native only**: `/lib/presence-cube.native/`
  - `infrastructure/` - Native implementations, web server
  - `logging/` - Console logging implementation
  - `main/` - Application entry point
  - `config/` - Native configuration
- **Components**: `/components` (ESP-IDF components with Kconfig)

## Development Workflows

### Building
```powershell
# Build specific environment
./build_env.ps1 native-dev
./build_env.ps1 espressif32-dev

# Alternative: Use PlatformIO wrapper
./invoke-pio.ps1 run --environment native-dev
```

### Testing
Tests organized by platform compatibility:
- `test/any/` - Run on all platforms (use cases, domain logic)
- `test/native/` - Native-specific (infrastructure tests)
- `test/embedded/` - ESP32-specific

```powershell
# Run tests (prefer this)
./invoke-tests.ps1 -Environment native-test

# Legacy wrapper (calls invoke-tests)
./test_native.ps1
```

**Test Pattern**: Use `fff` (Fake Function Framework) for mocking:
```c
DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(const rgb_led_diode_bag_t*, fff_get_leds);

void setUp(void) {
    // Inject fakes into function pointers
    get_led_query_handler__led_repository__get_leds = fff_get_leds;
}
```

### Configuration
Project uses ESP-IDF's Kconfig for runtime configuration:
```powershell
# Open menuconfig for specific environment
./Edit-ProjectConfig.ps1 -Environment espressif32-dev
./Edit-ProjectConfig.ps1 -Environment native-dev
```

Access in code via `CONFIG_*` defines:
```c
#define WIFI_SSID CONFIG_WIFI_SSID
#if CONFIG_WEBSERVER_ENABLED
    web_host_start(CONFIG_WEBSERVER_PORTS);
#endif
```

### ESP32-Specific: Flash Filesystem
Fonts stored in SPIFFS partition:
```powershell
./invoke-pio.ps1 run --target buildfs --environment espressif32-dev
./invoke-pio.ps1 run --target uploadfs --environment espressif32-dev
```

### Flashing & Monitoring
```powershell
./invoke-pio.ps1 run --target upload --target monitor --environment espressif32-dev
```

## Error Handling
Uses `CException` library for exception handling in C:
```c
#include "exception_handling.h"

Try {
    rgb_led_diode_t* led = get_led_repository__get_led(id);
} Catch(e) {
    LOG_ERROR("Failed: %i", e);
}
```

## Logging Convention
Platform-agnostic logging via `logger.iface.h`:
```c
#include "logger.iface.h"

LOG_INFORMATION("Server starting on port %s", port);
LOG_DEBUG("LED states: R=%u G=%u B=%u", r, g, b);
LOG_WARNING("Retrying connection");
LOG_ERROR("Initialization failed: %i", error);
```

## Multi-Platform Strategy

**Never use `#ifdef PLATFORM` in shared code**. Instead:
1. Define interface in domain layer (`.iface.h`)
2. Implement in platform-specific library
3. Let PlatformIO's `lib_extra_dirs` resolve at build time

Example: `logger.iface.h` → implemented by `presence-cube.esp/logging/logger.c` OR `presence-cube.native/logging/logger.c`

### .NET-Style Solution Organization
The project follows a .NET solution pattern where each platform is a "solution" folder:

```
lib/
├── presence-cube.esp/           # ESP32 Solution
│   ├── infrastructure/          # ESP32.Infrastructure
│   │   ├── led_store/           # ESP32.Infrastructure.LedStore
│   │   ├── models/              # ESP32.Infrastructure.Models
│   │   ├── presence-cube.repositories.esp/  # ESP32.Repositories
│   │   └── rgb_ledc/            # ESP32.Infrastructure.RgbLedc
│   ├── logging/                 # ESP32.Logging
│   ├── main/                    # ESP32.Main (entry point)
│   └── library.json
├── presence-cube.native/        # Native Solution
│   ├── config/                  # Native.Config
│   ├── infrastructure/          # Native.Infrastructure
│   ├── logging/                 # Native.Logging
│   ├── main/                    # Native.Main (entry point)
│   └── library.json
├── presence-cube.domain/        # Shared Domain
├── presence-cube.usecases/      # Shared Use Cases
└── presence-cube.shared/        # Shared Utilities
```

**Naming Convention**: Libraries follow `presence-cube.<platform>.<component>.<subcomponent>` pattern:
- ESP32: `presence-cube.esp.infrastructure`, `presence-cube.esp.logging`, `presence-cube.esp.main`
- ESP32 Infrastructure sub-libraries: `presence-cube.esp.infrastructure.led_store`, `presence-cube.esp.infrastructure.models`, etc.
- Native: `presence-cube.native.infrastructure`, `presence-cube.native.logging`, `presence-cube.native.main`

**Critical PlatformIO Requirement**: ALL library folders (parent AND sub-projects) must be explicitly listed in `lib_extra_dirs` in `platformio.ini` because `lib_ldf_mode = off` disables automatic library scanning.

## Common Pitfalls
- ❌ Forgetting `module__` prefix on public functions
- ❌ Using leading underscore (`_variable`) - reserved by C standard
- ❌ Enabling `lib_ldf_mode` - breaks multi-platform architecture
- ❌ Putting platform-specific code in domain/usecases layers
- ❌ Missing `library.json` dependencies - causes cryptic linker errors

## CI/CD
GitHub Actions workflow (`.github/workflows/ci.yml`):
- Builds native + ESP32 environments
- Runs native tests
- PowerShell script tests
- Flawfinder security analysis

## Repository Setup
**Critical**: Clone with `--recursive` for submodules:
```bash
git clone --recursive <url>
# Or if already cloned:
git submodule update --init --recursive
```
