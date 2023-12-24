	# There is a mutual authentication mechanism where the software and the
	# keyboard authenticate to each other ... without that, the keyboard
	# doesn't send REPORTs :/
	#
	# We don't care about authenticating the keyboard so the implementation
	# here is minimal, just enough to make the keyboard start working


def rol8(v):
	return ((v << 56) | (v >> 8)) & 0xffffffffffffffff


def rol8n(v, n):
	for i in range(n):
		v = rol8(v)
	return v


def bmd_kbd_auth(challenge):

	AUTH_EVEN_TBL = [
		0x3ae1206f97c10bc8,
		0x2a9ab32bebf244c6,
		0x20a6f8b8df9adf0a,
		0xaf80ece52cfc1719,
		0xec2ee2f7414fd151,
		0xb055adfd73344a15,
		0xa63d2e3059001187,
		0x751bf623f42e0dde,
	]
	AUTH_ODD_TBL = [
		0x3e22b34f502e7fde,
		0x24656b981875ab1c,
		0xa17f3456df7bf8c3,
		0x6df72e1941aef698,
		0x72226f011e66ab94,
		0x3831a3c606296b42,
		0xfd7ff81881332c89,
		0x61a3f6474ff236c6,
	]
	MASK = 0xa79a63f585d37bf0

	n = challenge & 7
	v = rol8n(challenge, n)

	if (v & 1) == ((0x78 >> n) & 1):
		k = AUTH_EVEN_TBL[n]
	else:
		v = v ^ rol8(v)
		k = AUTH_ODD_TBL[n]

	return v ^ (rol8(v) & MASK) ^ k


while(1):
    print('000000000000000000')
    challenge = input()
    print('010000000000000000')
    challenge = bytes.fromhex(challenge)
    id = challenge[0:1]
    challenge = challenge[1:]
    challenge = int.from_bytes(challenge, 'little')
    response= bmd_kbd_auth(challenge)
    response=response.to_bytes(8, 'little')
    response=b'\03'+response
    print(response.hex().upper())

