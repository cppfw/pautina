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

this_ldlibs += $(this_libpautina) -lsetka

$(eval $(prorab-build-app))
$(eval $(call prorab-depend, $(prorab_this_name), $(this_libpautina)))

$(eval $(prorab-clang-format))
