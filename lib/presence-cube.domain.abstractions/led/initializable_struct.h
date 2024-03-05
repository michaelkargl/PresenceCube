#pragma once

/**
 * @brief Since structs are value types, it is impossible
 *        to know when a certain struct is ready for use
 *        or still under construction by some builder for
 *        instance. This flags intent is to mitigate these
 *        false-positive states and provides oportunities
 *        for more fine-grained error checking.
 * @todo // TODO #119: move to global helper in case this proves useful
 */
#define INITIALIZABLE_STRUCT bool is_initialized;