// Include MicroPython API.
#include "py/runtime.h"

#include "../lib/i2sdacbuiltintone.h"

STATIC mp_obj_t builtindac_init() {
    i2sdacbuiltin_task_init();
    return mp_const_none;
}
// Define a Python reference to the function above.
STATIC MP_DEFINE_CONST_FUN_OBJ_0(builtindac_init_obj, builtindac_init);

STATIC mp_obj_t builtindac_deinit() {
    i2sdacbuiltin_task_deinit();
    return mp_const_none;
}
// Define a Python reference to the function above.
STATIC MP_DEFINE_CONST_FUN_OBJ_0(builtindac_deinit_obj, builtindac_deinit);

STATIC mp_obj_t builtindac_tone(mp_obj_t tone, mp_obj_t msec) {
    i2sdacbuiltin_task_tone(mp_obj_get_int(tone), mp_obj_get_int(msec));
    return mp_const_none;
}
// Define a Python reference to the function above.
STATIC MP_DEFINE_CONST_FUN_OBJ_2(builtindac_tone_obj, builtindac_tone);

// Define all properties of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t builtindac_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_builtindac) },
    { MP_ROM_QSTR(MP_QSTR_init),     MP_ROM_PTR(&builtindac_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_deinit),   MP_ROM_PTR(&builtindac_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR_tone),     MP_ROM_PTR(&builtindac_tone_obj) },
};
STATIC MP_DEFINE_CONST_DICT(builtindac_module_globals, builtindac_module_globals_table);

// Define module object.
const mp_obj_module_t builtindac_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&builtindac_module_globals,
};

// Register the module to make it available in Python.
// Note: the "1" in the third argument means this module is always enabled.
// This "1" can be optionally replaced with a macro like MODULE_CEXAMPLE_ENABLED
// which can then be used to conditionally enable this module.
MP_REGISTER_MODULE(MP_QSTR_builtindac, builtindac_user_cmodule, 1);
