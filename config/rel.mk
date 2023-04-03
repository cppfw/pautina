include $(config_dir)base/base.mk

this_cxxflags += -O3

this_lint_cmd = $(prorab_lint_cmd_clang_tidy)
