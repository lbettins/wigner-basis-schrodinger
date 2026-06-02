CXX = /usr/local/opt/llvm/bin/clang++
REPO_ROOT := $(abspath .)
BIN_DIR := $(REPO_ROOT)/bin
CXXFLAGS = -I$(REPO_ROOT)/include -I/usr/local/opt/llvm/include -I/usr/local/opt/armadillo/include -I/usr/local/include -fopenmp -O2 -std=c++20 -DWIGNER_BASIS_DATA_DIR=\"$(REPO_ROOT)/data\"
LDFLAGS = -L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib -L/usr/local/lib -L/usr/local/opt/armadillo/lib -lwignerSymbols -larmadillo -framework Accelerate

# Optional: WIGNER_CPP_INCLUDE=/path/to/wigner-cpp/include make gaunt_aux
ifdef WIGNER_CPP_INCLUDE
CXXFLAGS += -I$(WIGNER_CPP_INCLUDE)
endif

.PHONY: all ham gaunt_aux clean

all: ham

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

ham: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) src/hamiltonian.cpp -o $(BIN_DIR)/ham $(LDFLAGS)

gaunt_aux: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) src/aux/gaunt_coeffs.cpp -o $(BIN_DIR)/gaunt_aux $(LDFLAGS)

clean:
	-rm -rf $(BIN_DIR)
