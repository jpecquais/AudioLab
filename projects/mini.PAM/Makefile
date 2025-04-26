# Project Name
TARGET = miniPAM

# Sources
CPP_SOURCES = miniPAM.cpp 
FAUST_SOURCE = dsp/miniPAM.dsp

# # Generated Header from Faust
# FAUST_HEADER = sources/miniPAM.h

# # --- Faust Transpilation Rule ---
# # This rule tells make how to generate the C++ header from the Faust source.
# # It will run if $(FAUST_HEADER) doesn't exist or if $(FAUST_SOURCE) is newer.
# $(FAUST_HEADER): $(FAUST_SOURCE)
# 	@echo "Generating C++ header from Faust..."
# 	@mkdir -p $(dir $@) # Ensure the output directory exists (e.g., sources/)
# 	@faust -lang cpp -double -a ../../faust/tools/faustMinimalInlined.h --class-name MiniPAM -o $@ $< # Transpile: faust -lang cpp -double -o sources/miniPAM.h dsp/miniPAM.dsp
# 	@echo "Generated $@"

# Library Locations
LIBDAISY_DIR = ../../third-party/daisy/libDaisy
DAISYSP_DIR = ../../third-party/daisy/DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

