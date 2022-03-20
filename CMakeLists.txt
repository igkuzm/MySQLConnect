cmake_minimum_required(VERSION 3.15)

set(TARGET MySQLConnect)
project(${TARGET})

#To include MySQLConnect to your project use:
#set(MYSQLCONNECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/path/to/MySQLConnect)
#add_subdirectory(${MYSQLCONNECT_SOURCE_DIR})

if (NOT DEFINED MYSQLCONNECT_SOURCE_DIR)
  set(MYSQLCONNECT_SOURCE_DIR ${CMAKE_SOURCE_DIR})
endif ()

add_library(${TARGET} STATIC MySQLConnect.c)

if(ANDROID)
	#SET_TARGET_PROPERTIES(mysqlclient PROPERTIES IMPORTED_LOCATION ${MYSQLCONNECT_SOURCE_DIR}/resources/curl/android/${ANDROID_ABI}/libmysqlclient.a)	
	add_subdirectory(${MYSQLCONNECT_SOURCE_DIR}/mysql)
	list(APPEND ADD_LIBS mysql)

elseif(APPLE)
	add_library(mysqlclient STATIC IMPORTED)
	if(IOS)
		set_target_properties(mysqlclient PROPERTIES IMPORTED_LOCATION ${MYSQLCONNECT_SOURCE_DIR}/mysqlclient/iOS/libmysqlclient.a)	  
	else() #MacOS
		set_target_properties(mysqlclient PROPERTIES IMPORTED_LOCATION ${MYSQLCONNECT_SOURCE_DIR}/mysqlclient/MacOS/libmysqlclient.a)	  
	endif()
	list(APPEND ADD_LIBS mysqlclient stdc++ z)

elseif(WIN32)
	link_directories(${MYSQLCONNECT_SOURCE_DIR}/mysqlclient/Windows)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8) #64 bit
		configure_file(${MYSQLCONNECT_SOURCE_DIR}/mysqlclient/Windows/libmysql_win64.dll libmysql.dll COPYONLY)
		list(APPEND ADD_LIBS stdc++ mysql_win64)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) #32 bit
		configure_file(${MYSQLCONNECT_SOURCE_DIR}/mysqlclient/Windows/Win32/libmysql_win32.dll libmysql.dll COPYONLY)
		list(APPEND ADD_LIBS stdc++ mysql_win32)
	endif()

else() #LINUX OR UNIX
	#Use pkg_config
	find_package(PkgConfig)
	pkg_check_modules(MySQL REQUIRED mysqlclient>=5.7)
	target_include_directories(${TARGET} PUBLIC ${MySQL_INCLUDE_DIRS})	
	list(APPEND ADD_LIBS ${MySQL_LIBRARIES})
endif()

target_link_libraries(${TARGET} ${ADD_LIBS})

add_executable(mysql_connect_test test.c)
target_link_libraries(mysql_connect_test ${TARGET})
