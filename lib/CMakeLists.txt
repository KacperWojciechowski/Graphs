include(GNUInstallDirs)

add_library(RapidXML INTERFACE)

target_include_directories(RapidXML INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/rapidxml)

target_compile_features(RapidXML INTERFACE cxx_std_20)
