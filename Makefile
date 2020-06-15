MAKEFLAGS += --no-print-directory

INC_DIR=weilei_lib
#INC_DIR=~/working/weilei_lib
CXX=g++ -O3 -Wall -std=c++11 -fopenmp
### -O2 -O5 -Os
#g++ `pkg-config --cflags itpp` -o hello.out hello.cpp `pkg-config --libs itpp`

START=`pkg-config --cflags itpp`
END=`pkg-config --libs itpp`
ITPP=`pkg-config --cflags itpp` `pkg-config --libs itpp`
#files=$(INC_DIR)/mm_read.c $(INC_DIR)/mm_read.h $(INC_DIR)/mmio.c $(INC_DIR)/mmio.h $(INC_DIR)/mm_write.c $(INC_DIR)/mm_write.h $(INC_DIR)/lib.cpp $(INC_DIR)/lib.h $(INC_DIR)/dist.c $(INC_DIR)/dist.h $(INC_DIR)/concatenation_lib.c $(INC_DIR)/concatenation_lib.h  $(INC_DIR)/bp.c $(INC_DIR)/bp.h $(INC_DIR)/my_lib.h Makefile
files=$(INC_DIR)/mm_read.cpp $(INC_DIR)/mm_read.h $(INC_DIR)/mmio.c $(INC_DIR)/mmio.h $(INC_DIR)/mm_write.cpp $(INC_DIR)/mm_write.h $(INC_DIR)/lib.cpp $(INC_DIR)/lib.h $(INC_DIR)/dist.cpp $(INC_DIR)/dist.h $(INC_DIR)/product.cpp $(INC_DIR)/product.h  $(INC_DIR)/bp.cpp $(INC_DIR)/bp.h $(INC_DIR)/weilei_lib.h Makefile
command=$(CXX) $(START) -o $@ $< $(word 2,$^) $(word 4, $^) $(word 6, $^) $(word 8, $^) $(word 10, $^) $(word 12, $^) $(word 14, $^) $(END)

###include all headfiles into my_lib.h
gnuplot_dist.out:gnuplot_dist.c $(files)
	$(command)
random_concatenation.out:random_concatenation.c $(files)
	$(command)
counter_concatenation.out:counter_concatenation.c $(files)
	$(command)
#product.out:product.c $(files)
#	$(command) -fopenmp
concatenation.out:concatenation.c $(files)
	$(command)
hypergraph.out:hypergraph.c $(files)
	$(command)

#test.out:test.c $(files)
#	$(command)


product:
	make product.o && make lib && make product.out

#product.c $(files)
#$(command) -fopenmp


lib:
	cd weilei_lib && make all
#test.o:test.c
%.o:%.cpp
	$(CXX) $(START) $(END) -c $<

object_files=$(INC_DIR)/mm_read.o $(INC_DIR)/mmio.o $(INC_DIR)/mm_write.o $(INC_DIR)/lib.o $(INC_DIR)/dist.o $(INC_DIR)/product_lib.o $(INC_DIR)/bp.o 
#test-link:test.o $(obejct_files)
%.out:%.o $(obejct_files)
	$(CXX) $(START) $(END) -o $@ $< $(object_files)
test:
	make test.o && make lib && make test.out

clean:
	rm  *~
	rm \#*

sbatch-dry-run:
	sbatch --test run_prod.sh
sbatch:
	sbatch run_prod.sh
pkill-product:
	pkill .product
