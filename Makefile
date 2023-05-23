OUTPUT_DIR   := Debug
OUTPUT_FILE  := ${OUTPUT_DIR}/emcu
SIM_LIB      := ${OUTPUT_DIR}/libturbinesim.so
PICSIM_LIB   := ${OUTPUT_DIR}/libpicsim.so
C_SIM_SRC    := src/sim/windsim.c src/sim/batsim.c src/sim/turbinesim.c src/sim/brakesim.c
C_PICSIM_SRC := src/sim/picsim/iosim.c src/sim/picsim/adcsim.c src/sim/picsim/pwmsim.c
C_DRV_SRC    := src/drivers/io.c src/drivers/mem.c src/drivers/timer.c src/drivers/adc.c src/drivers/pwm.c
C_SRC        := src/main.c src/turbine_control.c


SOURCES    := ${C_DRV_SRC} ${C_SRC}
SIM_SRC    := ${C_SIM_SRC}
PICSIM_SRC := ${C_PICSIM_SRC}

C_OBJS        := $(SOURCES:%.c=${OUTPUT_DIR}/%.o)
C_SIM_OBJS    := $(SIM_SRC:%.c=${OUTPUT_DIR}/%.o)
C_PICSIM_OBJS := $(PICSIM_SRC:%.c=${OUTPUT_DIR}/%.o)

CCOMPILER := gcc
CFLAGS := -c -O0 -g -Wall -Werror -ggdb -c -I"./src"

# compile
$(C_SIM_OBJS): $(OUTPUT_DIR)/%.o : %.c
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) -fPIC $(CFLAGS) -o "$@" "$<"

$(C_PICSIM_OBJS): $(OUTPUT_DIR)/%.o : %.c
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) -fPIC $(CFLAGS) -o "$@" "$<"

$(C_OBJS): $(OUTPUT_DIR)/%.o : %.c
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) $(CFLAGS) -o "$@" "$<"

# link

all:	clean $(SIM_LIB) $(PICSIM_LIB) $(OUTPUT_FILE)

$(SIM_LIB): $(C_SIM_OBJS)
	@echo Linking $@
	@$(CCOMPILER) -shared $(C_SIM_OBJS) -o $(SIM_LIB)

$(PICSIM_LIB):	$(C_PICSIM_OBJS)
	@echo Linking $@
	@$(CCOMPILER) -shared $(C_PICSIM_OBJS) -o $(PICSIM_LIB)

$(OUTPUT_FILE): $(C_OBJS)
	@echo Linking: $@
	@$(CCOMPILER) $(C_OBJS) -L$(OUTPUT_DIR) -lpicsim -lturbinesim -o "$(OUTPUT_FILE)"
	@export LD_LIBRARY_PATH=$(pwd)/$(OUTPUT_DIR)

.PHONY: clean
clean: 
	@echo Cleaning Debug
	@rm -rf $(OUTPUT_DIR)



