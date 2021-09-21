# This top-level micropython.cmake is responsible for listing
# the individual modules we want to include.
# Paths are absolute, and ${CMAKE_CURRENT_LIST_DIR} can be
# used to prefix subdirectories.

# https://micropython-docs-ja.readthedocs.io/ja/latest/develop/cmodules.html
# -------------
# -- folders --
# -------------
# projects/
#   +esp32-i2s-dac-builtin-tone
#      +lib
#      +mpyusermod
#      -micropython.cmake
#   +micropython
#      +mpy-cross
#      +ports
#         +esp32
#
# ------------------
# -- how to build --
# ------------------
# cd micropython
# make -C mpy-cross
# cd ports/esp32
# make submodules
# make USER_C_MODULES=../../../../esp32-i2s-dac-builtin-tone/micropython.cmake CFLAGS_EXTRA=-DMODULE_BUILTINDAC_ENABLED=1
# 
# -----------
# -- flash --
# -----------
# esptool.py --chip esp32 --port COM3 write_flash -z 0x1000 firmware.bin
#
# ----------------
# -- how to use --
# ----------------
# >>> import cexample
# >>> print(cexample.add_ints(1,2))
# 3
# 

# Add the C example.
include(${CMAKE_CURRENT_LIST_DIR}/mpyusercmodule/micropython.cmake)
