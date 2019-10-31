CXXFLAGS = -std=c++11 -pedantic-errors -Wall -Wextra -g -O3 -march=native

PROGS = comp

all: $(PROGS)


OBJS = bb.o bb-dom-tree.o insn.o flow-graph.o

comp: $(OBJS)


# Include file dependencies, which should be transitively used by
# dependent source files.
#
bb.h-DEPS = memtoobj.h $(memtoobj.h-DEPS)
flow-graph.h-DEPS = bb.h $(bb.h-DEPS)

# Object file dependencies, basically the corresponding source file
# plus any include files it uses.
#
bb.o: bb.cc bb.h $(bb.h-DEPS) insn.h $(insn.h-DEPS)
bb-dom-tree.o: bb-dom-tree.cc bb.h $(bb.h-DEPS)
insn.o: insn.cc insn.h $(insn.h-DEPS) bb.h $(bb.h-DEPS)
flow-graph.o: flow-graph.cc flow-graph.h $(flow-graph.h-DEPS)

bitvec.o: bitvec.cc bitvec.h $(bitvec.h-DEPS)


clean:
	rm -f $(PROGS) *.o
