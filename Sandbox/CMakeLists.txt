add_executable(Sandbox
	"src/main.cpp"
)

target_include_directories(Sandbox PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_link_libraries(Sandbox PRIVATE Pyre)
