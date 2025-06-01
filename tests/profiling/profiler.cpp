#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <pbrt/profiling/profiler.hpp>

#include <chrono>
#include <thread>

using namespace pbrt::profiling;

namespace
{
void simulate_work(std::chrono::milliseconds duration)
{
  std::this_thread::sleep_for(duration);
}

void nested_function()
{
  PBRT_PROFILE_FUNCTION();
  simulate_work(std::chrono::milliseconds(5));
}

void function_with_scope()
{
  PBRT_PROFILE_FUNCTION();

  {
    PBRT_PROFILE_SCOPE("Inner Scope");
    simulate_work(std::chrono::milliseconds(10));
  }

  simulate_work(std::chrono::milliseconds(5));
}
} // namespace

TEST_CASE("Profiler singleton access", "[profiling]")
{
  auto &profiler1 = Profiler::get_instance();
  auto &profiler2 = Profiler::get_instance();

  REQUIRE(&profiler1 == &profiler2);
}

TEST_CASE("Profiler enable/disable functionality", "[profiling]")
{
  auto &profiler = Profiler::get_instance();

  SECTION("Default state is enabled")
  {
    REQUIRE(profiler.is_enabled());
  }

  SECTION("Can disable profiling")
  {
    profiler.enable(false);
    REQUIRE_FALSE(profiler.is_enabled());

    profiler.enable(true);
    REQUIRE(profiler.is_enabled());
  }
}

TEST_CASE("Manual profiling operations", "[profiling]")
{
  auto &profiler = Profiler::get_instance();
  profiler.reset();
  profiler.enable(true);

  SECTION("Single profile operation")
  {
    profiler.begin_profile("test_operation");
    simulate_work(std::chrono::milliseconds(10));
    profiler.end_profile("test_operation");

    auto profiles = profiler.get_sorted_profiles();
    REQUIRE(profiles.size() == 1);
    REQUIRE(profiles[0].name.find("test_operation") != std::string::npos);
    REQUIRE(profiles[0].callCount == 1);
    REQUIRE(profiles[0].total_time_ms() >= 9.0);
    REQUIRE(profiles[0].total_time_ms() < 50.0);
  }

  SECTION("Multiple profile operations")
  {
    profiler.begin_profile("operation_1");
    simulate_work(std::chrono::milliseconds(5));
    profiler.end_profile("operation_1");

    profiler.begin_profile("operation_2");
    simulate_work(std::chrono::milliseconds(15));
    profiler.end_profile("operation_2");

    auto profiles = profiler.get_sorted_profiles();
    REQUIRE(profiles.size() == 2);

    REQUIRE(profiles[0].total_time_ms() > profiles[1].total_time_ms());
  }

  SECTION("Repeated profile operations")
  {
    for (int i = 0; i < 3; ++i)
    {
      profiler.begin_profile("repeated_operation");
      simulate_work(std::chrono::milliseconds(5));
      profiler.end_profile("repeated_operation");
    }

    auto profiles = profiler.get_sorted_profiles();
    REQUIRE(profiles.size() == 1);
    REQUIRE(profiles[0].callCount == 3);
    REQUIRE(profiles[0].total_time_ms() >= 13.0);
  }
}

TEST_CASE("Scoped timer functionality", "[profiling]")
{
  auto &profiler = Profiler::get_instance();
  profiler.reset();
  profiler.enable(true);

  SECTION("Basic scoped timer")
  {
    {
      ScopedTimer timer("scoped_test");
      simulate_work(std::chrono::milliseconds(8));
    }

    auto profiles = profiler.get_sorted_profiles();
    bool found = false;
    for (const auto &profile : profiles)
    {
      if (profile.name.find("scoped_test") != std::string::npos)
      {
        found = true;
        REQUIRE(profile.callCount == 1);
        REQUIRE(profile.total_time_ms() >= 7.0);
        REQUIRE(profile.total_time_ms() < 30.0);
        break;
      }
    }
    REQUIRE(found);
  }

  SECTION("Nested scoped timers")
  {
    {
      ScopedTimer outerTimer("outer_scope");
      simulate_work(std::chrono::milliseconds(5));

      {
        ScopedTimer innerTimer("inner_scope");
        simulate_work(std::chrono::milliseconds(10));
      }

      simulate_work(std::chrono::milliseconds(5));
    }

    auto profiles = profiler.get_sorted_profiles();
    REQUIRE(profiles.size() >= 2);

    bool foundOuter = false;
    bool foundInner = false;

    for (const auto &profile : profiles)
    {
      if (profile.name.find("outer_scope") != std::string::npos)
      {
        foundOuter = true;
        REQUIRE(profile.total_time_ms() >= 18.0);
      }
      else if (profile.name.find("inner_scope") != std::string::npos)
      {
        foundInner = true;
        REQUIRE(profile.total_time_ms() >= 9.0);
        REQUIRE(profile.total_time_ms() < 25.0);
      }
    }

    REQUIRE(foundOuter);
    REQUIRE(foundInner);
  }
}

TEST_CASE("Profiler macro functionality", "[profiling]")
{
  auto &profiler = Profiler::get_instance();
  profiler.reset();
  profiler.enable(true);

  SECTION("PBRT_PROFILE_FUNCTION macro")
  {
    nested_function();

    auto profiles = profiler.get_sorted_profiles();
    bool found = false;
    for (const auto &profile : profiles)
    {
      if (profile.name.find("nested_function") != std::string::npos)
      {
        found = true;
        REQUIRE(profile.callCount == 1);
        REQUIRE(profile.total_time_ms() >= 4.0);
        break;
      }
    }
    REQUIRE(found);
  }

  SECTION("PBRT_PROFILE_SCOPE macro")
  {
    function_with_scope();

    auto profiles = profiler.get_sorted_profiles();
    REQUIRE(profiles.size() >= 2);

    bool foundFunction = false;
    bool foundScope = false;

    for (const auto &profile : profiles)
    {
      if (profile.name.find("function_with_scope") != std::string::npos)
      {
        foundFunction = true;
      }
      else if (profile.name.find("Inner Scope") != std::string::npos)
      {
        foundScope = true;
      }
    }

    REQUIRE(foundFunction);
    REQUIRE(foundScope);
  }
}

TEST_CASE("Frame-based profiling", "[profiling]")
{
  auto &profiler = Profiler::get_instance();
  profiler.reset();
  profiler.enable(true);

  SECTION("Single frame profiling")
  {
    REQUIRE(profiler.get_frame_count() == 0);

    profiler.begin_frame();
    simulate_work(std::chrono::milliseconds(16)); // Simulate ~60fps frame
    profiler.end_frame();

    REQUIRE(profiler.get_frame_count() == 1);
    REQUIRE(profiler.get_last_frame_time().count() > 0);

    auto frameTimeMs =
        std::chrono::duration<double, std::milli>(profiler.get_last_frame_time()).count();
    REQUIRE(frameTimeMs >= 15.0);
    REQUIRE(frameTimeMs < 50.0);
  }

  SECTION("Multiple frame profiling")
  {
    for (int i = 0; i < 3; ++i)
    {
      profiler.begin_frame();
      simulate_work(std::chrono::milliseconds(5));
      profiler.end_frame();
    }

    REQUIRE(profiler.get_frame_count() == 3);
  }
}

TEST_CASE("Profiler disabled state", "[profiling]")
{
  auto &profiler = Profiler::get_instance();
  profiler.reset();
  profiler.enable(false);

  SECTION("No profiles recorded when disabled")
  {
    profiler.begin_profile("disabled_test");
    simulate_work(std::chrono::milliseconds(10));
    profiler.end_profile("disabled_test");

    auto profiles = profiler.get_sorted_profiles();
    REQUIRE(profiles.empty());
  }

  SECTION("Scoped timer does nothing when disabled")
  {
    {
      ScopedTimer timer("disabled_scoped");
      simulate_work(std::chrono::milliseconds(10));
    }

    auto profiles = profiler.get_sorted_profiles();
    REQUIRE(profiles.empty());
  }

  profiler.enable(true);
}

TEST_CASE("Profile data statistics", "[profiling]")
{
  auto &profiler = Profiler::get_instance();
  profiler.reset();
  profiler.enable(true);

  SECTION("Min/max time tracking")
  {
    // Run the same operation with different durations
    profiler.begin_profile("variable_time");
    simulate_work(std::chrono::milliseconds(5));
    profiler.end_profile("variable_time");

    profiler.begin_profile("variable_time");
    simulate_work(std::chrono::milliseconds(15));
    profiler.end_profile("variable_time");

    profiler.begin_profile("variable_time");
    simulate_work(std::chrono::milliseconds(10));
    profiler.end_profile("variable_time");

    auto profiles = profiler.get_sorted_profiles();
    REQUIRE(profiles.size() == 1);

    const auto &profile = profiles[0];
    REQUIRE(profile.callCount == 3);

    auto minTimeMs = std::chrono::duration<double, std::milli>(profile.minTime).count();
    auto maxTimeMs = std::chrono::duration<double, std::milli>(profile.maxTime).count();

    REQUIRE(minTimeMs < maxTimeMs);
    REQUIRE(minTimeMs >= 4.0);
    REQUIRE(maxTimeMs >= 14.0);
  }

  SECTION("Average time calculation")
  {
    for (int i = 0; i < 5; ++i)
    {
      profiler.begin_profile("average_test");
      simulate_work(std::chrono::milliseconds(10));
      profiler.end_profile("average_test");
    }

    auto profiles = profiler.get_sorted_profiles();
    REQUIRE(profiles.size() == 1);

    const auto &profile = profiles[0];
    REQUIRE(profile.callCount == 5);

    auto totalTime = profile.total_time_ms();
    auto averageTime = profile.average_time_ms();

    REQUIRE(averageTime >= 9.0);
    REQUIRE(averageTime <= 20.0);
    REQUIRE(Catch::Approx(totalTime / 5.0).margin(1.0) == averageTime);
  }
}

TEST_CASE("Profiler reset functionality", "[profiling]")
{
  auto &profiler = Profiler::get_instance();
  profiler.reset();
  profiler.enable(true);

  // Add some profile data
  profiler.begin_profile("test_reset");
  simulate_work(std::chrono::milliseconds(5));
  profiler.end_profile("test_reset");

  profiler.begin_frame();
  simulate_work(std::chrono::milliseconds(5));
  profiler.end_frame();

  REQUIRE(profiler.get_sorted_profiles().size() == 1);
  REQUIRE(profiler.get_frame_count() == 1);

  // Reset and verify everything is cleared
  profiler.reset();

  REQUIRE(profiler.get_sorted_profiles().empty());
  REQUIRE(profiler.get_frame_count() == 0);
  REQUIRE(profiler.get_last_frame_time().count() == 0);
}

TEST_CASE("Unmatched profile operations", "[profiling]")
{
  auto &profiler = Profiler::get_instance();
  profiler.reset();
  profiler.enable(true);

  SECTION("End without begin does nothing")
  {
    profiler.end_profile("nonexistent");

    auto profiles = profiler.get_sorted_profiles();
    REQUIRE(profiles.empty());
  }

  SECTION("Begin without end leaves no trace")
  {
    profiler.begin_profile("unfinished");
    simulate_work(std::chrono::milliseconds(5));
    // Don't call end_profile

    auto profiles = profiler.get_sorted_profiles();
    for (const auto &profile : profiles)
    {
      if (profile.name.find("unfinished") != std::string::npos)
      {
        REQUIRE(profile.callCount == 0);
      }
    }
  }
}
