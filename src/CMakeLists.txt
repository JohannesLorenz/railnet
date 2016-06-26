set(src_dir "${CMAKE_CURRENT_SOURCE_DIR}")
set(incl_dir "${src_dir}/../lib")

set(RAILNET_INC_DIR "${OPENTTD_DIR}/src/railnet/")
include_directories("${RAILNET_INC_DIR}")

# TODO: comfortable function for that
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEXPORTER")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")
add_executable(railnet-filter railnet-filter.cpp
	railnet-filter_options.cpp "${RAILNET_INC_DIR}/common.cpp"
	"${RAILNET_INC_DIR}/json_static.cpp"
	"${RAILNET_INC_DIR}/railnet_node_list.cpp"
	)
add_executable(railnet2dot railnet2dot.cpp railnet2dot_options.cpp
	"${RAILNET_INC_DIR}/common.cpp"
	"${RAILNET_INC_DIR}/json_static.cpp"
	"${RAILNET_INC_DIR}/railnet_node_list.cpp"
	)

# target_link_libraries(test_seq ringbuffer)

# add_executable(test_par test_par.cpp)
# target_link_libraries(test_par ${CMAKE_THREAD_LIBS_INIT} ringbuffer)

# add_test(sequential test_seq)
# add_test(parallel test_par)

