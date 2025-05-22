#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

class Logger {
 public:
  static void init(std::string_view log_name) {
    try {
      fs::path log_dir = "../logs";
      if (!fs::exists(log_dir)) {
        fs::create_directory(log_dir);
      }

      std::string log_file = (log_dir / log_name / ".log").string();

      auto console_sink =
          std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      auto file_sink = spdlog::rotating_logger_mt("file_logger", log_file,
                                                  3 * 1024 * 1024, 1)
                           ->sinks()[0];

      console_sink->set_pattern(
          "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
      file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [thread %t] %v");

      std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
      auto logger = std::make_shared<spdlog::logger>(
          "multi_sink", sinks.begin(), sinks.end());

      logger->set_level(spdlog::level::info);

      spdlog::set_default_logger(logger);

      spdlog::flush_every(std::chrono::seconds(3));

      spdlog::info("Logger initialized! Logs will be saved to: {}", log_file);
    } catch (const spdlog::spdlog_ex& ex) {
      spdlog::stderr_color_mt("console");
      spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
      spdlog::error("Failed to initialize file logger: {}", ex.what());
    }
  }

  static void shutdown() { spdlog::shutdown(); }
};