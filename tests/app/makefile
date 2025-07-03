include prorab.mk
include prorab-clang-format.mk
include prorab-test.mk

$(eval $(call prorab-include, ../../src/makefile))

$(eval $(call prorab-config, ../../config))

this_name := app

this_srcs := $(call prorab-src-dir, src)

this_cxxflags += -isystem ../../src

this_ldlibs += -l tcpserver$(this_dbg)
this_ldlibs += -l nitki$(this_dbg)

this_ldlibs += ../../src/out/$(c)/libpautina$(this_dbg)$(dot_so)

$(eval $(prorab-build-app))

this_run_name := $(this_name)
this_test_cmd := $(prorab_this_name)
this_test_deps := $(prorab_this_name)
this_test_ld_path := ../../src/out/$(c)
$(eval $(prorab-run))

$(eval $(prorab-clang-format))
