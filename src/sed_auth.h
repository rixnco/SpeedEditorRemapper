#ifndef __SED_AUTH_H__
#define __SED_AUTH_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct __attribute__((packed)) {
  uint8_t  id;
  union {
    uint64_t value;
    uint8_t  bytes[8];
    uint16_t words[4];
    uint32_t dwords[2];
  };
} challenge_t;

uint64_t compute_auth(uint64_t challenge);

#ifdef __cplusplus
}
#endif
#endif

