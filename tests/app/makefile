include prorab.mk
include prorab-clang-format.mk
include prorab-test.mk

$(eval $(call prorab-include, ../../src/makefile))

$(eval $(call prorab-config, ../../config))

this_name := app

this_srcs := $(call prorab-src-dir, src)

this_libpautina := ../../src/out/$(c)/libpautina$(dot_so)

this_cxxflags += -I ../../src
this_ldflags += -L ../../src/out/$(c)

this_ldlibs += $(this_libpautina) -ltcpserver

$(eval $(prorab-build-app))
$(eval $(call prorab-depend, $(prorab_this_name), $(this_libpautina)))

this_run_name := $(this_name)
this_test_cmd := $(prorab_this_name)
this_test_deps := $(prorab_this_name)
this_test_ld_path := $(dir $(this_libpautina))
$(eval $(prorab-run))

$(eval $(prorab-clang-format))
