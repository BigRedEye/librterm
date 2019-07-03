static_assert(hash("Test string") == 0xE2A57B7B7B739001);
static_assert(hash("") == 0xA32F057AD860B25E);
static_assert(hash("\0") == 0x4BAF56F243B7BC09);
static_assert(hash("\xff") == 0xD94CD077EB8F2744);
static_assert(hash("123412341234512") == 0x3F8BF1D5B0EAA769);
static_assert(hash("static_assert driven development") == 0x1A5F7F7244618BCA);
static_assert(
    hash("static_assert(hash(\"static_assert driven development\") == 0x1A5F7F7244618BCA);") == 0x80E2E3FB23C08B14);
static_assert(hash("") == 0xa32f057ad860b25e);
static_assert(hash("\x57") == 0x1bbc03230ae62439);
static_assert(hash("\x30\x1") == 0x82037a1b102149af);
static_assert(hash("\x70\x46\x17") == 0xe3ca6ae835058b1);
static_assert(hash("\x16\x57\x78\x6c") == 0x5378fff78aa9e9a4);
static_assert(hash("\x3\x78\x1\x5b\x7d") == 0x50c4433de47daaec);
static_assert(hash("\x66\x1\x66\x46\x3d\x61") == 0x2a9fa45639ff22a3);
static_assert(hash("\x44\x30") == 0xfa39f5cb4ab9c648);
static_assert(hash("\x44\x12\xd\x7d") == 0xb80e37fd53ffe935);
static_assert(hash("\x20\x4f\x29\x31\x7c\x6b\x6b\x33") == 0xecf61acb45f2dc22);
static_assert(hash("\x39\x76\x35\x22\x2a\x14\x73\x61\x78") == 0x2a8e74c570709aa5);
static_assert(hash("\xc\x1d\x73\x7b\x38\x7a\x5a\x3f\x1a\x2c") == 0x8303476334610dae);
static_assert(hash("\x49\x33\x45\x2d\x56\x37\x26\x59\x50\x3f\x1e") == 0xaf7dcc65a8edf997);
static_assert(hash("\x57\x6c\x63\x4b\x2f\x52\x70\x3a\x30\x6a\x5d\x15") == 0xea45ac0969787543);
static_assert(hash("\x3\x17\x7c\x77\x5b") == 0xc4e8f425b0b80468);
static_assert(hash("\x43\x1e\x29\x2a\x30\x39\x57") == 0x95ba70b7d4fd38b0);
static_assert(hash("\x29\x71\x64\x66\x31\x35\x62\x1\x23\x1e\x49\x7d\x2b\x64") == 0xca09410cc2f2c248);
static_assert(hash("\x2\x69\x76\x79\x16\x7b\x73\x67\xf\x2d\x1\x49\x37\x5e\x26") == 0x86824112029f73a0);
static_assert(hash("\x4c\x25\x29\x7f\x6e\x6e\x7a\x73\x4c\x20\x70\x6\x3c\x4d\x66\xb") == 0x16e51033d26a2e27);
static_assert(hash("\x49\x2b\x59\x2b\x60\x74\x4b\x23\x59\x59\x21\x3c\x58\x43\x53\x7b\xe") == 0x79169025d3d86a16);
static_assert(hash("\x50\x13\x47\x64\x75\x9\x41\x7f\xb\x47\x49\x14\x1c\x10\x67\x54\x62\x7") == 0x81b81e85f58f9039);
static_assert(hash("\x15\x3a\x73\x56\x9\x55\x2\x5\x31\x14\x4c\x76\x18\x3c\x71\x6c\x77\x7f\x3c") == 0x1e7d268d437f657b);
static_assert(hash("\x76\x7a\x2d\x71\x24\x29\x5f\x1\x61\x53\x18\x3c\x7b\x27\x7a\x6c\x62") == 0xc2abdbbdc3101b70);
static_assert(hash("\x33\x0") == 0x62e845717eada252);
static_assert(
    hash("\x47\x36\x1d\x3e\x8\x6f\xc\x11\x2\x4f\x13\x76\x37\x15\x22\x6a\x39\x27\x77\x19\x43") == 0x1e706b611a41f5a3);
static_assert(hash("\x4d\x49\x16\x63\x65\x3\x44\x11\x50\x9\x5b\x14\x5f\x1\x10\x50") == 0x514a94e2c616706);
static_assert(hash("\xd\x55\x59\x6e\x30") == 0x9e3e4014ca6d789c);
static_assert(
    hash("\x18\x71\x30\x2c\x43\x5b\x46\x11\x37\x5e\x53\x5f\x1f\x68\x34\x18\x4c\x51\x3\xd\x6a\x5b\x25") ==
    0xc4168e89f3c922e5);
static_assert(
    hash("\x37\x6e\x18\x55\x26\x3a\x1\x10\x62\x25\x61\x1e\x46\x72\x36\x74\x58\x54\x3b\x53\x4d\x79\x5f\xd") ==
    0x89334c3ab79339fb);
static_assert(
    hash("\x44\x29\xc\x22\x34\x76\x73\x40\x14\x47\x26\x8\x20\x35\x37\x4b\x17\x5a\x11\x7\x66\xc\x70\xf\xe") ==
    0xcb6aa0f859e7d5b0);
static_assert(
    hash("\x56\x41\x56\x7\x5f\x27\x7a\x78\x2\x2b\x61\x7d\x1a\x49\x2b\x60\x78\x2d\x34\x4\x16\x2e\x1c\x1f\x4d\x54") ==
    0xcb5f606fbc121ff3);
static_assert(
    hash(
        "\x2b\x40\x64\x59\x4c\x56\x4c\x54\x60\x66\x64\x5f\x24\x1a\x9\x15\x6\x14\x57\x61\x2e\x69\x2c\x2d\x65\x2b\x11") ==
    0x651ecc87a8f7a351);
static_assert(
    hash("\x6f\x34\x67\xf\x3\x48\x2e\x38\x3f\xe\x4d\x12\x1e\x6b\x17\x64\x5\x23\x4c\x60\x69\x4a\x46\x55\x3a\x6a\x3f"
         "\x6f") == 0xde2de2cb5d8697ef);
static_assert(
    hash("\x5e\x47\x2\xb\x59\x28\x15\x27\x55\x76\x24\x46\x4e\xe\x48\x7b\x3a\x3f\x43\x35\x0\x3\x2c\x1d\x19\xd\x3c\x68"
         "\x46") == 0x80189c521e4d3964);
static_assert(
    hash("\x7d\x4\x47\x72\x2\x1c\x4a\x62\x74\x40\x1b\x55\xc\xd\x15\x28\x28\x14\x19\x49\x70\x3d\x2\x19\x14\x33\x30\x7f"
         "\x11\x31") == 0xefcbce426606b8f3);
static_assert(
    hash("\x58\x6c\x4d\x6d\x1b\x55\x7f\x7b\x37\x17\x66\x54\x76\x30\x77\x61\x1d\x1f\x12\x6\x18\x14\x66\x4d\x3d\x39\x4f"
         "\x20\x7b\x4\x5") == 0x7b46731e17b0c648);
static_assert(
    hash("\x5\x7e\x6\x25\x56\x6\x19\x1c\x1d\x4\x8\xb\x5\x4\x37\x9\x7b\x17\x74\x23\x12\x30\x12\x2a\x4b\x3a\x39\x2\x7\x59"
         "\x4b\x37") == 0x4a61eec2bb3246b8);
static_assert(
    hash("\x6a\x52\x66\x3e\x79\x39\x8\x39\x4c\x43\x1\xd\x55\x2f\x3\x5e\x5\x11\x76\x5b\x5d\xc\x32\x4f\x1a\x49\x5e\x21"
         "\x3a\x1b\x2e\x75\x66") == 0x250d91368fbe8925);
static_assert(
    hash("\x69\x15\x4a\x4d\x7\x1f\x1b\x37\x32\x40\x1c\x6b\x4b\x29\x53\x73\x50\x6d\x1b\x35\x11\x59\x7e\x9\x37\x52\x4f"
         "\x43\x3f\x68\x77\x28\x5f\x73") == 0x2e8d004eb4ca5990);
static_assert(hash("\x66\x1c\x4\xc\x77\x1f\x54\x7d") == 0x162cf9ccd26f30bf);
static_assert(
    hash("\x45\x4\x12\x2f\x5a\x10\x9\x72\x59\x23\x7e\x35\x28\x23\x16\x72\x63\x15\x21\x57\x5c\x33\x7d\x78\x17\x2d") ==
    0x7fa35fb790409b67);
static_assert(
    hash("\x31\x32\x6\x36\x27\x7b\x2\x4e\x1b\x45\x68\x7e\xc\x63\x46\x20\x12\x6e\x62\x11\x73\x6f\x6\x2c\x22\x20\x44\x1d"
         "\x4f\x66\x6e\x16\x8\x73\x57\x29") == 0xccd7e69be9d457d3);
static_assert(
    hash("\x7c\x48\x3c\x3\x2\x2c\x34\x70\x3d\x39\x39\x77\x7\x58\x63\x2c\x7d\x38\x33\x7b\x57\x29\x52\x2d\x69\x49\x79\x76"
         "\x9\xe\xb\x50\x25\x9\x4\x47\x4d") == 0x7faee240054dfdd1);
static_assert(
    hash("\x6e\x12\x16\x27\x55\x75\x6d\x1e\x65\x27\x52\x7b\x6f\x3f\x4\x16\x68\x2f\x1c\x2e\x71\x2f\x66\x34\x53\x26\xc"
         "\x10\x49\x23\x7c\x1d\x59\x2c\x6a\x72\x41\x7b") == 0x47df0e128d607dc5);
static_assert(
    hash("\x8\x28\x3d\x2f\x23\x28\x49\x42\x8\x2a\x45\x3a\x9\x23\x77\x18\x2e\x15\x3a\x32\x7e\x7a\x7e\x6c\x1e\x10\x4a\x4c"
         "\x49\x2d\x13\x72\x47\x3f\x54\x14\x2a\x5\x5c") == 0xe60bdf735ebd43bc);
static_assert(hash("\x2c\x59\x1f\x25\x2\x2e") == 0x18832bc333b0ff55);
static_assert(
    hash("\x39\x55\x77\x1e\x49\x34\x2e\x40\x33\xc\x51\x50\xf\x40\x4d\x5f\x6a\x2\x17\x13\x52\x76\x3\x25\x4f\x17") ==
    0xcde3ab2d2c7a443e);
static_assert(hash("\x5f\x77\x60\x45\x41\x5") == 0x9437707b65a4dacf);
static_assert(
    hash("\x7f\x50\x6f\x7f\x31\x46\x43\x53\x34\x9\x7b\x1f\x14\x41\x18\x77\x14\x52\x3c\x4f\x5\x1\xe\x64\x37\x63\x30\x62"
         "\x69\x5b\x5\x7d\x19\x3e\x7e\x6\x5b\x3a\x30\x7c\x2d") == 0x842a25caad908390);
static_assert(
    hash("\x20\x5b\x30\x2b\x47\x37\x6e\x67\xf\x6e\x18\x1c\x8\xb\x3c\x6\x5b\x61\x7f\x20\x0\x12\x64\x42\x49\x32\x3e\x9"
         "\x34") == 0x16fe5433b4b2baae);
static_assert(hash("\x32\x26\x25\x5e\x79\x70\x2a\x7c\x35\x42\x7b\x62") == 0x837824f4d1d83be0);
static_assert(
    hash("\x60\x0\x0\x68\x8\x1e\x37\x3b\x18\x47\x36\x65\x1c\x57\x5\x20\x29\x2b\x5c\x72\x55\x22\x2b\x46\x31\x41\x52\x5b"
         "\x5a\x65\x75\x13\x76\x67\x7f\x4a\x26\x4e\x18\x4d\x10\x1a\x6") == 0x71849719a1eed8b2);
static_assert(
    hash("\x32\x53\x1d\x6d\x44\x15\x35\x48\x22\x30\xe\x7f\x29\x5a\x75\x5e\x76\x4b\x27\x2d\x3a\x75\x49\x7e\x2a\x31\x34"
         "\xc\x62\x58\x28\xd\x34\x1") == 0xa84445b728c7831d);
static_assert(hash("\x79\x6b\x29\x34\x5\x12\x35\x5\x63") == 0xf9abaec915f6fb07);
static_assert(
    hash("\x4b\x18\x4e\x27\x5b\x15\x3\x6c\x57\x14\x6e\x7e\x67\x7a\x3b\x25\x31\x10\x73\x8\xb\x26\x70\x5a\x77\x49\x1\x4f"
         "\x30\x6a\x2c\x7d\x6d\x68\x74\x2d\x69\x2a\x69\x30\x64\x35\x5c\x16\x4f") == 0x60417552d275017f);
static_assert(
    hash("\x23\x3a\x7e\x2c\x35\x76\x15\x30\x3a\x19\xd\x31\x4a\x24\x5f\x6d\x9\x6f\x23\x2a\x4f\x55\x26\x4b\x42\xd\x1\x4a"
         "\x69\x4f\x1a\x4\x6d\x29\xf\x66\x8\x14\x22\x5c\x53\x15\x7\x34\x7e\x47") == 0x72d0d61a48b0c4d1);
static_assert(
    hash("\x35\x2e\x33\x38\x2a\x28\x5a\x3b\x4\x48\x51\x0\x4c\x5e\x3b\x6\x2e\x5d\x10\x4c\xd\x6e\x2\x6\x74\x5c\x3b\x67"
         "\x7a\x78\x16\x6c\x15\x44\x67\x58\x49\x8\x56\x6b\x49\x24\x14\x7f\x7a\x4a\x53") == 0x8edc5d6be6a826d9);
static_assert(
    hash(
        "\x60\x25\x16\x33\x71\x3f\x6\x50\x17\x4b\x55\x11\x5f\x2f\x25\x65\x25\x6d\x5a\x3d\x5e\x7b\x72\x3d\x16\x0\x73") ==
    0x9e6b9a5427af4ac2);
static_assert(
    hash("\x21\x39\x6f\x68\x44\x3d\x6a\x6c\x6a\x19\x58\x22\x14\x2b\x26\x5a\x38\x0\x28\x4c") == 0x6408e1ceff5916bd);
static_assert(hash("\x44\x3b\x43\x5c\x20\x47\x2a\x71\x7d\x1e\x62") == 0x54c693bf43118cf7);
static_assert(
    hash("\x68\x55\x6f\x2c\x6\x67\xc\x3b\x10\x35\x6\x56\x6a\x6\x70\x3\x60\x7a\x43\x8\x56\x35\x68\x60\x49\x54\x23\x11\x5"
         "\x35\x3d\x1f\x37\x1c\x4e\x19\x2") == 0x93534b76cac35848);
static_assert(
    hash("\x7a\x60\x29\x21\xf\x6f\x1c\x5d\x69\x8\x51\x3\x2c\x8\x3b\x30\x73\x63\x13\x4a\x5\x4d\x45\x4c\x77\x46\x3\x43"
         "\x43\x8\x4f\x74\x73\x2\x73\x34\x49\x16\x2b\x22\x6e\x21\x10\x42\x68\x60\x4f\x14\x53\x1c") ==
    0x50125eb44fef2eab);
static_assert(
    hash("\x2\x53\x24\x32\x33\x24\x34\x75\x6f\x2c\x3b\x36\x43\xe\x9\x62\x61\x1\xc\x2a\x2b\x3d\x1c\x6c\x3e\x68\x6\x16\x6"
         "\x49\x43\x11\x7c\x1e\x51\x2c\x31\x7f\x1c\x7e\x23\x43\x5\x7\x56\x3e\x35\x68\x39\x42\x5f") ==
    0xc2b7fbb6684d1341);
static_assert(
    hash("\x38\x66\x69\x18\x56\x56\x2e\x67\x7\x28\x2a\x4f\x15\x56\x5c\x1e\x40\x51\x46\x27\x55\x33\x3e\x60\x31\x6f\x60"
         "\x2d\x3c\x1f\x78\x11\x55\x5b\x73\x34\x37\x39\x1e\x4c\x57\x4c\xc\x1f\x70\x4d\x17\xf\xd\x4a\x72\x3a") ==
    0xc27dfa06ef7c9296);
static_assert(hash("\x70\x21\x27\x38\x3c\x4f\x2\x6") == 0xd9fb0bd4f88dcb86);
static_assert(
    hash("\x69\x3e\x77\x45\x3d\x38\x0\x2a\x6e\x1f\x78\x65\x5c\x51\x38\x17\x14\x2a\x16\x4e\x5d\x52\x57\x4e\x5f\x55\x16"
         "\x1d\x79\x40\x12\x7b\x3d\x69\x1a\x3\x3a\x25\x1b\x74\x3e\xf\x28\x21") == 0xd8c99e03d7ff09b9);
static_assert(
    hash("\x3\x29\x6c\x12\xc\x73\x7c\x2b\x72\x72\x52\x58\x13\x30\x4d\x69\x22\x5\x39\x6f\x7a\x76\x61\x23\xe\x3e\x42\xe"
         "\x57\x1\x4c\x72\x15\x6e\x16\xc\x75\x77\x76\x31\xf\x7\x11\x2\x32\x64\x35\x38\x71\x5a\x7c\x70\x70\x72") ==
    0x9bb51ada1a546fa7);
static_assert(
    hash(
        "\x59\x5b\x1c\x39\x19\x35\x7\x6f\x57\x8\x2c\x2f\x26\x4d\x79\x59\x73\x19\x7f\x7e\x6d\x47\x5d\x53\x68\x1a\x7c\x2d"
        "\x27\x6a\xd\x70\x43\x16\x2f\x50\x3d\x19\x1f\x75\x31\x41\x6a\x5d\x29\x57\x50\x5\x65\x72\x56\x0\x67\x20\x50") ==
    0x9dc1748f8b44746d);
static_assert(
    hash("\x20\x7\x67\x62\x3d\x35\x48\x33\xd\x63\x28\x5f\x79\x4c\x44\x5d\x4b\x4a\x40\x26\x52\x2a\x74\x47\xc\x54\x57\xf"
         "\x60\x2a\x40\x77\x3e\x76\x74\x5c\x4a\x21\x54\x59\x7c\x2d\x14\x24\x6d\xc\x16\x7\x4d\x6e\x2f\x10\x3b\x50\xf"
         "\x2d") == 0xa12aded22b7d0e3);
static_assert(hash("\x5\xf\x73") == 0xa618d8a8b3edff32);
static_assert(
    hash("\xb\x55\x37\x65\x2e\x12\x3d\x5\x63\x67\x1\x2b\x54\x76\x2f\x14\x6b\x76\xe\x36\x5d\x78\x4a\x17\x25\x76\x6\x4"
         "\x17\x2\x66\x38\x8\x17\x72\x50\x45") == 0xc136193f51b0c233);
static_assert(hash("\x2e") == 0x9e16ff749a0ed77e);
static_assert(hash("\xe\x71\x37\x54\x74\x71\x6\x3a\x11\x8\x4d\x7a\x44") == 0xdedb8248cf760ec6);
static_assert(
    hash("\x11\x2f\x46\x1f\x7b\x6f\xe\x79\x5b\x4d\x38\x56\x21\x74\x45\x3b\x6\x8\x10\x3a\x44\x2f\x54\x71\x67\x54\x5e\x34"
         "\x58\x26\x34\xf\x7c\x1b\xf\x5\x16\x4f\x13\x16\x2a\x56\x9\x7f\x2f\x45\x3b\x65\x1e\x25\x2f\x3a\x1c\x69\x1\x11"
         "\x3a\x37") == 0x1568d419a2d79915);
static_assert(hash("\x5a\xc\x25\x10\x50\x2c\x22\x5f\x78\x3b\x21") == 0x98a74fb386ca6f68);
static_assert(
    hash("\x5d\x6\x6c\x5\x55\x23\x1d\x6c\x1e\x39\x79\x6f\x4c\x4f\x0\x46\x36\x26\x7a\x1\x5b\x69\x35\x7f\x34\x77\x25\x20"
         "\x69\x53\x2e\x54\x25\x4c\x73\x47\x12\x67\x20\x75\x49\x1b\x4a\x77\x4b\x1c\x21") == 0x804a47cb2504bb5c);
static_assert(
    hash("\x62\x56\x67\x5\x7c\x5d\x66\x7\x74\x3b\x59\x54\x4b\x60\x31\x50\x66\x33\x26\x4d\x12\xe\x2a\x1c\x50\x70\x5f\x1b"
         "\x1e\x52\x55\x3a\x4\x63\x35\x5\x53\x39\x68\x3\x5b\x6\x57\x4c\x1a\x1e\x40\x42\x4f\x1c\x70\x3d\x48\x6\x1f\x31"
         "\x31\x30\x5\x7c") == 0x8172ff709dab2235);
static_assert(hash("\x7d\x29\x19\x20\x68") == 0x73d248a29ea561b9);
static_assert(
    hash(
        "\x56\x5f\x16\x10\x0\x8\x6f\x25\x23\x64\x4\xc\x2a\x0\x15\x53\x68\x3b\x36\x61\x2b\x73\x76\x1e\x60\x1a\x3\x24\x29"
        "\x2b\x12\x6a\x65\x65\x17\x7e\x5c\x6a\x39\x66\x26\x14\x46\x42\x5d\x7c\x60\x1d\x14\x5a\x3\x76\x53\x18\x33") ==
    0x5c3bf3a43d6bc8cc);
static_assert(
    hash("\x59\x3d\x6a\x20\x22\x7a\x65\x6d\x68\x43\x15\x52\x37\x62\x6c\xd\x4b\xe\x3a\x3a\x6c\x3a\x53\x49\x78\x55\x48"
         "\x27\x65\x70\x39\x6c\x5b\x7a\x63\x4b\x67\x60\x4e\x43\x77\x7\x2d\x2d\x17\x31\x30\x53\x48\x43\x6f\x1e\x59\x74"
         "\x4\x2e\x37\x30\x58\x5d\x38\x24") == 0xf82af1d6f2db1c5a);
static_assert(
    hash("\x39\x15\x30\x3b\x2d\x41\x24\x75\x15\x49\x52\x51\x3e\x41\x33\x9\x30\x33\x6\x76\x46\x35\x6\x34\x75\xe\xb\x5e"
         "\x72\x52\x5d\x4c\x46") == 0xa59f9766d026fadc);
static_assert(
    hash("\x62\x4c\x63\x5b\x22\x7a\x3b\x55\x5e\x5a\x6f\x65\x48\x4b\x14\x31\x77\x77\x72\x1a\x18\x63\x6d\x6a\x29\x36\x7a"
         "\x64\x60") == 0x7df5cfad3cbe5bd8);