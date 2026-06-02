CPP = /usr/local/opt/llvm/bin/clang++
REPO_ROOT := $(abspath ..)
CPPFLAGS = -I$(REPO_ROOT)/include -I/usr/local/opt/llvm/include -I/usr/local/opt/armadillo/include -I/usr/local/include -fopenmp -O2 -std=c++20 -DWIGNER_BASIS_DATA_DIR=\"$(REPO_ROOT)/data\"
LDFLAGS = -L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib -L/usr/local/lib -L/usr/local/opt/armadillo/lib -lwignerSymbols -larmadillo -framework Accelerate
.PHONY : clean

all: ham

ham: hamiltonian.cpp
	$(CPP)	-O2	$(CPPFLAGS)	$^	-o	$@	$(LDFLAGS)

clean:
	-rm ham
