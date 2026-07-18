add_library(Pyre STATIC
	"src/Pyre.cpp"
)

target_include_directories(Pyre PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
