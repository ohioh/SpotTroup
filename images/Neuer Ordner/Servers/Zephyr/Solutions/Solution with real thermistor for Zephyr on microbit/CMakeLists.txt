cmake_minimum_required(VERSION 3.13.1)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(bdsk)

zephyr_include_directories_ifdef(CONFIG_BOARD_BBC_MICROBIT
   $ENV{ZEPHYR_BASE}/boards/arm/bbc_microbit)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE
  ${app_sources}
  src/services/link_loss.c
  src/services/immediate_alert.c
  src/services/tx_power.c
  src/services/proximity_monitoring.c
  src/services/health_thermometer.c
  src/hardware/lcd/lcd.c
  src/hardware/led/led.c
  src/hardware/thermistor/thermistor.c
  src/hardware/buzzer/buzzer.c
  )

zephyr_library_include_directories(src)
