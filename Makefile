CPP ?= clang++
REPO_ROOT := $(abspath .)
CPPFLAGS = -I$(REPO_ROOT)/include -I/usr/local/opt/llvm/include -I/usr/local/opt/armadillo/include -fopenmp -O2 -std=c++20 -DWIGNER_BASIS_DATA_DIR=\"$(REPO_ROOT)/data\"
# Optional: WIGNER_CPP_INCLUDE=/path/to/wigner-cpp/include
ifdef WIGNER_CPP_INCLUDE
CPPFLAGS += -I$(WIGNER_CPP_INCLUDE)
endif
LDFLAGS = -L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib -DARMA_DONT_USE_WRAPPER -framework Accelerate

all: gaunt

gaunt: gaunt_coeffs.cpp
	$(CPP)	$(CPPFLAGS)	$^	-o	$@	$(LDFLAGS)
