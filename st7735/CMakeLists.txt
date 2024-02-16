file(GLOB FILES *.c *.h)
add_library(st7735 ${FILES})
target_link_libraries(st7735
	pico_stdlib
	hardware_spi
	)
target_include_directories(st7735 PUBLIC ./)
