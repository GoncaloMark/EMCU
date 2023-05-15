OUTPUT_DIR := Debug
OUTPUT_FILE := ${OUTPUT_DIR}/emcu

C_SRC := src/main.c src/io.c src/mem.c src/timer.c src/adc.c src/avg_filter.c
C_SIM_SRC := src/iosim.c src/adcsim.c src/windsim.c src/batsim.c src/turbinesim.c

SOURCES := ${C_SIM_SRC} ${C_SRC}

C_OBJS := $(SOURCES:%.c=${OUTPUT_DIR}/%.o)

CCOMPILER := gcc
CFLAGS := -c -O0 -g -Wall -Werror -ggdb -c -I"./src/include"

# compile

$(C_OBJS): $(OUTPUT_DIR)/%.o : %.c
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) $(CFLAGS) -o "$@" "$<"

# link

all:	clean $(OUTPUT_FILE)

$(OUTPUT_FILE) : $(C_OBJS)
	@echo Linking: $@
	@$(CCOMPILER) -o "$(OUTPUT_FILE)" $(C_OBJS)

.PHONY: clean
clean: 
	@echo Cleaning Debug
	@rm -rf $(OUTPUT_DIR)



