add_executable(PyreTests
	"entity/EntityTraitsTests.cpp"
)

target_link_libraries(PyreTests PRIVATE Pyre)

enable_testing()

find_package(GTest CONFIG REQUIRED)
target_link_libraries(PyreTests PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(PyreTests)
