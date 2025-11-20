# cmake/FindMySQL.cmake
# Find MySQL library
#
# This module defines:
#  MYSQL_FOUND - true if MySQL is found
#  MYSQL_INCLUDE_DIRS - MySQL include directories
#  MYSQL_LIBRARIES - MySQL libraries to link against

# MySQL Client Library depends on OpenSSL, dns and others libs, thus In this Library, I will also find ssl、crypto、resolv 

# 设置MySQL的根目录
if(NOT MYSQL_ROOT)
set(MYSQL_ROOT "/usr")
endif()

find_path( 
    MYSQL_INCLUDE_DIR
    NAMES mysql-cppconn/jdbc/mysql_driver.h
    PATHS 
        "${MYSQL_ROOT}/include"
    DOC "MySQL include directory"
)

set(MYSQL_LIB_NAMES
    mysqlcppconn-static
    mysqlcppconnx-static
    )

set(MYSQL_LIBRARY)
foreach(LIB_NAME ${MYSQL_LIB_NAMES})
    find_library(
        ${LIB_NAME}_LIBRARY
        NAMES ${LIB_NAME}
        PATHS 
            "${MYSQL_ROOT}/lib64/"
        NO_DEFAULT_PATH
        DOC "MySQL library ${LIB_NAME}"
    )

    if(${LIB_NAME}_LIBRARY)
        list(APPEND MYSQL_LIBRARY ${${LIB_NAME}_LIBRARY})
    else()
        message(FATAL_ERROR "Can not find lib: ${LIB_NAME}")
    endif()
endforeach()

find_package(OpenSSL REQUIRED)
list(APPEND MYSQL_LIBRARY OpenSSL::SSL OpenSSL::Crypto resolv)

# 处理查找结果
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MYSQL
    FOUND_VAR MYSQL_FOUND
    REQUIRED_VARS 
        MYSQL_INCLUDE_DIR
        MYSQL_LIBRARY
)

if(MYSQL_FOUND)
    # 设置输出变量
    set(MYSQL_INCLUDE_DIRS 
        ${MYSQL_INCLUDE_DIR}
    )

    set(MYSQL_LIBRARIES ${MYSQL_LIBRARY})
    
    # 创建导入目标（现代CMake方式）
    if(NOT TARGET MySQL::MySQL)
        add_library(MySQL::MySQL INTERFACE IMPORTED)  # 使用 INTERFACE 而不是 UNKNOWN
        
        # 设置包含目录
        set_target_properties(MySQL::MySQL PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_INCLUDE_DIRS}"
        )
        
        # 分别链接每个库文件
        foreach(lib ${MYSQL_LIBRARIES})
            # 为每个库创建单独的导入目标
            get_filename_component(lib_name "${lib}" NAME_WE)
            if(NOT TARGET MySQL::${lib_name})
                add_library(MySQL::${lib_name} UNKNOWN IMPORTED)
                set_target_properties(MySQL::${lib_name} PROPERTIES
                    IMPORTED_LOCATION "${lib}"
                )
            endif()
            # 将每个库链接到主目标
            target_link_libraries(MySQL::MySQL INTERFACE MySQL::${lib_name})
        endforeach()
    endif()
    
    # 打印状态信息
    message(STATUS "Found MySQL:")
    message(STATUS "  Includes: ${MYSQL_INCLUDE_DIRS}")
    message(STATUS "  Libraries: ${MYSQL_LIBRARIES}")
endif()

mark_as_advanced(  
    MYSQL_FOUND
    MYSQL_INCLUDE_DIRS
    MYSQL_LIBRARIES
)