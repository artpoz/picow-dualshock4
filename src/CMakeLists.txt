add_executable(picow_dualshock4
	main.cpp
	ds4.cpp
	bt_hid.cpp
	scan.cpp
	arp.cpp
	#fs.cpp
)

pico_enable_stdio_uart(picow_dualshock4 0)
pico_enable_stdio_usb(picow_dualshock4 1)
# pico_enable_stdio_semihosting(picow_dualshock4 0)

target_include_directories(picow_dualshock4 PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)

# add_subdirectory(../lib/pico-extras pico-extras)
target_link_libraries(picow_dualshock4
	hardware_pwm
	hardware_gpio
	pico_stdlib
	pico_cyw43_arch_none
        pico_btstack_classic
        pico_btstack_ble
        pico_btstack_cyw43
	pico_multicore
	hardware_flash	
)

pico_add_extra_outputs(picow_dualshock4)
