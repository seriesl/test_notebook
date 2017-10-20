CC = gcc-7
FC = gfortran-7 -fno-underscoring
LD = gcc-7 #-lgfortran

CCFLAGS = 
LDFLAGS = 

OBJS = dc_decsol.o  decsol.o radau5.o integration.o 
LIB = integration.so

all: $(LIB)

lib: $(LIB)

$(LIB): $(OBJS)
	$(CC) -shared -o $@ $^ -lgfortran

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

%.o : %.f90
	$(FC) $(FCFLAGS) -c $< -o $@

clean:
	rm -f $(LIB) $(EXE) $(OBJS)
