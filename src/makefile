include prorab.mk
include prorab-license.mk
include prorab-clang-format.mk
include prorab-install-dbgsrc.mk

$(eval $(call prorab-config, ../config))

this_name := pautina

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir, .)

this_ldlibs += -ltcpserver -lurlmodel -lhttpmodel -lutki -lnitki -lsetka

$(eval $(prorab-build-lib))

this_src_dir := $(this_name)
$(eval $(prorab-clang-format))

$(eval $(prorab-install-dbgsrc))

this_license_file := ../LICENSE
$(eval $(prorab-license))
