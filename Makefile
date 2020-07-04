MAKEFLAGS += --no-print-directory

# build options to test dynamic library
LIB_WEILEI_PATH=/rhome/wzeng002/.local/lib
LIB_WEILEI=-L$(LIB_WEILEI_PATH) -lweilei -Iweilei_lib


INC_DIR=weilei_lib
#INC_DIR=~/working/weilei_lib
CXX=g++ -O3 -Wall -std=c++11 -fopenmp 
# optimization options -O2 -O5 -Os
ITPP=`pkg-config --cflags itpp` `pkg-config --libs itpp`
#full command example
#g++ `pkg-config --cflags itpp` -o hello.out hello.cpp `pkg-config --libs itpp` -fopenmp


#make object file for target file
%.o:%.cpp $(object_files)
	$(CXX) $(START) $(END) -c $<
#compile object files for lib files
lib:
	cd weilei_lib && make all
#now link object files
object_files=$(INC_DIR)/mm_read.o $(INC_DIR)/mmio.o $(INC_DIR)/mm_write.o $(INC_DIR)/lib.o $(INC_DIR)/dist.o $(INC_DIR)/product_lib.o $(INC_DIR)/bp.o 
%.out:%.o $(obejct_files)
	$(CXX) $(ITPP) -o $@ $< $(object_files)

cmd=make lib && make $@.o  && make $@.out
# eg: make test.o && make lib && make test.out
product:
	$(cmd)
test:
	$(cmd)


clean:
	rm *.o
	rm *.out

#job management
sbatch-dry-run:
	sbatch --test run_prod.sh
sbatch:
	sbatch run_prod.sh
pkill-product:
	pkill .product



#test dynamic lib
dynamic:$(LIB_WEILEI_PATH)/libweilei.so
	cd weilei_lib && make libweilei.so
	$(CXX) $(ITPP) -o test_dynamic.out test.cpp -lweilei -L$(LIB_WEILEI_PATH)
	./test_dynamic.out



############to be discared

START=`pkg-config --cflags itpp`
END=`pkg-config --libs itpp`
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

