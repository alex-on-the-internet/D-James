# Builds the benchmark of one or all parameter sets:
#
#   make                 all four sets
#   make d-james-128     D-James, 128-bit security   -> build/d-james-128/bench
#   make d-james-256
#   make james-128
#   make james-256
#   make clean
#
# Dependencies (Debian/Ubuntu package names in parentheses):
#   NTL     (libntl-dev)   - polynomial factorisation over GF(2^n)
#   GMP     (libgmp-dev)   - needed by NTL
#   gf2x    (libgf2x-dev)  - multiplication in GF(2)[x]
#   OpenSSL (libssl-dev)   - AES, used by the NIST DRBG (src/rng.c)
#   XKCP                   - SHA-3 (Keccak): git submodule in third_party/XKCP,
#                            built by this Makefile (needs xsltproc)
#
# Variables that can be overridden, e.g. make XKCP_TARGET=AVX2 LOCAL=/opt/local
#   LOCAL       prefix of NTL, GMP and gf2x if they are not in the default paths
#   XKCP_TARGET XKCP target to build (generic64, AVX2, ...)
#   XKCP_DIR    directory containing libXKCP.a and libXKCP.a.headers; if set,
#               the submodule is not built

LOCAL       ?= /usr/local
XKCP_TARGET ?= generic64
XKCP_LIB    := third_party/XKCP/bin/$(XKCP_TARGET)/libXKCP.a
ifndef XKCP_DIR
    XKCP_DIR := third_party/XKCP/bin/$(XKCP_TARGET)
    XKCP_DEP := $(XKCP_LIB)
endif

CXX      ?= g++
CXXFLAGS ?= -Wall -O3
CPPFLAGS += -I include -I $(LOCAL)/include -I $(XKCP_DIR)
LDFLAGS  += -L $(LOCAL)/lib -L $(XKCP_DIR)
LDLIBS   := -lntl -lgmp -lXKCP -lgf2x -lcrypto -ldl -lpthread

SETS = d-james-128 d-james-256 james-128 james-256
SRC  = $(wildcard src/*.c src/*.cpp)
HEAD = $(wildcard include/*.h include/*.hpp)

all: $(SETS)

# Parameter set macro: d-james-128 -> D_JAMES_128
define set_rules
$(1): build/$(1)/bench
build/$(1)/bench: $$(addprefix build/$(1)/,$$(addsuffix .o,$$(basename $$(notdir $$(SRC)))))
	$$(CXX) $$(LDFLAGS) -o $$@ $$^ $$(LDLIBS)
build/$(1)/%.o: src/%.c $$(HEAD) $$(XKCP_DEP) | build/$(1)
	$$(CXX) $$(CPPFLAGS) -D$(2) $$(CXXFLAGS) -x c++ -c -o $$@ $$<
build/$(1)/%.o: src/%.cpp $$(HEAD) $$(XKCP_DEP) | build/$(1)
	$$(CXX) $$(CPPFLAGS) -D$(2) $$(CXXFLAGS) -c -o $$@ $$<
build/$(1):
	mkdir -p $$@
endef
$(eval $(call set_rules,d-james-128,D_JAMES_128))
$(eval $(call set_rules,d-james-256,D_JAMES_256))
$(eval $(call set_rules,james-128,JAMES_128))
$(eval $(call set_rules,james-256,JAMES_256))

# XKCP is a submodule which has a submodule itself (XKCBuild)
$(XKCP_LIB):
	git submodule update --init --recursive third_party/XKCP
	# XKCP does not support parallel builds: do not inherit -j
	cd third_party/XKCP && env -u MAKEFLAGS -u MFLAGS $(MAKE) -s $(XKCP_TARGET)/libXKCP.a

clean:
	rm -rf build

.PHONY: all clean $(SETS)
