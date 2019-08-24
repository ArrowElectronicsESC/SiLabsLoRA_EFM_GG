################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include ports/source/gnu/subdir.mk
-include ports/source/generic/subdir.mk
-include platform/subdir.mk
-include mbedtls/targets/TARGET_Silicon_Labs/subdir.mk
-include mbedtls/targets/subdir.mk
-include mbedtls/src/subdir.mk
-include mbedtls/platform/src/subdir.mk
-include mbedtls/mbed-crypto/src/subdir.mk
-include mbedtls/mbed-crypto/platform/COMPONENT_PSA_SRV_IMPL/subdir.mk
-include lorawan/system/subdir.mk
-include lorawan/lorastack/phy/subdir.mk
-include lorawan/lorastack/mac/subdir.mk
-include lorawan/subdir.mk
-include lora_rf_drivers/SX126X/subdir.mk
-include kernel/source/subdir.mk
-include events/equeue/subdir.mk
-include events/subdir.mk
-include emlib/subdir.mk
-include cpu/source/subdir.mk
-include common/source/shell/subdir.mk
-include common/source/rtos/subdir.mk
-include common/source/ring_buf/subdir.mk
-include common/source/platform_mgr/subdir.mk
-include common/source/op_lock/subdir.mk
-include common/source/logging/subdir.mk
-include common/source/lib/subdir.mk
-include common/source/kal/subdir.mk
-include common/source/common/subdir.mk
-include common/source/collections/subdir.mk
-include common/source/clk/subdir.mk
-include common/source/auth/subdir.mk
-include bsp/siliconlabs/generic/source/subdir.mk
-include bsp/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/EFM32GG11B/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: LoRa_Stack_Port_CxxProject_3.axf

# Tool invocations
LoRa_Stack_Port_CxxProject_3.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C++ Linker'
	arm-none-eabi-g++ -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "LoRa_Stack_Port_CxxProject_3.ld" --specs=nosys.specs -Xlinker --gc-sections -Xlinker -Map="LoRa_Stack_Port_CxxProject_3.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o LoRa_Stack_Port_CxxProject_3.axf "./CMSIS/EFM32GG11B/startup_gcc_efm32gg11b.o" "./CMSIS/EFM32GG11B/system_efm32gg11b.o" "./Drivers/dmadrv.o" "./Drivers/gpiointerrupt.o" "./Drivers/nvm.o" "./Drivers/nvm_hal.o" "./Drivers/retargetio.o" "./Drivers/rtcdriver.o" "./Drivers/sleep.o" "./Drivers/spidrv.o" "./Drivers/tempdrv.o" "./Drivers/uartdrv.o" "./Drivers/ustimer.o" "./bsp/bsp_bcc.o" "./bsp/bsp_stk.o" "./bsp/bsp_stk_leds.o" "./bsp/bsp_trace.o" "./bsp/siliconlabs/generic/source/bsp_can.o" "./bsp/siliconlabs/generic/source/bsp_cpu.o" "./bsp/siliconlabs/generic/source/bsp_fs_nor_quad_spi.o" "./bsp/siliconlabs/generic/source/bsp_fs_nor_spi.o" "./bsp/siliconlabs/generic/source/bsp_net_ether_gem.o" "./bsp/siliconlabs/generic/source/bsp_os.o" "./bsp/siliconlabs/generic/source/bsp_tick_rtcc.o" "./bsp/siliconlabs/generic/source/bsp_usb_dev.o" "./bsp/siliconlabs/generic/source/bsp_usb_host_pbhcd.o" "./common/source/auth/auth.o" "./common/source/clk/clk.o" "./common/source/clk/clk_cmd.o" "./common/source/collections/bitmap.o" "./common/source/collections/map.o" "./common/source/collections/slist.o" "./common/source/common/common.o" "./common/source/kal/kal_kernel.o" "./common/source/lib/lib_ascii.o" "./common/source/lib/lib_math.o" "./common/source/lib/lib_mem.o" "./common/source/lib/lib_mem_cmd.o" "./common/source/lib/lib_str.o" "./common/source/logging/logging.o" "./common/source/op_lock/op_lock.o" "./common/source/platform_mgr/platform_mgr.o" "./common/source/ring_buf/ring_buf.o" "./common/source/rtos/rtos_err_str.o" "./common/source/rtos/rtos_utils.o" "./common/source/shell/shell.o" "./common/source/shell/shell_cmd.o" "./cpu/source/cpu_core.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_ldma.o" "./emlib/em_rtcc.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./events/EventQueue.o" "./events/mbed_shared_queues.o" "./events/equeue/equeue.o" "./events/equeue/equeue_mbed.o" "./events/equeue/equeue_posix.o" "./kernel/source/os_cfg_app.o" "./kernel/source/os_core.o" "./kernel/source/os_dbg.o" "./kernel/source/os_flag.o" "./kernel/source/os_mem.o" "./kernel/source/os_mon.o" "./kernel/source/os_msg.o" "./kernel/source/os_mutex.o" "./kernel/source/os_prio.o" "./kernel/source/os_q.o" "./kernel/source/os_sem.o" "./kernel/source/os_stat.o" "./kernel/source/os_task.o" "./kernel/source/os_tick.o" "./kernel/source/os_time.o" "./kernel/source/os_tmr.o" "./kernel/source/os_var.o" "./lora_rf_drivers/SX126X/SX126X_LoRaRadio.o" "./lorawan/LoRaWANInterface.o" "./lorawan/LoRaWANStack.o" "./lorawan/lorastack/mac/LoRaMac.o" "./lorawan/lorastack/mac/LoRaMacChannelPlan.o" "./lorawan/lorastack/mac/LoRaMacCommand.o" "./lorawan/lorastack/mac/LoRaMacCrypto.o" "./lorawan/lorastack/phy/LoRaPHY.o" "./lorawan/lorastack/phy/LoRaPHYAS923.o" "./lorawan/lorastack/phy/LoRaPHYAU915.o" "./lorawan/lorastack/phy/LoRaPHYCN470.o" "./lorawan/lorastack/phy/LoRaPHYCN779.o" "./lorawan/lorastack/phy/LoRaPHYEU433.o" "./lorawan/lorastack/phy/LoRaPHYEU868.o" "./lorawan/lorastack/phy/LoRaPHYIN865.o" "./lorawan/lorastack/phy/LoRaPHYKR920.o" "./lorawan/lorastack/phy/LoRaPHYUS915.o" "./lorawan/system/LoRaWANTimer.o" "./mbedtls/mbed-crypto/platform/COMPONENT_PSA_SRV_IMPL/psa_crypto.o" "./mbedtls/mbed-crypto/platform/COMPONENT_PSA_SRV_IMPL/psa_crypto_slot_management.o" "./mbedtls/mbed-crypto/platform/COMPONENT_PSA_SRV_IMPL/psa_crypto_storage.o" "./mbedtls/mbed-crypto/platform/COMPONENT_PSA_SRV_IMPL/psa_its_file.o" "./mbedtls/mbed-crypto/src/aes.o" "./mbedtls/mbed-crypto/src/aesni.o" "./mbedtls/mbed-crypto/src/arc4.o" "./mbedtls/mbed-crypto/src/aria.o" "./mbedtls/mbed-crypto/src/asn1parse.o" "./mbedtls/mbed-crypto/src/asn1write.o" "./mbedtls/mbed-crypto/src/base64.o" "./mbedtls/mbed-crypto/src/bignum.o" "./mbedtls/mbed-crypto/src/blowfish.o" "./mbedtls/mbed-crypto/src/camellia.o" "./mbedtls/mbed-crypto/src/ccm.o" "./mbedtls/mbed-crypto/src/chacha20.o" "./mbedtls/mbed-crypto/src/chachapoly.o" "./mbedtls/mbed-crypto/src/cipher.o" "./mbedtls/mbed-crypto/src/cipher_wrap.o" "./mbedtls/mbed-crypto/src/cmac.o" "./mbedtls/mbed-crypto/src/ctr_drbg.o" "./mbedtls/mbed-crypto/src/des.o" "./mbedtls/mbed-crypto/src/dhm.o" "./mbedtls/mbed-crypto/src/ecdh.o" "./mbedtls/mbed-crypto/src/ecdsa.o" "./mbedtls/mbed-crypto/src/ecjpake.o" "./mbedtls/mbed-crypto/src/ecp.o" "./mbedtls/mbed-crypto/src/ecp_curves.o" "./mbedtls/mbed-crypto/src/entropy.o" "./mbedtls/mbed-crypto/src/entropy_poll.o" "./mbedtls/mbed-crypto/src/gcm.o" "./mbedtls/mbed-crypto/src/havege.o" "./mbedtls/mbed-crypto/src/hkdf.o" "./mbedtls/mbed-crypto/src/hmac_drbg.o" "./mbedtls/mbed-crypto/src/md.o" "./mbedtls/mbed-crypto/src/md2.o" "./mbedtls/mbed-crypto/src/md4.o" "./mbedtls/mbed-crypto/src/md5.o" "./mbedtls/mbed-crypto/src/md_wrap.o" "./mbedtls/mbed-crypto/src/memory_buffer_alloc.o" "./mbedtls/mbed-crypto/src/nist_kw.o" "./mbedtls/mbed-crypto/src/oid.o" "./mbedtls/mbed-crypto/src/padlock.o" "./mbedtls/mbed-crypto/src/pem.o" "./mbedtls/mbed-crypto/src/pk.o" "./mbedtls/mbed-crypto/src/pk_wrap.o" "./mbedtls/mbed-crypto/src/pkcs12.o" "./mbedtls/mbed-crypto/src/pkcs5.o" "./mbedtls/mbed-crypto/src/pkparse.o" "./mbedtls/mbed-crypto/src/pkwrite.o" "./mbedtls/mbed-crypto/src/platform.o" "./mbedtls/mbed-crypto/src/platform_util.o" "./mbedtls/mbed-crypto/src/poly1305.o" "./mbedtls/mbed-crypto/src/ripemd160.o" "./mbedtls/mbed-crypto/src/rsa.o" "./mbedtls/mbed-crypto/src/rsa_internal.o" "./mbedtls/mbed-crypto/src/sha1.o" "./mbedtls/mbed-crypto/src/sha256.o" "./mbedtls/mbed-crypto/src/sha512.o" "./mbedtls/mbed-crypto/src/threading.o" "./mbedtls/mbed-crypto/src/timing.o" "./mbedtls/mbed-crypto/src/xtea.o" "./mbedtls/platform/src/mbed_trng.o" "./mbedtls/platform/src/platform_alt.o" "./mbedtls/src/certs.o" "./mbedtls/src/debug.o" "./mbedtls/src/error.o" "./mbedtls/src/net_sockets.o" "./mbedtls/src/pkcs11.o" "./mbedtls/src/ssl_cache.o" "./mbedtls/src/ssl_ciphersuites.o" "./mbedtls/src/ssl_cli.o" "./mbedtls/src/ssl_cookie.o" "./mbedtls/src/ssl_srv.o" "./mbedtls/src/ssl_ticket.o" "./mbedtls/src/ssl_tls.o" "./mbedtls/src/version.o" "./mbedtls/src/version_features.o" "./mbedtls/src/x509.o" "./mbedtls/src/x509_create.o" "./mbedtls/src/x509_crl.o" "./mbedtls/src/x509_crt.o" "./mbedtls/src/x509_csr.o" "./mbedtls/src/x509write_crt.o" "./mbedtls/src/x509write_csr.o" "./mbedtls/targets/TARGET_Silicon_Labs/aes_aes.o" "./mbedtls/targets/TARGET_Silicon_Labs/crypto_aes.o" "./mbedtls/targets/TARGET_Silicon_Labs/crypto_ecp.o" "./mbedtls/targets/TARGET_Silicon_Labs/crypto_management.o" "./mbedtls/targets/TARGET_Silicon_Labs/crypto_sha.o" "./mbedtls/targets/hash_wrappers.o" "./platform/SPI.o" "./platform/mbed_critical.o" "./ports/source/generic/armv6m_v7m_cpu_int.o" "./ports/source/gnu/armv7m_cpu_a.o" "./ports/source/gnu/armv7m_cpu_c.o" "./ports/source/gnu/armv7m_os_cpu_a.o" "./ports/source/gnu/armv7m_os_cpu_c.o" "./src/main.o" "./src/pg_retargetswo.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: LoRa_Stack_Port_CxxProject_3.hex'
	arm-none-eabi-objcopy -O ihex "LoRa_Stack_Port_CxxProject_3.axf" "LoRa_Stack_Port_CxxProject_3.hex"
	@echo ' '

	@echo 'Building bin file: LoRa_Stack_Port_CxxProject_3.bin'
	arm-none-eabi-objcopy -O binary "LoRa_Stack_Port_CxxProject_3.axf" "LoRa_Stack_Port_CxxProject_3.bin"
	@echo ' '

	@echo 'Building s37 file: LoRa_Stack_Port_CxxProject_3.s37'
	arm-none-eabi-objcopy -O srec "LoRa_Stack_Port_CxxProject_3.axf" "LoRa_Stack_Port_CxxProject_3.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "LoRa_Stack_Port_CxxProject_3.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(C_DEPS)$(CPP_DEPS) LoRa_Stack_Port_CxxProject_3.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
