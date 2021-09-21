BUILTINDAC_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(BUILTINDAC_MOD_DIR)/builtindac.c

# We can add our module folder to include paths if needed
# This is not actually needed in this example.
CFLAGS_USERMOD += -I$(BUILTINDAC_MOD_DIR)
CBUILTINDAC_MOD_DIR := $(USERMOD_DIR)
