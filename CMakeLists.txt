cmake_minimum_required ( VERSION 2.6 )
# Укажите здесь имя вашего проекта.
# Учитываете установку переменных 
# <project_name>_BINARY_DIR и  <project_name>_SOURCE_DIR
# в значение имени каталога с проектом.
project (life_opengl)

set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# Укажите вместо my-project имя своего приложения
# - имя исполняемого файла
set ( APP_NAME life_opengl )

# Управление типом сборки через задание значения
# предопределенной переменной CMAKE_BUILD_TYPE
# Возможные значения:
# DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
set( CMAKE_BUILD_TYPE RELEASE )

# Снять комментарий для добавления пути на 
# заголовочные файлы, которые следует добавить 
# для компиляции проекта
#include_directories ( /path/to/headers_1 )
#include_directories ( /path/to/headers_2 )
# ...
#include_directories ( /path/to/headers_N )
#add_library(life STATIC \
#    life.h life.cpp).
#target_include_directories(life PUBLIC ./include/)

# Снять комментарий для добавления пути на 
# библиотеки, которые следует добавить 
# для линковки проекта
#link_directories ( /path/to/lib_1 )
#link_directories ( /path/to/lib_2 )
# ...
#link_directories ( /path/to/lib_N )

#list(APPEND SRC ${SRC_DIR}/main.cpp )
# снять комментарий для добавления других *.cpp файлов
# list(APPEND SRC ${SRC_DIR}/file_1.cpp )
# list(APPEND SRC ${SRC_DIR}/file_2.cpp )
# ...
# list(APPEND SRC ${SRC_DIR}/file_N.cpp )
#list(APPEND SRC ${SRC_DIR}/life.cpp )

#add_executable ( ${APP_NAME} ${SRC} )
add_executable(life_opengl src/main.cpp include/life.h src/life.cpp)

target_include_directories(life_opengl PUBLIC ./include)
target_include_directories(life_opengl PUBLIC ./src)

# Снять комментарий для добавления имени библиотеки
# требуемой для линковки проекта
#target_link_libraries( ${APP_NAME} name_1 )
#target_link_libraries( ${APP_NAME} name_2 )
# ...
#target_link_libraries( ${APP_NAME} name_N )