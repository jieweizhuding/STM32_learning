
cmake_minimum_required(VERSION 3.20)



# project settings
# 设置项目名
project(Templet  C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
 



#添加宏定义
add_definitions(-DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER)
# add_definitions(-DUSE_HAL_DRIVER -DSTM32F103xB -DUSE_STDPERIPH_DRIVER)

#添加头文件路径，即.h文件
include_directories(./lib/inc ./code)


#添加源文件路径,即.c或者.s文件
file(GLOB_RECURSE SOURCES ./lib/src/*.c ./code/*.c ./startup/*.s)

 

add_executable(${PROJECT_NAME} ${SOURCES} ${LINKER_SCRIPT})
 


set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
 
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
