#include "sed_auth.h"


static const uint64_t	AUTH_EVEN_TBL[] = {
		0x3ae1206f97c10bc8,
		0x2a9ab32bebf244c6,
		0x20a6f8b8df9adf0a,
		0xaf80ece52cfc1719,
		0xec2ee2f7414fd151,
		0xb055adfd73344a15,
		0xa63d2e3059001187,
		0x751bf623f42e0dde
};

static const uint64_t AUTH_ODD_TBL[] = {
		0x3e22b34f502e7fde,
		0x24656b981875ab1c,
		0xa17f3456df7bf8c3,
		0x6df72e1941aef698,
		0x72226f011e66ab94,
		0x3831a3c606296b42,
		0xfd7ff81881332c89,
		0x61a3f6474ff236c6
};

static const uint64_t MASK = 0xa79a63f585d37bf0;

static uint64_t rol8(uint64_t v);
static uint64_t rol8n(uint64_t v, uint16_t n);



static uint64_t rol8(uint64_t v)
{
	return ((v << 56) | (v >> 8)) & 0xffffffffffffffff;
}

static uint64_t rol8n(uint64_t v, uint16_t n)
{
	for(uint16_t i=0; i<n; ++i) v = rol8(v);
	return v;
}


uint64_t compute_auth(uint64_t challenge)
{
	uint16_t n = challenge & 7;
	uint64_t v = rol8n(challenge, n);
  uint64_t k;

	if( (v & 1) == ((0x78 >> n) & 1) )
  {
		k = AUTH_EVEN_TBL[n];
  }
	else
  {
		v = v ^ rol8(v);
		k = AUTH_ODD_TBL[n];
  }
	return v ^ (rol8(v) & MASK) ^ k;
}
