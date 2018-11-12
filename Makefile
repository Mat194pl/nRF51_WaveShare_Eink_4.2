PROJECT_NAME := nrf51_eink_test_project

export OUTPUT_FILENAME
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

SDK_ROOT := nRF5_SDK_12.3.0
TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc
PROJ_DIR = .

ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
CXX             := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-c++"
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#C files common to all targets
C_SOURCE_FILES += \
	$(PROJ_DIR)/image_data.c \
	$(PROJ_DIR)/Drivers/eink_ws42/eink_ws42.c \
	$(PROJ_DIR)/Drivers/eink_ws42/eink_ws42_port.c \
	$(SDK_ROOT)/components/ble/ble_advertising/ble_advertising.c \
	$(SDK_ROOT)/components/ble/ble_services/ble_nus/ble_nus.c \
	$(SDK_ROOT)/components/ble/common/ble_advdata.c \
	$(SDK_ROOT)/components/ble/common/ble_conn_params.c \
	$(SDK_ROOT)/components/ble/common/ble_srv_common.c \
	$(SDK_ROOT)/components/boards/boards.c \
	$(SDK_ROOT)/components/drivers_nrf/adc/nrf_drv_adc.c \
	$(SDK_ROOT)/components/drivers_nrf/clock/nrf_drv_clock.c \
	$(SDK_ROOT)/components/drivers_nrf/common/nrf_drv_common.c \
	$(SDK_ROOT)/components/drivers_nrf/gpiote/nrf_drv_gpiote.c \
	$(SDK_ROOT)/components/drivers_nrf/ppi/nrf_drv_ppi.c \
	$(SDK_ROOT)/components/drivers_nrf/spi_master/nrf_drv_spi.c \
	$(SDK_ROOT)/components/drivers_nrf/timer/nrf_drv_timer.c \
	$(SDK_ROOT)/components/drivers_nrf/twi_master/nrf_drv_twi.c \
	$(SDK_ROOT)/components/drivers_nrf/uart/nrf_drv_uart.c \
	$(SDK_ROOT)/components/libraries/fifo/app_fifo.c \
	$(SDK_ROOT)/components/libraries/fstorage/fstorage.c \
	$(SDK_ROOT)/components/libraries/hardfault/hardfault_implementation.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
	$(SDK_ROOT)/components/libraries/pwm/app_pwm.c \
	$(SDK_ROOT)/components/libraries/timer/app_timer.c \
	$(SDK_ROOT)/components/libraries/twi/app_twi.c \
	$(SDK_ROOT)/components/libraries/uart/retarget.c \
	$(SDK_ROOT)/components/libraries/util/app_error.c \
	$(SDK_ROOT)/components/libraries/util/app_error_weak.c \
	$(SDK_ROOT)/components/libraries/util/app_util_platform.c \
	$(SDK_ROOT)/components/libraries/util/nrf_assert.c \
	$(SDK_ROOT)/components/libraries/util/sdk_errors.c \
	$(SDK_ROOT)/components/softdevice/common/softdevice_handler/softdevice_handler.c \
	$(SDK_ROOT)/components/toolchain/system_nrf51.c \
	$(SDK_ROOT)/external/segger_rtt/RTT_Syscalls_GCC.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \

CPP_SOURCE_FILES += \
	$(abspath src/main.cpp) \

#assembly files common to all targets
ASM_SOURCE_FILES += \
	$(SDK_ROOT)/components/toolchain/gcc/gcc_startup_nrf51.s \

#includes common to all targets
INC_PATHS  += \
  -I$(SDK_ROOT)/components \
  -I$(SDK_ROOT)/components/ble/ble_advertising \
  -I$(SDK_ROOT)/components/ble/ble_dtm \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_ancs_c \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_ans_c \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_bas \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_bas_c \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_cscs \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_cts_c \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_dfu \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_dis \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_gls \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_hids \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_hrs_c \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_hts \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_ias \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_ias_c \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_lbs \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_lls \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_nus \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_nus_c \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_rscs_c \
  -I$(SDK_ROOT)/components/ble/ble_services/ble_tps \
  -I$(SDK_ROOT)/components/ble/common \
  -I$(SDK_ROOT)/components/ble/peer_manager \
  -I$(SDK_ROOT)/components/boards \
  -I$(SDK_ROOT)/components/device \
  -I$(SDK_ROOT)/components/drivers_nrf/adc \
  -I$(SDK_ROOT)/components/drivers_nrf/common \
  -I$(SDK_ROOT)/components/drivers_nrf/comp \
  -I$(SDK_ROOT)/components/drivers_nrf/clock \
  -I$(SDK_ROOT)/components/drivers_nrf/delay \
  -I$(SDK_ROOT)/components/drivers_nrf/gpiote \
  -I$(SDK_ROOT)/components/drivers_nrf/hal \
  -I$(SDK_ROOT)/components/drivers_nrf/i2s \
  -I$(SDK_ROOT)/components/drivers_nrf/lpcomp \
  -I$(SDK_ROOT)/components/drivers_nrf/pdm \
  -I$(SDK_ROOT)/components/drivers_nrf/power \
  -I$(SDK_ROOT)/components/drivers_nrf/ppi \
  -I$(SDK_ROOT)/components/drivers_nrf/pwm \
  -I$(SDK_ROOT)/components/drivers_nrf/qdec \
  -I$(SDK_ROOT)/components/drivers_nrf/rng \
  -I$(SDK_ROOT)/components/drivers_nrf/rtc \
  -I$(SDK_ROOT)/components/drivers_nrf/spi_master \
  -I$(SDK_ROOT)/components/drivers_nrf/spi_slave \
  -I$(SDK_ROOT)/components/drivers_nrf/swi \
  -I$(SDK_ROOT)/components/drivers_nrf/timer \
  -I$(SDK_ROOT)/components/drivers_nrf/twi_master \
  -I$(SDK_ROOT)/components/drivers_nrf/twis_slave \
  -I$(SDK_ROOT)/components/drivers_nrf/uart \
  -I$(SDK_ROOT)/components/drivers_nrf/wdt \
  -I$(SDK_ROOT)/components/libraries/crc16 \
  -I$(SDK_ROOT)/components/libraries/crc32 \
  -I$(SDK_ROOT)/components/libraries/csense \
  -I$(SDK_ROOT)/components/libraries/csense_drv \
  -I$(SDK_ROOT)/components/libraries/experimental_section_vars \
  -I$(SDK_ROOT)/components/libraries/fifo \
  -I$(SDK_ROOT)/components/libraries/fstorage \
  -I$(SDK_ROOT)/components/libraries/gpiote \
  -I$(SDK_ROOT)/components/libraries/hardfault \
  -I$(SDK_ROOT)/components/libraries/hci \
  -I$(SDK_ROOT)/components/libraries/led_softblink \
  -I$(SDK_ROOT)/components/libraries/log \
  -I$(SDK_ROOT)/components/libraries/log/src \
  -I$(SDK_ROOT)/components/libraries/low_power_pwm \
  -I$(SDK_ROOT)/components/libraries/mem_manager \
  -I$(SDK_ROOT)/components/libraries/pwm \
  -I$(SDK_ROOT)/components/libraries/queue \
  -I$(SDK_ROOT)/components/libraries/scheduler \
  -I$(SDK_ROOT)/components/libraries/slip \
  -I$(SDK_ROOT)/components/libraries/timer \
  -I$(SDK_ROOT)/components/libraries/twi \
  -I$(SDK_ROOT)/components/libraries/uart \
  -I$(SDK_ROOT)/components/libraries/usbd/class/audio \
  -I$(SDK_ROOT)/components/libraries/usbd/class/cdc \
  -I$(SDK_ROOT)/components/libraries/usbd/class/hid/kbd \
  -I$(SDK_ROOT)/components/libraries/usbd/class/hid/mouse \
  -I$(SDK_ROOT)/components/libraries/usbd/config \
  -I$(SDK_ROOT)/components/libraries/util \
  -I$(SDK_ROOT)/components/softdevice/common/softdevice_handler \
  -I$(SDK_ROOT)/components/softdevice/s130/headers \
  -I$(SDK_ROOT)/components/softdevice/s130/headers/nrf51 \
  -I$(SDK_ROOT)/components/toolchain \
  -I$(SDK_ROOT)/components/toolchain/cmsis/include \
  -I$(SDK_ROOT)/external/segger_rtt \
  -Iconfig \
  -Isrc \
  -IDrivers/eink_ws42
  
OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

# C flags to all targets
CFLAGS += -DBOARD_CUSTOM
CFLAGS += -DNRF51
CFLAGS += -DNRF51822
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DSWI_DISABLE0
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -O0 -g3
CFLAGS += -mfloat-abi=soft
CFLAGS += -DCONFIG_NFCT_PINS_AS_GPIOS
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums 


# CXX Flags
CXXFLAGS += -fno-threadsafe-statics
CXXFLAGS += -fno-rtti
CXXFLAGS += -fno-exceptions
CXXFLAGS += -fno-non-call-exceptions
CXXFLAGS += -fno-use-cxa-atexit


# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DNRF_LOG_USES_UART=0
ASMFLAGS += -DBOARD_CUSTOM
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DNRF51
ASMFLAGS += -DNRF51822
ASMFLAGS += -DS130
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DSWI_DISABLE0
#default target - first one defined
default: clean nrf51822_xxac

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e nrf51_test

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	nrf51822_xxac
	@echo 	flash_softdevice

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
CPP_SOURCE_FILE_NAMES = $(notdir $(CPP_SOURCE_FILES))
ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))

C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
CPP_PATHS = $(call remduplicates, $(dir $(CPP_SOURCE_FILES) ) )
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))

C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )
CPP_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(CPP_SOURCE_FILE_NAMES:.cpp=.o) )
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )	

vpath %.c   $(C_PATHS)
vpath %.cpp $(CPP_PATHS)
vpath %.s   $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(CPP_OBJECTS) $(ASM_OBJECTS)

nrf51822_xxac: OUTPUT_FILENAME := nrf51_test
nrf51822_xxac: LINKER_SCRIPT=linker_script.ld
nrf51822_xxac: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out

	$(NO_ECHO)$(CC) -std=gnu99 $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) -std=gnu99 $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Create objects from Cpp SRC files
$(OBJECT_DIRECTORY)/%.o: %.cpp
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CXX) $(CFLAGS) $(CXXFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) -std=c99 $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<


# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out


## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

flash: $(MAKECMDGOALS)
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/nrf51_test.hex
	tools/nrfjprog/nrfjprog --program $(OUTPUT_BINARY_DIRECTORY)/nrf51_test.hex -f nrf51  --sectorerase
	tools/nrfjprog/nrfjprog --reset
	
erase:
	tools/nrfjprog/nrfjprog --eraseall -f nrf51
