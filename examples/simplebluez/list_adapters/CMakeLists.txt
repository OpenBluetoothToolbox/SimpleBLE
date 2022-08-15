cmake_minimum_required(VERSION 3.20)

project(EXAMPLE_LIST_ADAPTERS)

macro(dump_target_includes TARGET_NAME)
    set(OUT_TMP "====dump_target_includes begin (${TARGET_NAME})====\n\n")
    set(INCLUDE_DIRS $<TARGET_PROPERTY:${TARGET_NAME},INCLUDE_DIRECTORIES>)
    set(INTERFACE_INCLUDE_DIRS $<TARGET_PROPERTY:${TARGET_NAME},INTERFACE_INCLUDE_DIRECTORIES>)
    string(CONCAT OUT_TMP ${OUT_TMP} "----INCLUDE_DIRECTORIES----\n")
    string(CONCAT OUT_TMP ${OUT_TMP} $<JOIN:$<TARGET_PROPERTY:${TARGET_NAME},INCLUDE_DIRECTORIES>,\n>\n\n)
    string(CONCAT OUT_TMP ${OUT_TMP} "----INTERFACE_INCLUDE_DIRECTORIES----\n")
    string(CONCAT OUT_TMP ${OUT_TMP} $<JOIN:$<TARGET_PROPERTY:${TARGET_NAME},INTERFACE_INCLUDE_DIRECTORIES>,\n>\n\n)
    string(CONCAT OUT_TMP ${OUT_TMP} "----INTERFACE_LINK_LIBRARIES----\n")
    string(CONCAT OUT_TMP ${OUT_TMP} $<JOIN:$<TARGET_PROPERTY:${TARGET_NAME},INTERFACE_LINK_LIBRARIES>,\n>\n\n)
    string(CONCAT OUT_TMP ${OUT_TMP} "====dump_target_includes end (${TARGET_NAME})====")
    file(GENERATE OUTPUT "target_includes${TARGET_NAME}.log" CONTENT ${OUT_TMP})
endmacro()

message("-- [INFO] Building Example")
add_executable(example_list_adapters list_adapters.cpp)
target_link_libraries(example_list_adapters simplebluez::simplebluez)

dump_target_includes(example_list_adapters)
dump_target_includes(simplebluez::simplebluez)
