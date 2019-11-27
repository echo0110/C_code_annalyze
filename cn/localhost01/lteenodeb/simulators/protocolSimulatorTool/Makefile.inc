# this is needed as setting .DEFAULT_GOAL does not seem to work
all:

include $(MAKE_ROOT)/flags.mk

CC=$(LTE_CC)    
CXX=$(LTE_CC)     

ifndef ARCH
  ARCH := $(patsubst i%86,i686,$(shell uname -m))
endif

ifeq ($(filter $(ARCH),$(KNOWN_ARCH)),)
  $(error Unknown architecture '$(ARCH)'. Known architectures are: $(foreach a,$(KNOWN_ARCH),'$a'))
endif

ifeq ($(filter $(MODE),$(KNOWN_MODE)),)
  $(error Unknown mode '$(MODE)'. Known modes are: $(foreach m,$(KNOWN_MODE),'$m'))
endif

C_FLAGS := $(C_FLAGS) $(C_ADD_DEF) $(C_FLAGS_$(ARCH)) $(C_FLAGS_$(MODE)) $(C_FLAGS_$(ARCH)_$(MODE))
CFLAGS := $(strip $(C_FLAGS) $(CFLAGS) $(CFLAGS_$(ARCH)) $(CFLAGS_$(MODE)) $(CFLAGS_$(ARCH)_$(MODE)))
CXXFLAGS := $(strip $(C_FLAGS) $(CXXFLAGS) $(CXXFLAGS_$(ARCH)) $(CXXFLAGS_$(MODE)) $(CXXFLAGS_$(ARCH)_$(MODE)) $(LIBXML++INC))
LDFLAGS := $(strip $(LDFLAGS) $(LDFLAGS_$(ARCH)) $(LDFLAGS_$(MODE)) $(LDFLAGS_$(ARCH)_$(MODE)))
CPPFLAGS := $(strip $(CPPFLAGS) $(CPPFLAGS_$(ARCH)) $(CPPFLAGS_$(MODE)) $(CPPFLAGS_$(ARCH)_$(MODE)))

INSTALL := $(shell which install)

BUILD_ROOT := $(MAKE_ROOT)/build/$(ARCH)-$(MODE)
INSTALL_ROOT := $(MAKE_ROOT)/build/$(ARCH)-$(MODE)

RPMS_DIR := $(MAKE_ROOT)/rpms

SO_FLAGS := -fpic 

LIB_PATH := $(BUILD_ROOT)/lib $(LIB_PATH)
            
FORCE_PREREQ := 1

.DELETE_ON_ERROR:

# stored in .sig files, should be incremented to mark incompatible changes to
# build system, mandating rebuild of targets
CBSVER := 1

################################################################################

# cancel default implicit rules

%.o:%.c
%.o:%.cpp
%:%.c
%:%.cpp

#### target type specific code ####

#####
# $1 list of patterns
# $2 substitution, usually just '%'
# $3 words to process

define lpatsubst
$(if $1,$(patsubst $(firstword $1),$2,$(call lpatsubst,$(wordlist 2,$(words $1),$1),$2,$3)),$3)
endef

#####
# $1 directory name

define make_dir_target
$(eval _s := $(patsubst %/,%,$1))
$(eval dirmark := $(_s)/.mark)
$(if $(MKDIR_$(_s)),,
  $(eval MKDIR_$(_s) := 1)
  $(_s)/.mark:
	@mkdir -p $(_s)
	@touch $(dirmark)
)
endef

#####
# $1 file name
# $2 data
# $3 dependencies

define make_sig_target_prim
  $(call make_dir_target,$(dir $1))
  $1: $3 $(dirmark)
	@echo '$(CBSVER) -- $2' > $1.tmp && if cmp -s $1.tmp $1; then rm $1.tmp; else mv $1.tmp $1; fi
endef

#####
# $1 target name
# $2 directory relative to MAKE_ROOT
# $3 base name (i.e. 'compiler', 'linker')
# $4 data

define make_sig_target
  $(call make_sig_target_prim,$(BUILD_ROOT)$2/$1/$3.sig,$4,$(BASE_MAKEFILES) $(MAKE_ROOT)$2/user.mk)
  $2/$1/clean::
	@-rm -f $(BUILD_ROOT)$2/$1/$3.sig

endef

source_types := %.c %.cpp

#####
# $1 source
# $2 directory relative to MAKE_ROOT
# $3 suffix (e.g. 'cpp')
# $4 compiler
# $5 flags
# $6 .d suffix
# $7 .o suffix
# $8 target
# $9 compiler type (i.e. 'CC', 'CXX') for activity display purposes
# $10 PCH suffix (0 for executables and static libs, 1 for shared libs) , or '' if disabled
#
# relies on $(dirmark) being set

define make_o_d
  $2/$8/$(1:.$3=.$7): $(BUILD_ROOT)$2/$8/$(1:.$3=.$7)
  ALL_TARGETS += $2/$8/$(1:.$3=.$7)
  .PHONY: $2/$8/$(1:.$3=.$7)

  $(BUILD_ROOT)$2/$8/$(1:.$3=.$7): $(MAKE_ROOT)$2/$1 $(dirmark) $(BUILD_ROOT)$2/$8/compiler.sig $(if $(10),$(BUILD_ROOT)/PRECOMPILED$(10).h.gch)
	@echo '[$2/$8] $9 $1'
	@cd $(MAKE_ROOT) && \
	$(if $(10),CCACHE_CPP2= )$4 -c $$(patsubst $(MAKE_ROOT)/%,%,$$<) \
	  $(if $(10),-I$(patsubst $(SIMROOT)%,$(SIM_REL_MAKE_ROOT)%,$(BUILD_ROOT)) -include PRECOMPILED$(10).h -Winvalid-pch) \
	  $(patsubst -I$(SIMROOT)%,-I$(SIM_REL_MAKE_ROOT)%,$5) \
	  -o $$(patsubst $(MAKE_ROOT)/%,%,$$@)

  $(BUILD_ROOT)$2/$8/$(1:.$3=.$6): $(MAKE_ROOT)$2/$1 $(dirmark) $(BUILD_ROOT)$2/$8/compiler.sig
	@echo '[$2/$8] making dependencies for $1'
	@$(4:ccache=) -MM -MG -MT '$(BUILD_ROOT)$2/$8/$(1:.$3=.$7) $(BUILD_ROOT)$2/$8/$(1:.$3=.$6)' $$< $5 |\
	  sed -r -e 's"$(SIMROOT)"$$$$(SIMROOT)"g' \
	         $(foreach l,$(DEP_IGNORE), -e 's"\$$$$\(SIMROOT\)/$l/[^ ]+""g') > $$@
  
  $2/$8/$(1:.$3=.i):
	@cd $(MAKE_ROOT) && \
	$(4:ccache=) -E .$2/$1 $(patsubst -I$(SIMROOT)%,-I$(SIM_REL_MAKE_ROOT)%,$5)
  .PHONY: $2/$8/$(1:.$3=.i)

  TARGET_D_DEPS_$2/$8/$(1:.$3=.$7) := $(BUILD_ROOT)$2/$8/$(1:.$3=.$7)
  TARGET_D_INCLUDE_$(BUILD_ROOT)$2/$8/$(1:.$3=.$7) := $(BUILD_ROOT)$2/$8/$(1:.$3=.$6)

endef

#####
# $1 target name
# $2 directory relative to MAKE_ROOT

define make_testlibs_target
  $1/testlibs: $2/$1 $$(EXTRA_LIB_$1)
	@echo '[$2/$1] verifying library dependencies'
	@echo 'int main() {}'| $(CXX) \
            -o /dev/null \
            $(CXXFLAGS) $($1.CXXFLAGS) $(CPPFLAGS) $($1.CPPFLAGS) $(LDFLAGS) $($1.LDFLAGS) \
            $$(addprefix -L,$$(LIB_PATH)) \
            $$(addprefix -l,$$(LIB_ALL_XLIBS_$1) $$(LIB_ALL_LIBS_$1) $(TESTLIBS_EXTRA)) \
            -x c++ -
  TARGET_D_DEPS_$1/testlibs := $2/$1 $$(EXTRA_LIB_$1)

endef

#####
# $1 target name
# $2 directory relative to MAKE_ROOT
#
# Side-effect: _S is set to a list of sources with suffixes removed

define check_sources_types
  $(eval _S := $(filter-out $(source_types),$($1.SRCS)))
  $(if $(_S),$(error Sources '$(_S)' of unknown type are specified for target '$2/$1'))
  $(eval _S := $(call lpatsubst,$(source_types),%,$($1.SRCS)))
  $(if $(strip $(_S)),,$(error Empty SRCS list specified for target '$2/$1'))
endef

#####
# $1 target name
# $2 directory relative to MAKE_ROOT
# $3 destination directory (e.g. "bin", "lib")
# $4 .d suffix
# $5 .o suffix
# $6 bin name
# $7 install destination directory
# $8 .gcno suffix
# $9 .gcda suffix
# _S as set by check_source_types

define make_bin_targets
  $2/$1/all: $(INSTALL_ROOT)/$7/$6
  $2/$1/clean::
	@echo '[$2/$1] cleaning'
	@-rm -f $(INSTALL_ROOT)/$7/$6 $(foreach s,$5 $4 $8 $9,$(addprefix $(BUILD_ROOT)$2/$1/,$(addsuffix .$s,$(_S))))

  TARGET_D_DEPS_$2/$1/all += $(INSTALL_ROOT)/$7/$6

  $(call make_dir_target,$(BUILD_ROOT)$2/$1)
endef

#####
# $1 target name
# $2 directory relative to MAKE_ROOT
# $3 bin name

define make_lib_install_targets
  $2/$1/install::
	@echo '[$2/$1] installing $3 to $(LIB_INSTALL_DIR)'
	@$(INSTALL) -C $(INSTALL_ROOT)/lib/$3 $(LIB_INSTALL_DIR)/$3
  $2/$1/uninstall::
	@echo '[$2/$1] uninstalling $3 from $(LIB_INSTALL_DIR)'
	@-rm -f $(LIB_INSTALL_DIR)/$3
endef

bin_target_required_vars := SRCS
bin_target_optional_vars := CFLAGS CXXFLAGS CPPFLAGS LDFLAGS LIBS XLIBS DESTDIR NOPCH

################################################################################

target_type_executable_required_vars := $(bin_target_required_vars)
target_type_executable_optional_vars := $(bin_target_optional_vars)

define process_executable_target
  $(call check_target_uniq,$1,$2,$1)
  $(call check_target_uniq,$1,$2,$($1.DESTDIR)/$1)
  $(call check_sources_types,$1,$2)
  $(call compute_chmod,$1,$2,$($1.DESTDIR))

  $(call make_sig_target,$1,$2,linker,$($1.SRCS) -- $($1.LIBS) -- $($1.XLIBS) -- $(CXXFLAGS) $($1.CXXFLAGS) $(CPPFLAGS) $($1.CPPFLAGS) $(LDFLAGS) $($1.LDFLAGS) -- $(_CHMOD))
  $(call make_sig_target,$1,$2,compiler,$(CFLAGS) $($1.CFLAGS) -- $(CXXFLAGS) $($1.CXXFLAGS) -- $(CPPFLAGS) $($1.CPPFLAGS))

  $(if $(PCH),,$(eval $1.NOPCH := 1))

  $(call make_bin_targets,$1,$2,bin,d,o,$1,$($1.DESTDIR),gcda,gcno)

  $(foreach s,$(filter %.c,$($1.SRCS)),$(call make_o_d,$s,$2,c,$(CC),$(CFLAGS) $($1.CFLAGS) $(CPPFLAGS) $($1.CPPFLAGS),d,o,$1,CC))
  $(foreach s,$(filter %.cpp,$($1.SRCS)),$(call make_o_d,$s,$2,cpp,$(CXX),$(CXXFLAGS) $($1.CXXFLAGS) $(CPPFLAGS) $($1.CPPFLAGS),d,o,$1,CXX,$(if $($1.NOPCH),,0)))

  $(call make_dir_target,$(INSTALL_ROOT)/$($1.DESTDIR))
  $(INSTALL_ROOT)/$($1.DESTDIR)/$1: $(addprefix $(BUILD_ROOT)$2/$1/,$(addsuffix .o,$(_S))) $3 $(dirmark) $(BUILD_ROOT)$2/$1/linker.sig $$(EXTRA_EXE_$1)
	@echo '[$2/$1] linking '
	@echo '$(CXX) -o $$@ $$(filter %.o,$$^) $(CXXFLAGS) $($1.CXXFLAGS) $(CPPFLAGS) $($1.CPPFLAGS) $(LDFLAGS) $($1.LDFLAGS)  \
	    $$(addprefix -L,$$(LIB_PATH)) $$(addprefix -l,$$(EXE_ALL_LIBS_$1)) $$(addprefix -l,$$(EXE_ALL_XLIBS_$1))'
	@$(CXX) -o $$@ $$(filter %.o,$$^) $(CXXFLAGS) $($1.CXXFLAGS) $(CPPFLAGS) $($1.CPPFLAGS) $(LDFLAGS) $($1.LDFLAGS) $(SCTPOBJFLAGS) ${SCTP_LIBS}\
	    $$(addprefix -L,$$(LIB_PATH)) $$(addprefix -l,$$(EXE_ALL_LIBS_$1)) $$(addprefix -l,$$(EXE_ALL_XLIBS_$1))
	@chmod $(_CHMOD) $$@

  TARGET_D_DEPS_$(INSTALL_ROOT)/$($1.DESTDIR)/$1 := $(addprefix $(BUILD_ROOT)$2/$1/,$(addsuffix .o,$(_S))) $3 $$(EXTRA_EXE_$1)

  $(eval OUR_EXES += $1)
  $(eval EXE_IS_KNOWN_$1 := $2/$1)
  $(eval EXE_LIBS_$1 := $($1.LIBS))
  $(eval EXE_XLIBS_$1 := $($1.XLIBS))
endef

################################################################################

target_type_lib_required_vars := $(bin_target_required_vars)
target_type_lib_optional_vars := $(bin_target_optional_vars)

define process_lib_target
  $(call check_target_uniq,$1,$2,lib$1.a)
  $(call check_target_uniq,$1,$2,$($1.DESTDIR)/lib$1.a)
  $(call check_sources_types,$1,$2)

  $(call make_sig_target,$1,$2,linker,$($1.SRCS) -- $($1.LIBS) -- $($1.XLIBS) -- $(CXXFLAGS) $($1.CXXFLAGS) $(CPPFLAGS) $($1.CPPFLAGS) $(LDFLAGS) $($1.LDFLAGS))
  $(call make_sig_target,$1,$2,compiler,$(CFLAGS) $($1.CFLAGS) -- $(CXXFLAGS) $($1.CXXFLAGS) -- $(CPPFLAGS) $($1.CPPFLAGS))

  $(if $(PCH),,$(eval $1.NOPCH := 1))

  $(call make_bin_targets,$1,$2,lib,d,o,lib$1.a,$($1.DESTDIR),gcda,gcno)

  $(foreach s,$(filter %.c,$($1.SRCS)),$(call make_o_d,$s,$2,c,$(CC),$(CFLAGS) $($1.CFLAGS) $(CPPFLAGS) $($1.CPPFLAGS),d,o,$1,CC))
  $(foreach s,$(filter %.cpp,$($1.SRCS)),$(call make_o_d,$s,$2,cpp,$(CXX),$(CXXFLAGS) $($1.CXXFLAGS) $(CPPFLAGS) $($1.CPPFLAGS),d,o,$1,CXX,$(if $($1.NOPCH),,0)))

  $(call make_dir_target,$(INSTALL_ROOT)/$($1.DESTDIR))
  $(INSTALL_ROOT)/$($1.DESTDIR)/lib$1.a: $(addprefix $(BUILD_ROOT)$2/$1/,$(addsuffix .o,$(_S))) $3 $(dirmark) $(BUILD_ROOT)$2/$1/linker.sig
	@rm -f $$@
	@echo '[$2/$1] making static library'
	@$(AR) rcs $$@ $$(filter %.o,$$^)

  TARGET_D_DEPS_$(INSTALL_ROOT)/$($1.DESTDIR)/lib$1.a := $(addprefix $(BUILD_ROOT)$2/$1/,$(addsuffix .o,$(_S))) $3

  $(call make_lib_install_targets,$1,$2,lib$1.a)
  $(call make_testlibs_target,$1,$2)

  $(eval LIB_IS_KNOWN_$1 := $2/$1)
  $(eval LIB_LIBS_$1 := $($1.LIBS))
  $(eval LIB_XLIBS_$1 := $($1.XLIBS))
  $(eval LIB_FILE_$1 := $(INSTALL_ROOT)/$($1.DESTDIR)/lib$1.a)
endef

################################################################################

target_type_so_required_vars := $(bin_target_required_vars)
target_type_so_optional_vars := $(bin_target_optional_vars)

define process_so_target
  $(call check_target_uniq,$1,$2,lib$1.so)
  $(call check_target_uniq,$1,$2,$($1.DESTDIR)/lib$1.so)
  $(call compute_chmod,$1,$2,$($1.DESTDIR))
  $(call check_sources_types,$1,$2)

  $(call make_sig_target,$1,$2,linker,$($1.SRCS) -- $($1.LIBS) -- $($1.XLIBS) -- $(CXXFLAGS) $($1.CXXFLAGS) $(CPPFLAGS) $($1.CPPFLAGS) $(LDFLAGS) $($1.LDFLAGS) -- $(_CHMOD))
  $(call make_sig_target,$1,$2,compiler,$(CFLAGS) $($1.CFLAGS) -- $(CXXFLAGS) $($1.CXXFLAGS) -- $(CPPFLAGS) $($1.CPPFLAGS))

  $(if $(PCH),,$(eval $1.NOPCH := 1))

  $(call make_bin_targets,$1,$2,lib,ds,os,lib$1.so,$($1.DESTDIR),gcda,gcno)

  $(foreach s,$(filter %.c,$($1.SRCS)),$(call make_o_d,$s,$2,c,$(CC),$(CFLAGS) $($1.CFLAGS) $(CPPFLAGS) $($1.CPPFLAGS) $(SO_FLAGS),ds,os,$1,CC))
  $(foreach s,$(filter %.cpp,$($1.SRCS)),$(call make_o_d,$s,$2,cpp,$(CXX),$(CXXFLAGS) $($1.CXXFLAGS) $(CPPFLAGS) $($1.CPPFLAGS) $(SO_FLAGS),ds,os,$1,CXX,$(if $($1.NOPCH),,1)))

  $(call make_dir_target,$(INSTALL_ROOT)/$($1.DESTDIR))
  $(INSTALL_ROOT)/$($1.DESTDIR)/lib$1.so: $(addprefix $(BUILD_ROOT)$2/$1/,$(addsuffix .os,$(_S))) $3 $(dirmark) $(BUILD_ROOT)$2/$1/linker.sig
	@echo '[$2/$1] linking shared library'
	@$(CXX) -shared -o $$@ $(CXXFLAGS) $($1.CXXFLAGS) $(CPPFLAGS) $($1.CPPFLAGS) $(LDFLAGS) $($1.LDFLAGS) $(SO_FLAGS) $$(filter %.os,$$^)
	@chmod $(_CHMOD) $$@

  TARGET_D_DEPS_$(INSTALL_ROOT)/$($1.DESTDIR)/lib$1.so := $(addprefix $(BUILD_ROOT)$2/$1/,$(addsuffix .os,$(_S))) $3

  $(call make_lib_install_targets,$1,$2,lib$1.so)
  $(call make_testlibs_target,$1,$2)

  $(eval LIB_IS_KNOWN_$1 := $2/$1)
  $(eval LIB_LIBS_$1 := $($1.LIBS))
  $(eval LIB_XLIBS_$1 := $($1.XLIBS))
  $(eval LIB_FILE_$1 := $(INSTALL_ROOT)/$($1.DESTDIR)/lib$1.so)
endef

################################################################################

target_type_lib2_required_vars := $(bin_target_required_vars)
target_type_lib2_optional_vars := $(bin_target_optional_vars)

define process_lib2_target
  $(call process_lib_target,$1,$2,$3)
  $(call process_so_target,$1,$2,$3)
endef

################################################################################

target_type_external_lib_required_vars := FILE MAKE
target_type_external_lib_optional_vars := LIBS XLIBS DESTDIR
target_type_external_lib_KW := external

define process_external_lib_target
  $(call check_target_uniq,$1,$2,$(notdir $($1.FILE)))
  $(if $($1.DESTDIR),
    $(call check_target_uniq,$1,$2,$($1.DESTDIR)/$(notdir $($1.FILE)))
    $(call compute_chmod,$1,$2,$($1.DESTDIR)),
    $(eval _CHMOD :=)
  )

  $(call make_sig_target,$1,$2,a,$($1.MAKE) -- $(_CHMOD))

  $(eval LIB_IS_KNOWN_$1 := $2/$1)
  $(eval LIB_LIBS_$1 := $($1.LIBS))
  $(eval LIB_XLIBS_$1 := $($1.XLIBS))
  $(eval LIB_FILE_$1 := $($1.FILE))

  $(if $(filter $(dir $($1.FILE)),$(LIB_PATH)),,$(eval LIB_PATH += $(dir $($1.FILE))))

  $($1.FILE): $(if $(FORCE_PREREQ),FORCE) $3 $(BUILD_ROOT)$2/$1/a.sig
	@echo '[$2/$1] making external library'
	@$($1.MAKE)

  $(if $($1.DESTDIR),
    $(call make_dir_target,$(INSTALL_ROOT)/$($1.DESTDIR))
    $(INSTALL_ROOT)/$($1.DESTDIR)/$(notdir $($1.FILE)): $(dirmark) $($1.FILE)
	@cp --force --update $($1.FILE) $(INSTALL_ROOT)/$($1.DESTDIR) && chmod $(_CHMOD) $$@
    $2/$1/clean::
	@rm -f $(INSTALL_ROOT)/$($1.DESTDIR)/$(notdir $($1.FILE))
  )

  $2/$1/all: $($1.FILE)

  TARGET_D_DEPS_$($1.FILE) := $3
  TARGET_D_DEPS_$2/$1/all := $($1.FILE)

  $(call make_testlibs_target,$1,$2)
endef

################################################################################

target_type_file_required_vars := SRC DEST

define process_file_target
  $(call check_target_uniq,$1,$2,$($1.DEST))
  $(call compute_chmod,$1,$2,$(patsubst %/,%,$(dir $($1.DEST))))

  $(call make_sig_target,$1,$2,a,$($1.SRC) -- $(_CHMOD))

  $(call make_dir_target,$(dir $(INSTALL_ROOT)/$($1.DEST)))
  $(INSTALL_ROOT)/$($1.DEST): $(MAKE_ROOT)$2/$($1.SRC) $(dirmark) $3 $(BUILD_ROOT)$2/$1/a.sig
	@echo '[$2/$1] copying file $($1.SRC) to $($1.DEST)'
	@cp --force $$< $$@ && chmod $(_CHMOD) $$@

  $2/$1/all: $(INSTALL_ROOT)/$($1.DEST) 
  $2/$1/clean::
	@echo '[$2/$1] cleaning file $($1.DEST)'
	@-rm -f $(INSTALL_ROOT)/$($1.DEST)

  TARGET_D_DEPS_$(INSTALL_ROOT)/$($1.DEST) := $3
  TARGET_D_DEPS_$2/$1/all := $(INSTALL_ROOT)/$($1.DEST)

endef

################################################################################

target_type_files_required_vars := FILES DESTDIR
target_type_files_optional_vars := SRCDIR

define process_files_target
  $(foreach s,$($1.FILES),$(call check_target_uniq,$1,$2,$($1.DESTDIR)/$s))
  $(call compute_chmod,$1,$2,$($1.DESTDIR))

  $(call make_sig_target,$1,$2,a,$(_CHMOD))

  $(call make_dir_target,$(INSTALL_ROOT)/$($1.DESTDIR))
  $(foreach s,$($1.FILES),
    $(INSTALL_ROOT)/$($1.DESTDIR)/$s: $3 $($1.SRCDIR)/$s $(dirmark) $(BUILD_ROOT)$2/$1/a.sig
	@echo '[$2/$1] copying file $s to $($1.DESTDIR)'
	@cp --force $($1.SRCDIR)/$s $$@ && chmod $(_CHMOD) $$@
    TARGET_D_DEPS_$(INSTALL_ROOT)/$($1.DESTDIR)/$s := $3
  )

  $2/$1/all: $(addprefix $(INSTALL_ROOT)/$($1.DESTDIR)/,$($1.FILES))
  $2/$1/clean::
	@echo '[$2/$1] cleaning'
	@-rm -f $(addprefix $(INSTALL_ROOT)/$($1.DESTDIR)/,$($1.FILES))

  TARGET_D_DEPS_$(INSTALL_ROOT)/$($1.DEST) := $(addprefix $(INSTALL_ROOT)/$($1.DESTDIR)/,$($1.FILES))

endef

################################################################################

target_type_custom_optional_vars := EXTRA

define process_custom_target
  $(foreach s,$(filter-out $(TARGET_SUFFIXES),$($1.EXTRA)),
    .PHONY: $2/$1/$s
    ALL_TARGETS += $2/$1/$s
  )
endef

################################################################################

target_type_podman_required_vars := NAME 
target_type_podman_optional_vars := SECTION DESTDIR SRCDIR
target_type_podman_extra_subtargets := show

define process_podman_target
  $(if $($1.SECTION),,$(eval $1.SECTION := 1))
  $(eval _ND := $($1.DESTDIR)/man$($1.SECTION))
  $(eval _N := $(_ND)/$($1.NAME).$($1.SECTION).gz)
  $(call check_target_uniq,$1,$2,$(_N))
  $(call compute_chmod,$1,$2,$(_ND))

  $(call make_sig_target,$1,$2,a,$(_CHMOD))

  $(call make_dir_target,$(INSTALL_ROOT)/$(_ND))
  $(INSTALL_ROOT)/$(_N): $($1.SRCDIR)/$($1.NAME).pod $(dirmark) $3 $(BUILD_ROOT)$2/$1/a.sig
	@echo '[$2/$1] generating man page'
	@pod2man $(POD2MAN_FLAGS) --name "$($1.NAME)" --section $($1.SECTION) $$< | gzip > $$@ && chmod $(_CHMOD) $$@

  $2/$1/all: $(INSTALL_ROOT)/$(_N)

  $2/$1/clean::
	@echo '[$2/$1] cleaning'
	@-rm -f $(INSTALL_ROOT)/$(_N)
  $2/$1/show: $(INSTALL_ROOT)/$(_N)
	@less $$^

  TARGET_D_DEPS_$(INSTALL_ROOT)/$(_N) := $3
  TARGET_D_DEPS_$2/$1/all := $(INSTALL_ROOT)/$(_N)
  TARGET_D_DEPS_$2/$1/show := $(INSTALL_ROOT)/$(_N)

endef

#### end of target type specific code ####

#####
# $1 short target name
# $2 directory relative to MAKE_ROOT
# $3 key
#
# Side-effects: local var _n

define check_target_uniq
  $(eval _n := unique_target_is_defined_$(subst /,_,$(subst _,__,$3)))
  $(if $($(_n)),
    $(error Target '$2/$1' produces '$3' which is already produced by '$($(_n))'),
    $(eval $(_n) := $2/$1)
  )
endef

#####
# $1 variable name
#
# returns variable name if defined (and not = $(undef)), or blank otherwise

define is_var_defined
$(if $(findstring undefined,$(origin $1)),,$(if $(findstring $$(undef),$(value $1)),,$1))
endef

#####
# $1 short target name
# $2 directory relative to MAKE_ROOT
# $3 destination directory ($1.DESTDIR or $(dir $1.DEST))
#
# Sets _CHMOD variable to computed value 

define compute_chmod
$(eval _CHMOD := $(if $($1.CHMOD),$($1.CHMOD),$(if $(CHMOD_TYPE_$($1.TYPE)),$(CHMOD_TYPE_$($1.TYPE)),$(if $(CHMOD_DIR_$3),$(CHMOD_DIR_$3),$(error File permissions are not defined for target $2/$1. Please set them either as taget specific variable "$1.CHMOD" in $(MAKE_ROOT)$2/user.mk, or per destination directory variable "CHMOD_DIR_$3" in $(MAKE_ROOT)/flags.mk)))))
endef

TARGET_SUFFIXES := all clean install uninstall

#####
# $1 short target name
# $2 directory relative to MAKE_ROOT

define process_target
  $(foreach v,$(filter $1.%,$(.VARIABLES)),$(if $(call is_var_defined,$v),$(eval $v = $(strip $($v)))))
  $(if $($1.TYPE),,$(error No target type defined for '$2/$1'))
  $(if $(value process_$($1.TYPE)_target),,$(error Unknown target type '$($1.TYPE)' specified for target '$2/$1'))

  $(eval _V_EXTRA := $(filter-out TYPE DEPS LDEPS KW CHMOD _% $(target_type_$($1.TYPE)_required_vars) $(target_type_$($1.TYPE)_optional_vars),$(patsubst $1.%,%,$(strip $(foreach v,$(filter $1.%,$(.VARIABLES)),$(call is_var_defined,$v))))))
  $(if $(_V_EXTRA),$(warning Ignoring unknown variable(s) '$(_V_EXTRA)' specified for target '$2/$1' of type '$($1.TYPE)'))
  $(foreach v,$(target_type_$($1.TYPE)_required_vars),$(if $(call is_var_defined,$1.$v),,$(error Missing required variable $v for target '$2/$1' of type '$($1.TYPE)')))

  $(if $($1.DESTDIR),,$(eval $1.DESTDIR := $($($1.TYPE)_DESTDIR)))
  $(if $($1.SRCDIR),
    $(if $(patsubst /%,,$($1.SRCDIR)),$(eval $1.SRCDIR := $(MAKE_ROOT)$2/$($1.SRCDIR))),
    $(eval $1.SRCDIR := $(MAKE_ROOT)$2$(if $(call is_var_defined,$1.SRCDIR),$($1.SRCDIR),$($($1.TYPE)_SRCDIR)))
  )

  $(call process_$($1.TYPE)_target,$1,$2,$($1.DEPS) $(addprefix $2/,$($1.LDEPS)))

  $(if $($1.KW),,$(eval $1.KW := $(if $(target_type_$($1.TYPE)_KW),$(target_type_$($1.TYPE)_KW),default)))

  $2/$1: $2/$1/all
  .PHONY: $2/$1
  ALL_TARGETS += $2/$1

  $(eval TARGET_D_DEPS_$2/$1 += $2/$1/all)

  $(if $(filter $(KW),$($1.KW)),
    $(foreach s,$(TARGET_SUFFIXES),
      $2/$s: $2/$1/$s
    )
    TARGET_D_DEPS_$2/all += $2/$1/all
  )

  $(foreach s,$(TARGET_SUFFIXES) $(target_type_$($1.TYPE)_extra_subtargets),
  .PHONY: $2/$1/$s
  ALL_TARGETS += $2/$1/$s
  )

  $(foreach v,$(filter $1.%,$(.VARIABLES)),$(eval $v = $$(undef)))
endef

# $(THIS_DIR) can be used by "native" targets

define process_dir
  $(eval SUB_DIRS :=)
  $(eval TARGETS :=)
  $(eval THIS_DIR := $1)
  $(eval include $(MAKE_ROOT)$1/user.mk)
  $(if $(if $(SUB_DIRS),$(SUB_DIRS),$(TARGETS)),,$(error Neither SUB_DIRS nor TARGETS is defined in $1))
  $(if $(TARGETS),$(foreach t,$(TARGETS),$(call process_target,$t,$1)))

  $(foreach s,$(TARGET_SUFFIXES),
    ALL_TARGETS += $1/$s
    .PHONY: $1/$s
    $1/$s: $(addprefix $1/,$(addsuffix /$s,$(SUB_DIRS)))
  )

  TARGET_D_DEPS_$1/all += $(addprefix $1/,$(addsuffix /all,$(SUB_DIRS)))

  $(if $1,
    .PHONY: $1
    $1: $1/all
    ALL_TARGETS += $1

    TARGET_D_DEPS_$1 += $1/all
  )

  $(if $(SUB_DIRS),$(foreach d,$(SUB_DIRS),$(call process_dir,$1/$d)))
endef

BASE_MAKEFILES := $(MAKEFILE_LIST)
LOCAL := $(patsubst $(MAKE_ROOT)%,%,$(CURDIR))

define add_local_aliases
$(if $(filter $(LOCAL)/%,$(ALL_TARGETS)),,$(error '$(LOCAL)' is not in build system))
$(foreach t,$(patsubst $(LOCAL)/%,%,$(filter $(LOCAL)/%,$(ALL_TARGETS))),
  $t: $(LOCAL)/$t
  .PHONY: $t
  TARGET_D_DEPS_$t += $(LOCAL)/$t
)
endef

# this is for compatibility with other build systems which expect "stage" target
stage: all install

cleanall:
	rm -rf $(BUILD_ROOT) $(INSTALL_ROOT)

help:
	@cat $(MAKE_ROOT)/Makefile.help

showrpms:
	@echo $(ALL_RPMS)

.PHONY: FORCE stage cleanall help showrpms

### precompiled header related targets ########################################
ifneq ($(PCH),)
  $(eval $(call make_sig_target_prim,$(BUILD_ROOT)/PRECOMPILED.sig,$(strip $(PCH)) -- $(CXXFLAGS) $(CPPFLAGS),$(BASE_MAKEFILES)))
  $(eval $(call make_dir_target,$(BUILD_ROOT)))
  $(foreach i,0 1,$(BUILD_ROOT)/PRECOMPILED$(i).h $(BUILD_ROOT)/PRECOMPILED$(i).h.gch): $(dirmark) $(BUILD_ROOT)/PRECOMPILED.sig
	@echo 'Building precompiled headers: $(PCH)'
	@touch $(BUILD_ROOT)/PRECOMPILED0.h
	@echo -e '$(foreach i,$(PCH),#include $i\n)' | $(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -x c++-header - -o $(BUILD_ROOT)/PRECOMPILED0.h.gch 
	@touch $(BUILD_ROOT)/PRECOMPILED1.h
	@echo -e '$(foreach i,$(PCH),#include $i\n)' | $(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $(SO_FLAGS) -x c++-header - -o $(BUILD_ROOT)/PRECOMPILED1.h.gch 
  # note: a single file mentioned in the next rule is sufficient to kick building of all pch files
  PCH: $(BUILD_ROOT)/PRECOMPILED0.h
  PCH/clean:
	@echo 'Removing precompiled headers file'
	@-rm -f $(BUILD_ROOT)/PRECOMPILED*

  .PHONY: PCH PCH/clean
endif
###############################################################################

ALL_TARGETS :=
OUR_EXES :=
ALL_RPMS :=
KW := default

DELAYED_process_dir := $(call process_dir,)

#####
# $1 name of the variable into which xlibs deps are collected
# $2 name of the variable from which to collect deps
define add_xlibs
  $(foreach l,$($2),$(if $(filter $l,$($1)),,$(eval $1 += $l)))
endef

#####
# $1 name of the variable into which deps are collected
# $2 name of the variable into which xlibs deps are collected
# $3 name of lib to be processed
# $4 context

define make_lib_deps_r
$(if $(LIB_IS_KNOWN_$3),,$(error Unknown library $3 is referenced by $4))
$(if $(filter $3,$($1)),,
  $(eval $1 += $3)
  $(foreach l,$(LIB_LIBS_$3),$(call make_lib_deps_r,$1,$2,$l,$(LIB_IS_KNOWN_$3)))
  $(call add_xlibs,$2,LIB_XLIBS_$3)
)
endef

#####
# $1 name of executable

define make_exe_deps
  $(foreach l,$(EXE_LIBS_$1),$(call make_lib_deps_r,EXE_ALL_LIBS_$1,EXE_ALL_XLIBS_$1,$l,$(EXE_IS_KNOWN_$1)))
  $(call add_xlibs,EXE_ALL_XLIBS_$1,EXE_XLIBS_$1)
  EXTRA_EXE_$1 := $(foreach l,$(EXE_ALL_LIBS_$1),$(LIB_FILE_$l))
endef

$(eval $(foreach e,$(OUR_EXES),$(call make_exe_deps,$e)))

#####
# $1 name of library 

define make_lib_deps
  $(call make_lib_deps_r,LIB_ALL_LIBS_$1,LIB_ALL_XLIBS_$1,$1,$(LIB_IS_KNOWN_$1))
  $(call add_xlibs,LIB_ALL_XLIBS_$1,LIB_XLIBS_$1)
  EXTRA_LIB_$1 := $(foreach l,$(LIB_ALL_LIBS_$1),$(LIB_FILE_$l))
endef

$(eval $(foreach l,$(patsubst %/testlibs,%,$(filter %/testlibs,$(MAKECMDGOALS))),$(call make_lib_deps,$l)))

$(eval $(DELAYED_process_dir))
$(eval $(call add_local_aliases))

define recurse_target_d
$(if $(filter $1,$(TARGET_D_VISITED)),,
  $(eval TARGET_D_VISITED += $1)
  $(foreach i,$(TARGET_D_DEPS_$1),$(call recurse_target_d,$i))
  $(if $(TARGET_D_INCLUDE_$1),$(eval -include $(TARGET_D_INCLUDE_$1)))
)
endef

$(eval $(foreach g,$(if $(MAKECMDGOALS),$(MAKECMDGOALS),all),$(call recurse_target_d,$g)))

# vim: syntax=make
