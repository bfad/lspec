PROJECT = lspec
CC = gcc
KERNEL_NAME = $(shell uname -s)

OSX_DEPLOY_VERS = 10.5
SDK = $(shell xcodebuild -version -sdk macosx Path)

ifeq ($(DEBUG),1)

MDEBUG_FLAGS = -g
DEBUG_FLAGS = -g3 -O0

ifeq ($(KERNEL_NAME),Darwin)
DEBUG_CMD = dsymutil --out="$(DLL_FINALE_NAME).dSYM" $@
DEBUG_CMD_EXE = dsymutil --out="$@.dSYM" $@
endif

else

MDEBUG_FLAGS = -O
DEBUG_FLAGS = 
DEBUG_CMD = 

endif

Darwin_DLL_FLAGS = -dynamiclib
Darwin_LN_FLAGS = -isysroot $(SDK) -Wl,-syslibroot,$(SDK) -mmacosx-version-min=$(OSX_DEPLOY_VERS) -F/Library/Frameworks -framework Lasso9
Darwin_DLL_EXT = .dylib
Linux_DLL_FLAGS = -shared
Linux_LN_FLAGS = -llasso9_runtime
Linux_DLL_EXT = .so

DLL_EXT = $($(KERNEL_NAME)_DLL_EXT)

LASSO9_MODULE_CMD = $(shell which lassoc)

C_FLAGS = $(DEBUG_FLAGS) -fPIC
LN_FLAGS = $(DEBUG_FLAGS) $($(KERNEL_NAME)_LN_FLAGS)
DLL_FLAGS = $($(KERNEL_NAME)_DLL_FLAGS) 

LASSO_EXT = inc
BC_EXT = .bc
LASSOAPP_EXT = .lassoapp
INTERMEDIATE_EXT = o

MODULE_DLL_FLAGS = $(MDEBUG_FLAGS) -dll -n -obj
MODULE_LASSOAPP_FLAGS = $(MDEBUG_FLAGS) -dll -n -obj -lassoapp
MODULE_APP_FLAGS = $(MDEBUG_FLAGS) -app -n -obj

DLL_FINALE_NAME = $(basename $@)$(DLL_EXT)
BC_FINALE_NAME = $(basename $@)$(BC_EXT)
LASSOAPP_FINALE_NAME = $(basename $@)$(LASSOAPP_EXT)
EXE_FINAL_NAME = $(basename $@)

ifeq (test -d $(LASSO9_HOME), 0)
INSTALL_HOME = $(LASSO9_HOME)
else
INSTALL_HOME = /var/lasso/home
endif



all: $(PROJECT) $(PROJECT)$(DLL_EXT)
	
debug:
	$(MAKE) DEBUG=1 -j2
	
install:
	mkdir -p $(INSTALL_HOME)/bin
	mkdir -p $(INSTALL_HOME)/LassoLibraries
	mv -f $(PROJECT) $(INSTALL_HOME)/bin/
	mv -f $(PROJECT)$(DLL_EXT) $(INSTALL_HOME)/LassoLibraries

makefile: ;

%.d.$(INTERMEDIATE_EXT): %.$(LASSO_EXT)
	"$(LASSO9_MODULE_CMD)" $(MODULE_DLL_FLAGS) -o $@ $<
%.ap.$(INTERMEDIATE_EXT): $(PROJECT_LASSOAPP_DIR)/
	"$(LASSO9_MODULE_CMD)" $(MODULE_LASSOAPP_FLAGS) -o $@ $<
%.a.$(INTERMEDIATE_EXT): command/$(PROJECT)#%.$(LASSO_EXT)
	"$(LASSO9_MODULE_CMD)" $(MODULE_APP_FLAGS) -o $@ $<
%.d.i386.$(INTERMEDIATE_EXT): %.$(LASSO_EXT)
	"$(LASSO9_MODULE_CMD)" $(MODULE_DLL_FLAGS) -arch i386 -o $@ $<
%.ap.i386.$(INTERMEDIATE_EXT): %
	"$(LASSO9_MODULE_CMD)" $(MODULE_LASSOAPP_FLAGS) -arch i386 -o $@ $<
%.a.i386.$(INTERMEDIATE_EXT): %.$(LASSO_EXT)
	"$(LASSO9_MODULE_CMD)" $(MODULE_APP_FLAGS) -arch i386 -o $@ $<
%.d.x86_64.$(INTERMEDIATE_EXT): %.$(LASSO_EXT)
	"$(LASSO9_MODULE_CMD)" $(MODULE_DLL_FLAGS) -arch x86_64 -o $@ $<
%.ap.x86_64.$(INTERMEDIATE_EXT): %
	"$(LASSO9_MODULE_CMD)" $(MODULE_LASSOAPP_FLAGS) -arch x86_64 -o $@ $<
%.a.x86_64.$(INTERMEDIATE_EXT): %.$(LASSO_EXT)
	"$(LASSO9_MODULE_CMD)" $(MODULE_APP_FLAGS) -arch x86_64 -o $@ $<

%$(BC_EXT): %.$(LASSO_EXT)
	"$(LASSO9_MODULE_CMD)" -dll -o $@ $<

%$(DLL_EXT): %.d.o
	$(CC) $(ARCH_PART) $(DLL_FLAGS) -o $@ $< $(LN_FLAGS)
	#$(DEBUG_CMD)

%$(LASSOAPP_EXT): %.ap.o
	$(CC) $(ARCH_PART) $(DLL_FLAGS) -o $@ $< $(LN_FLAGS)
	#$(DEBUG_CMD)

%: %.a.o
	$(CC) $(ARCH_PART) -o $@ $@.a.o $(LN_FLAGS)
	#$(DEBUG_CMD_EXE)

clean:
	@rm *.$(INTERMEDIATE_EXT) *$(DLL_EXT) *$(BC_EXT) *$(LASSOAPP_EXT) *.o 2>/dev/null || :
	@rm compat/*.$(INTERMEDIATE_EXT) compat/*$(DLL_EXT) compat/*$(BC_EXT) compat/*.o 2>/dev/null  || :
	@rm -rf *.dSYM 2>/dev/null  || :
	@rm lasso9 2>/dev/null  || :
