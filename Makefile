#############################################################################

DIR=$(shell pwd)

#############################################################################

CINT          = rootcint
ROOTCONFIG    = root-config

#############################################################################

ROOTLIBS      = $(shell $(ROOTCONFIG) --libs)
ROOTGLIBS     = $(shell $(ROOTCONFIG) --glibs)
ROOTCFLAGS    = $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS   = $(shell $(ROOTCONFIG) --ldflags)

#############################################################################

ATLASFLAGS = -DSTANDALONE \
-I tools/ggFReweighting-00-00-08 \
-I tools/HiggsZZ4lUtils-00-00-10 \
-I tools/PileupReweighting-00-02-03 \
-I tools/TrigMuonEfficiency-00-01-22 \
-I tools/egammaAnalysisUtils-00-03-04 \
-I tools/MuonIsolationCorrection-00-08 \
-I tools/MuonMomentumCorrections-00-05-03 \
-I tools/MuonEfficiencyCorrections-01-01-04

ATLASLIBS = -L./tools \
-legammaAnalysisUtils \
-lggFReweighting \
-lHiggsZZ4lUtils \
-lMuonEfficiencyCorrections \
-lMuonIsolationCorrection \
-lMuonMomentumCorrections \
-lPileupReweighting \
-lTrigMuonEfficiency

#############################################################################

CXX           =g++
CXXFLAGS      =-std=c++98 -g -O2 -fPIC -ansi -Wall $(ROOTCFLAGS) $(ATLASFLAGS)

LD            =g++
LDFLAGS       =-g $(ROOTLDFLAGS) -L.

LIBS          = $(ROOTLIBS) -lTreePlayer
GLIBS         = $(ROOTGLIBS) -lTreePlayer

#############################################################################

OBJS1         = core/main.o core/core.o core/config.o core/loader.o core/utils.o core/selection/isLoosePP.o core/selection/selection.o core/selection/triggers.o

OBJS2         = analysis/test/main.o

OBJS3         = analysis/l_analysis/main.o

#############################################################################

all: core $(OBJS2) $(OBJS3)
#	@cd $(DIR)/tools && make all && cd $(DIR)

	$(LD) $(LDFLAGS) -o test $(OBJS2) $(LIBS) -L. -lcore $(ATLASLIBS)

	$(LD) $(LDFLAGS) -o l_analysis $(OBJS3) $(LIBS) -L. -lcore $(ATLASLIBS)

#############################################################################

ALL: core $(OBJS2) $(OBJS3)
	@cd $(DIR)/tools && make all && cd $(DIR)

	$(LD) $(LDFLAGS) -o test $(OBJS2) $(LIBS) -L. -lcore $(ATLASLIBS)

	$(LD) $(LDFLAGS) -o l_analysis $(OBJS3) $(LIBS) -L. -lcore $(ATLASLIBS)

#############################################################################

clean:
#	@cd $(DIR)/tools && make clean && cd $(DIR)

	rm -fr $(OBJS1) $(OBJS2) $(OBJS3) libcore.a

#############################################################################

CLEAN:
	@cd $(DIR)/tools && make clean && cd $(DIR)

	rm -fr $(OBJS1) $(OBJS2) $(OBJS3) libcore.a

#############################################################################

core: $(OBJS1)
	ar -rcs libcore.a $(OBJS1) && ranlib libcore.a

#############################################################################

%.o: %.C
	@printf "Building $@\n"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf "[ \033[32mOk.\033[0m ]\n"

%.o: %.cc
	@printf "Building $@\n"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf "[ \033[32mOk.\033[0m ]\n"

%.o: %.cxx
	@printf "Building $@\n"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf "[ \033[32mOk.\033[0m ]\n"

#############################################################################

