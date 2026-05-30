add_executable(mlp_xor main.c)

target_link_libraries(mlp_xor PRIVATE nnet::nnet)

target_compile_options(mlp_xor PRIVATE -Wall -Wextra)
