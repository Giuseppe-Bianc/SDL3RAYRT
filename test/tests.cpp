// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-avoid-magic-numbers, *-magic-numbers, *-unchecked-optional-access, *-avoid-do-while, *-use-anonymous-namespace, *-qualified-auto, *-suspicious-stringview-data-usage, *-err58-cpp, *-function-cognitive-complexity, *-macro-usage, *-unnecessary-copy-initialization)
// clang-format on

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_container_properties.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <future>

#include "testsConstanst.hpp"

using namespace std::literals::string_view_literals;

using Catch::Matchers::ContainsSubstring;
using Catch::Matchers::EndsWith;
using Catch::Matchers::Message;
using Catch::Matchers::MessageMatches;
using Catch::Matchers::StartsWith;
//  using Catch::Matchers::SizeIs;
//  using Catch::Matchers::Equals;

#define REQ_FORMAT(type, string) REQUIRE(FORMAT("{}", type) == (string));
#define REQ_FORMAT_COMPTOK(type, string) REQUIRE(FORMAT("{}", comp_tokType(type)) == (string));
#define MSG_FORMAT(...) Message(FORMAT(__VA_ARGS__))

TEST_CASE("extractTabs basic functionality", "[extractTabs]") {
    SECTION("String with only tabs") {
        const auto result = extractTabs(inputTabs);
        REQUIRE(result == inputTabs);  // Only tabs, so the result should be the same as the input
    }

    SECTION("String with tabs followed by other characters") {
        const auto result = extractTabs(inputMixed);
        REQUIRE(result == "\t\t\t");
    }

    SECTION("String without any tabs") {
        const auto result = extractTabs(inputText);
        REQUIRE(result.empty());  // No tabs, so the result should be empty
    }

    SECTION("String with no leading tabs but tabs in middle") {
        const auto result = extractTabs(inputMixedMidle);
        REQUIRE(result.empty());  // No leading tabs
    }

    SECTION("Empty string") {
        const std::string_view input;
        const std::string_view result = extractTabs(input);
        REQUIRE(result.empty());  // Empty input should return empty result
    }

    SECTION("String with mixed whitespace and tabs") {
        const auto result = extractTabs(inputMixedSpace);
        REQUIRE(result == "\t");
    }

    SECTION("String with multiple tabs and spaces") {
        const auto result = extractTabs(inputMixedSpace2);
        REQUIRE(result == "\t\t");  // Only leading tabs should be extracted, ignoring spaces
    }

    SECTION("String with only spaces") {
        const auto result = extractTabs(inputSpace);
        REQUIRE(result.empty());  // No tabs should result in an empty output
    }

    SECTION("String with a mix of leading tabs and spaces") {
        const auto result = extractTabs(inputMixedSpace3);
        REQUIRE(result == "\t");  // Only tabs until first non-tab character
    }
}

TEST_CASE("TimeValues initialization", "[TimeValues]") {
    using vnd::TimeValues;

    SECTION("Default Constructor") {
        const TimeValues time;
        REQUIRE(time.get_seconds() == 0.0L);
        REQUIRE(time.get_millis() == 0.0L);
        REQUIRE(time.get_micro() == 0.0L);
        REQUIRE(time.get_nano() == 0.0L);
    }

    SECTION("Initialization with nanoseconds") {
        const TimeValues time(1'000'000.0L);  // 1 millisecond in nanoseconds
        REQUIRE(time.get_seconds() == 0.001L);
        REQUIRE(time.get_millis() == 1.0L);
        REQUIRE(time.get_micro() == 1000.0L);
        REQUIRE(time.get_nano() == 1'000'000.0L);
    }

    SECTION("Initialization with individual time units") {
        const TimeValues time(1.0L, 1000.0L, 1'000'000.0L, 1'000'000'000.0L);  // 1 second
        REQUIRE(time.get_seconds() == 1.0L);
        REQUIRE(time.get_millis() == 1000.0L);
        REQUIRE(time.get_micro() == 1'000'000.0L);
        REQUIRE(time.get_nano() == 1'000'000'000.0L);
    }
}

TEST_CASE("ValueLabel functionality", "[ValueLabel]") {
    using vnd::ValueLabel;

    SECTION("Transform time in microseconds") {
        const ValueLabel value(time_val_micro, "us");
        REQUIRE(value.transformTimeMicro(time_val_micro) == "1500us,0ns");

        const ValueLabel valueNonExact(time_val_micro2, "us");
        REQUIRE(valueNonExact.transformTimeMicro(time_val_micro2) == "1500us,500ns");
    }

    SECTION("Transform time in milliseconds") {
        const ValueLabel value(time_val_milli, "ms");
        REQUIRE(value.transformTimeMilli(time_val_milli) == "2ms,500us,0ns");

        const ValueLabel valueNonExact(time_val_milli2, "ms");
        REQUIRE(valueNonExact.transformTimeMilli(time_val_milli2) == "2ms,505us,0ns");
    }

    SECTION("Transform time in seconds") {
        const ValueLabel value(time_val_second, "s");
        REQUIRE(value.transformTimeSeconds(time_val_second) == "1s,0ms,0us,0ns");

        const ValueLabel valueNonExact(time_val_second2, "s");
        REQUIRE(valueNonExact.transformTimeSeconds(time_val_second2) == "1s,5ms,1us,0ns");
    }

    SECTION("ToString based on time label") {
        const ValueLabel secondsVal(2.0L, "s");
        REQUIRE(secondsVal.toString() == "2s,0ms,0us,0ns");

        const ValueLabel millisVal(2500.0L, "ms");
        REQUIRE(millisVal.toString() == "2500ms,0us,0ns");

        const ValueLabel microsVal(1500.0L, "us");
        REQUIRE(microsVal.toString() == "1500us,0ns");

        const ValueLabel unknownVal(3.0L, "unknown");
        REQUIRE(unknownVal.toString() == "3 unknown");
    }
}

TEST_CASE("Times functionality for  nano seconds", "[Times]") {
    const vnd::Times time(10.0L);  // 1 millisecond
    REQUIRE(time.getRelevantTimeframe().toString() == "10 ns");
}

TEST_CASE("Times functionality", "[Times]") {
    using vnd::Times;
    using vnd::TimeValues;
    using vnd::ValueLabel;

    SECTION("Initialization with nanoseconds") {
        const Times time(1'000'000.0L);  // 1 millisecond
        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "1000us,0ns");
    }

    SECTION("Initialization with TimeValues and custom labels") {
        const TimeValues timeVals(0.5L, 500.0L, 500'000.0L, 500'000'000.0L);  // 0.5 seconds
        const Times time(timeVals, "seconds", "milliseconds", "microseconds", "nanoseconds");

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "500 milliseconds");
    }

    SECTION("Switch between time units") {
        const TimeValues timeVals(0.001L, 1.0L, 1000.0L, 1'000'000.0L);  // 1 millisecond
        const Times time(timeVals);

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "1000us,0ns");
    }

    SECTION("Very small nanoseconds") {
        const TimeValues timeVals(0.000001L, 0.001L, 1.0L, 1'000.0L);  // 1 microsecond
        const Times time(timeVals);

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "1000 ns");
    }
}

TEST_CASE("Corner cases for TimeValues and Times", "[TimeValues][Times][CornerCases]") {
    using vnd::Times;
    using vnd::TimeValues;
    using vnd::ValueLabel;

    SECTION("Negative values") {
        const TimeValues negativeTime(-1000000.0L);  // -1 millisecond
        const Times time(negativeTime);

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "-1000000 ns");
    }

    SECTION("Zero values") {
        const TimeValues zeroTime(0.0L);  // Zero nanoseconds
        const Times time(zeroTime);

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "0 ns");
    }

    SECTION("Large values") {
        const long double largeValue = 1'000'000'000'000.0L;  // 1 second in nanoseconds
        const TimeValues largeTime(largeValue);               // 1 second
        const Times time(largeTime);

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "1000s,0ms,0us,0ns");
    }
}

TEST_CASE("get_current_timestamp() tests", "[timestamp]") {
    SECTION("Basic test") {
        auto timestamp = get_current_timestamp();
        REQUIRE(timestamp.size() >= timestampSize);
    }

    SECTION("Repeatability test") {
        auto timestamp1 = get_current_timestamp();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto timestamp2 = get_current_timestamp();
        REQUIRE(timestamp1 != timestamp2);
    }

    SECTION("Concurrency test") {
        constexpr int num_threads = 4;
        std::vector<std::future<std::string>> futures;
        for(int i = 0; i < num_threads; ++i) {
            // NOLINTNEXTLINE(*-inefficient-vector-operation)
            futures.emplace_back(std::async(std::launch::async, []() { return get_current_timestamp(); }));
        }
        for(auto &future : futures) {
            auto timestamp = future.get();
            REQUIRE(timestamp.size() >= timestampSize);
        }
    }
}

/*TEST_CASE("deleteFile: Handle file with read-only permissions", "[FileDelitionResult]") {
    fs::path readOnlyFile = fs::temp_directory_path() / "read_only_file.txt";

    // Create the test file and set to read-only
    std::ofstream(readOnlyFile) << "Read-only file content";
    REQUIRE(fs::exists(readOnlyFile));

#ifdef _WIN32
    _chmod(readOnlyFile.string().c_str(), _S_IREAD);
#else
    chmod(readOnlyFile.string().c_str(), S_IRUSR);
#endif

    auto result = vnd::FileDelitionResult::deleteFile(readOnlyFile);

    REQUIRE(result.success());
    REQUIRE(!fs::exists(readOnlyFile));

    // No need to reset permissions since the file is deleted
}*/

TEST_CASE("my_error_handler(const std::string&) tests", "[error_handler]") {
    SECTION("Basic error handling") {
        const std::stringstream sss;
        auto *original = std::cerr.rdbuf(sss.rdbuf());  // Redirect cerr to stringstream
        my_error_handler("Sample error message");
        std::cerr.rdbuf(original);  // Restore cerr

        auto output = sss.str();
        REQUIRE_THAT(output, ContainsSubstring("Error occurred:"));
        REQUIRE_THAT(output, ContainsSubstring("Timestamp: "));
        REQUIRE_THAT(output, ContainsSubstring("Thread ID: "));
        REQUIRE_THAT(output, ContainsSubstring("Message: Sample error message"));
    }

    SECTION("Error handler with different messages") {
        const std::stringstream sss;
        auto *original = std::cerr.rdbuf(sss.rdbuf());  // Redirect cerr to stringstream
        my_error_handler("Error 1");
        my_error_handler("Another error");
        std::cerr.rdbuf(original);  // Restore cerr

        auto output = sss.str();
        REQUIRE_THAT(output, ContainsSubstring("Message: Error 1"));
        REQUIRE_THAT(output, ContainsSubstring("Message: Another error"));
    }

    SECTION("Location information") {
        const std::stringstream sss;
        auto *original = std::cerr.rdbuf(sss.rdbuf());  // Redirect cerr to stringstream
        my_error_handler("Location test");
        std::cerr.rdbuf(original);  // Restore cerr

        auto output = sss.str();
        REQUIRE_THAT(output, ContainsSubstring("File: "));
        REQUIRE_THAT(output, ContainsSubstring("Line: "));
        REQUIRE_THAT(output, ContainsSubstring("Column: "));
    }
}

TEST_CASE("std::filesystem::path formater", "[FMT]") { REQ_FORMAT(std::filesystem::path("../ssss"), "../ssss"); }

/*
TEST_CASE("glm::vec formater", "[FMT]") {
    REQ_FORMAT(glm::vec2(0.0F, 0.0F), "vec2(0, 0)");
    REQ_FORMAT(glm::dvec2(0.0, 0.0), "dvec2(0, 0)");
    REQ_FORMAT(glm::ldvec2(0.0, 0.0), "ldvec2(0, 0)");
    REQ_FORMAT(glm::vec3(0.0F, 0.0F, 0.0F), "vec3(0, 0, 0)");
    REQ_FORMAT(glm::dvec3(0.0, 0.0, 0.0), "dvec3(0, 0, 0)");
    REQ_FORMAT(glm::ldvec3(0.0, 0.0, 0.0F), "ldvec3(0, 0, 0)");
    REQ_FORMAT(glm::vec4(0.0F, 0.0, 0.0, 0.0), "vec4(0, 0, 0, 0)");
    REQ_FORMAT(glm::dvec4(0.0, 0.0, 0.0, 0.0), "dvec4(0, 0, 0, 0)");
    REQ_FORMAT(glm::ldvec4(0.0, 0.0, 0.0F, 0.0), "ldvec4(0, 0, 0, 0)");
}

// clang-format off
TEST_CASE("glm::mat formater", "[FMT]") {
    REQ_FORMAT(glm::mat2(0.0F, 0.0F, 0.0F, 0.0F), "mat2x2((0,0), (0,0))");
    REQ_FORMAT(glm::dmat2(0.0, 0.0, 0.0, 0.0), "dmat2x2((0,0), (0,0))");
    REQ_FORMAT(glm::ldmat2(0.0, 0.0, 0.0, 0.0), "ldmat2x2((0,0), (0,0))");
    REQ_FORMAT(glm::mat3(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F), "mat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQ_FORMAT(glm::dmat3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "dmat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQ_FORMAT(glm::ldmat3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "ldmat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQ_FORMAT(glm::mat4(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F), "mat4x4((0, 0, 0,
0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))"); REQ_FORMAT(glm::dmat4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
0.0, 0.0), "dmat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))"); REQ_FORMAT(glm::ldmat4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "ldmat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))");
}
*/

TEST_CASE("std::complex formater", "[FMT]") {
    REQ_FORMAT(std::complex<float>(1.0F, 2.0F), "(1+2i)");
    REQ_FORMAT(std::complex<double>(3.0, 4.0), "(3+4i)");
    REQ_FORMAT(std::complex<long double>(5.0L, 6.0L), "(5+6i)");
}

TEST_CASE("std::complex formater 2", "[FMT]") {
    REQ_FORMAT(std::complex<float>(1.1F, 2.1F), "(1.1+2.1i)");
    REQ_FORMAT(std::complex<double>(3.1, 4.1), "(3.1+4.1i)");
    REQ_FORMAT(std::complex<long double>(5.1L, 6.1L), "(5.1+6.1i)");
}

// clang-format on

TEST_CASE("Timer: MSTimes", "[timer]") {
    const auto timerNameData = timerName.data();
    vnd::Timer timer{timerNameData};
    std::this_thread::sleep_for(std::chrono::milliseconds(timerSleap));
    const std::string output = timer.to_string();
    const std::string new_output = (timer / timerCicles).to_string();
    REQUIRE_THAT(output, ContainsSubstring(timerNameData));
    REQUIRE_THAT(output, ContainsSubstring(timerNameData));
    REQUIRE_THAT(output, ContainsSubstring(timerTime1.data()));
    REQUIRE_THAT(new_output, ContainsSubstring(timerTime2.data()));
}

TEST_CASE("Timer: MSTimes FMT", "[timer]") {
    const auto timerNameData = timerName.data();
    vnd::Timer timer{timerNameData};
    std::this_thread::sleep_for(std::chrono::milliseconds(timerSleap));
    const std::string output = FORMAT("{}", timer);
    const std::string new_output = FORMAT("{}", (timer / timerCicles));
    REQUIRE_THAT(output, ContainsSubstring(timerNameData));
    REQUIRE_THAT(output, ContainsSubstring(timerTime1.data()));
    REQUIRE_THAT(new_output, ContainsSubstring(timerTime2.data()));
}

TEST_CASE("Timer: BigTimer", "[timer]") {
    const auto timerNameData = timerName.data();
    const vnd::Timer timer{timerNameData, vnd::Timer::Big};
    const std::string output = timer.to_string();
    REQUIRE_THAT(output, ContainsSubstring(timerNameData));
    REQUIRE_THAT(output, ContainsSubstring(timerBigs.data()));
}

TEST_CASE("Timer: BigTimer FMT", "[timer]") {
    const auto timerNameData = timerName.data();
    vnd::Timer timer{timerNameData, vnd::Timer::Big};
    const std::string output = FORMAT("{}", timer);
    REQUIRE_THAT(output, ContainsSubstring(timerNameData));
    REQUIRE_THAT(output, ContainsSubstring(timerBigs.data()));
}

TEST_CASE("Timer: AutoTimer", "[timer]") {
    const vnd::Timer timer;
    const std::string output = timer.to_string();
    REQUIRE_THAT(output, ContainsSubstring("Timer"));
}

TEST_CASE("Timer: PrintTimer", "[timer]") {
    std::stringstream out;
    const vnd::Timer timer;
    out << timer;
    const std::string output = out.str();
    REQUIRE_THAT(output, ContainsSubstring(timerName2.data()));
}

TEST_CASE("Timer: PrintTimer FMT", "[timer]") {
    vnd::Timer timer;
    const std::string output = FORMAT("{}", timer);
    REQUIRE_THAT(output, ContainsSubstring(timerName2.data()));
}

TEST_CASE("Timer: TimeItTimer", "[timer]") {
    vnd::Timer timer;
    const std::string output = timer.time_it([]() { std::this_thread::sleep_for(std::chrono::milliseconds(timerSleap2)); },
                                             timerResolution);
    REQUIRE_THAT(output, ContainsSubstring(timerTime1.data()));
}

namespace {
    // Helper function to create a file with content
    // NOLINTBEGIN(*-easily-swappable-parameters, *-signed-bitwise)
    void createFile(const std::string &infilename, const std::string &content) {
        std::ofstream ofs(infilename, std::ios::out | std::ios::binary);
        ofs << content;
        ofs.close();
    }
    // NOLINTEND(*-easily-swappable-parameters, *-signed-bitwise)
}  // namespace

TEST_CASE("vnd::readFromFile - Valid File", "[file]") {
    const std::string infilename = "testfile.txt";
    const std::string content = "This is a test.";

    createFile(infilename, content);

    auto result = vnd::readFromFile(infilename);
    REQUIRE(result == content);  // Ensure the content matches

    [[maybe_unused]] auto unsed = fs::remove(infilename);
}

TEST_CASE("vnd::readFromFile - Non-existent File", "[file]") {
    const std::string nonExistentFile = "nonexistent.txt";

    REQUIRE_THROWS_MATCHES(vnd::readFromFile(nonExistentFile), std::runtime_error, MSG_FORMAT("File not found: {}", nonExistentFile));
}

TEST_CASE("vnd::readFromFile - Non-regular File", "[file]") {
    const std::string dirName = "testdir";

    fs::create_directory(dirName);

    REQUIRE_THROWS_MATCHES(vnd::readFromFile(dirName), std::runtime_error, MSG_FORMAT("Path is not a regular file: {}", dirName));
    [[maybe_unused]] auto unsed = fs::remove(dirName);
}

TEST_CASE("vnd::readFromFile - Empty File", "[file]") {
    const std::string emtfilename = "emptyfile.txt";

    createFile(emtfilename, "");

    SECTION("Read from an empty file") {
        const auto result = vnd::readFromFile(emtfilename);
        REQUIRE(result.empty());  // Ensure the result is empty
    }

    [[maybe_unused]] auto unsed = fs::remove(emtfilename);
}

TEST_CASE("vnd::readFromFile - Large File", "[file]") {
    const std::string lrgfilename = "largefile.txt";
    const std::string largeContent(C_ST(1024 * 1024) * 10, 'a');  // 10 MB of 'a'

    createFile(lrgfilename, largeContent);

    SECTION("Read from a large file") {
        auto result = vnd::readFromFile(lrgfilename);
        REQUIRE(result == largeContent);  // Ensure content matches
    }

    [[maybe_unused]] auto unsed = fs::remove(lrgfilename);
}

// clang-format off
// NOLINTEND(*-include-cleaner, *-avoid-magic-numbers, *-magic-numbers, *-unchecked-optional-access, *-avoid-do-while, *-use-anonymous-namespace, *-qualified-auto, *-suspicious-stringview-data-usage, *-err58-cpp, *-function-cognitive-complexity, *-macro-usage, *-unnecessary-copy-initialization)
// clang-format on