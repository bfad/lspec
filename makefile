CC = gcc
AS = as
KERNEL_NAME = $(shell uname -s)
LIB_LOC_DIR = /usr/local/lib

MAKE_TARGET = lspec

ifeq ($(KERNEL_NAME),Darwin)
BUILD_UNIVERSAL = 1
endif

ifeq ($(BUILD_UNIVERSAL),1)
ARCH_PART = -arch x86_64 -arch i386 -read_only_relocs suppress
endif

OSX_DEPLOY_VERS = 10.5
SDK = /Developer/SDKs/MacOSX$(OSX_DEPLOY_VERS).sdk

ifeq ($(DEBUG),1)

MDEBUG_FLAGS = -g
DEBUG_FLAGS = -g3 -O0

ifeq ($(KERNEL_NAME),Darwin)
DEBUG_CMD = dsymutil --out="$(DLL_FINALE_NAME).dSYM" $@
DEBUG_CMD_EXE = dsymutil --out="$(EXE_DST)/$@.dSYM" $@
endif

else

MDEBUG_FLAGS = -O
DEBUG_FLAGS = 
DEBUG_CMD = 

endif

define INSTALL
	$(INSTALL_CMD) $1 $2
endef

Darwin_DLL_FLAGS = -dynamiclib #-read_only_relocs suppress
Darwin_LN_FLAGS = -isysroot $(SDK) -Wl,-syslibroot,$(SDK) -mmacosx-version-min=$(OSX_DEPLOY_VERS) -macosx_version_min=$(OSX_DEPLOY_VERS) -F/Library/Frameworks -framework Lasso9
Darwin_DLL_EXT = .dylib
Linux_DLL_FLAGS = -shared
Linux_LN_FLAGS = -llasso9_runtime #-Wl,-rpath,$(LIB_LOC_DIR)
Linux_DLL_EXT = .so

DLL_EXT = $($(KERNEL_NAME)_DLL_EXT)
#LINK_DST = $(LASSO9_HOME)/LassoLibraries/builtins
LINK_DST = $(LASSO9_HOME)/LassoLibraries
#LASSOAPP_DST = $(LASSO9_HOME)/LassoApps
EXE_DST = $(LASSO9_HOME)/LassoExecutables

LASSO9_MODULE_CMD = $(EXE_DST)/lassoc
INSTALL_CMD = cp -f 

C_FLAGS = $(DEBUG_FLAGS) -fPIC
LN_FLAGS = $(DEBUG_FLAGS) $($(KERNEL_NAME)_LN_FLAGS)
DLL_FLAGS = $($(KERNEL_NAME)_DLL_FLAGS) 

CC_ERR = #> /dev/null 2>&1

ASSEMBLY_EXT = s
#BC_EXT = .bc
#LASSOAPP_EXT = .lassoapp
INTERMEDIATE_EXT = $(ASSEMBLY_EXT)

MODULE_DLL_FLAGS = $(MDEBUG_FLAGS) -dll -n -s
#MODULE_LASSOAPP_FLAGS = $(MDEBUG_FLAGS) -dll -n -s -lassoapp
#MODULE_APP_FLAGS = $(MDEBUG_FLAGS) -app -n -s

#DLL_FINALE_NAME = $(LINK_DST)/$(basename $@)$(DLL_EXT)
ifeq ($@,install)
DLL_FINALE_NAME = $(LINK_DST)/$(basename $@)$(DLL_EXT)
else
DLL_FINALE_NAME = $(LINK_DST)/$(basename $(MAKE_TARGET))$(DLL_EXT)
endif
#BC_FINALE_NAME = $(LINK_DST)/$(basename $@)$(BC_EXT)
#LASSOAPP_FINALE_NAME = $(LASSOAPP_DST)/$(basename $@)$(LASSOAPP_EXT)
#EXE_FINAL_NAME = $(EXE_DST)/$(basename $@)

#COMPAT_DEPS_LIST = compat/compat.bytes compat/compat.compare \
					compat/compat.database compat/compat.date \
					compat/compat.define compat/compat.encoding \
					compat/compat.error compat/compat.file \
					compat/compat.global compat/compat.map \
					compat/compat.match compat/compat.math \
					compat/compat.misc compat/compat.null \
					compat/compat.priorityqueue compat/compat.sessions \
					compat/compat.string compat/compat.var \
					compat/compat.valid

#STD_LIST = lasso9-core support_paths locale \
				file dir net regexp sqlite curl database \
				queue date duration list random ljapi pdf \
				inline zip set xml serialization bom json \
				encrypt sys_process worker_pool debugger $(COMPAT_DEPS_LIST)
#DLL_LIST = $(addsuffix $(DLL_EXT), $(STD_LIST) $(COMPAT_DEPS_LIST))
#BC_LIST = $(addsuffix $(BC_EXT), $(STD_LIST) $(COMPAT_DEPS_LIST))

#all: base lasso9
all: $(MAKE_TARGET)$(DLL_EXT)
	

debug:
	$(MAKE) DEBUG=1 -j2

#base: $(DLL_LIST)
	
#bc: $(BC_LIST)
	

makefile: ;

%.d.$(INTERMEDIATE_EXT): %.inc
	"$(LASSO9_MODULE_CMD)" $(MODULE_DLL_FLAGS) -o $@ $<
#%.ap.$(INTERMEDIATE_EXT): %
#	"$(LASSO9_MODULE_CMD)" $(MODULE_LASSOAPP_FLAGS) -o $@ $<
#%.a.$(INTERMEDIATE_EXT): %.inc
#	"$(LASSO9_MODULE_CMD)" $(MODULE_APP_FLAGS) -o $@ $<
%.d.i386.$(INTERMEDIATE_EXT): %.inc
	"$(LASSO9_MODULE_CMD)" $(MODULE_DLL_FLAGS) -arch i386 -o $@ $<
#%.ap.i386.$(INTERMEDIATE_EXT): %
#	"$(LASSO9_MODULE_CMD)" $(MODULE_LASSOAPP_FLAGS) -arch i386 -o $@ $<
#%.a.i386.$(INTERMEDIATE_EXT): %.inc
#	"$(LASSO9_MODULE_CMD)" $(MODULE_APP_FLAGS) -arch i386 -o $@ $<
%.d.x86_64.$(INTERMEDIATE_EXT): %.inc
	"$(LASSO9_MODULE_CMD)" $(MODULE_DLL_FLAGS) -arch x86_64 -o $@ $<
#%.ap.x86_64.$(INTERMEDIATE_EXT): %
#	"$(LASSO9_MODULE_CMD)" $(MODULE_LASSOAPP_FLAGS) -arch x86_64 -o $@ $<
#%.a.x86_64.$(INTERMEDIATE_EXT): %.inc
#	"$(LASSO9_MODULE_CMD)" $(MODULE_APP_FLAGS) -arch x86_64 -o $@ $<

#%$(BC_EXT): %.inc
#	"$(LASSO9_MODULE_CMD)" -dll -o $@ $<
#	@mkdir -p "$(LINK_DST)"
#	@mkdir -p "$(LINK_DST)/compat"
#	@$(INSTALL_CMD) "`pwd`/$@" "$(BC_FINALE_NAME)"

ifeq ($(BUILD_UNIVERSAL),1)
%.o: %.x86_64.$(INTERMEDIATE_EXT) %.i386.$(INTERMEDIATE_EXT)
	$(AS) -arch x86_64 -o $(basename $@).x86_64.o $(basename $@).x86_64.$(INTERMEDIATE_EXT)
	$(AS) -arch i386 -o $(basename $@).i386.o $(basename $@).i386.$(INTERMEDIATE_EXT)
	lipo -create $(basename $@).x86_64.o $(basename $@).i386.o -output $@
	-@rm $(basename $@).x86_64.o $(basename $@).i386.o

else
%.o: %.$(INTERMEDIATE_EXT)
	$(AS) -o $@ $<
endif

%$(DLL_EXT): %.d.o
	$(CC) $(ARCH_PART) $(DLL_FLAGS) $(LN_FLAGS) -o $@ $< #> /dev/null 2>&1
#	@mkdir -p "$(LINK_DST)"
#	@mkdir -p "$(LINK_DST)/compat"
	$(DEBUG_CMD)
#	@$(call INSTALL, $@, "$(DLL_FINALE_NAME)")

#%$(LASSOAPP_EXT): %.ap.o
#	$(CC) $(ARCH_PART) $(DLL_FLAGS) $(LN_FLAGS) -o $@ $< $(CC_ERR)
#	@mkdir -p "$(LASSOAPP_DST)"
#	$(DEBUG_CMD)
#	@$(call INSTALL, $@, "$(LASSOAPP_FINALE_NAME)")

#%: %.a.o
#	$(CC) $(ARCH_PART) $(LN_FLAGS) -o $@ $@.a.o $(CC_ERR)
#	$(DEBUG_CMD_EXE)
#	@$(call INSTALL, $@, "$(EXE_FINAL_NAME)")

clean:
#	-rm *.$(INTERMEDIATE_EXT) *$(DLL_EXT) *$(BC_EXT) *$(LASSOAPP_EXT) *.o
	-rm *.$(INTERMEDIATE_EXT) *$(DLL_EXT) *.o
#	-rm compat/*.$(INTERMEDIATE_EXT) compat/*$(DLL_EXT) compat/*.o
	-rm -rf *.dSYM
#	-rm lasso9
install:
	-mv -f "$(MAKE_TARGET)$(DLL_EXT)" "$(DLL_FINALE_NAME)"