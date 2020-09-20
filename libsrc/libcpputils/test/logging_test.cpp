/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cpputils-base/logging.h"

#include <libgen.h>

#if defined(_WIN32)
#include <signal.h>
#endif

#include <regex>
#include <string>

#include "cpputils-base/file.h"
#include "cpputils-base/stringprintf.h"
#include "cpputils-base/test_utils.h"

#include <gtest/gtest.h>

#ifdef __ANDROID__
#define HOST_TEST(suite, name) TEST(suite, DISABLED_##name)
#else
#define HOST_TEST(suite, name) TEST(suite, name)
#endif

#if defined(_WIN32)
static void ExitSignalAbortHandler(int)
{
  _exit(3);
}
#endif

static void SuppressAbortUI()
{
#if defined(_WIN32)
  // We really just want to call _set_abort_behavior(0, _CALL_REPORTFAULT) to
  // suppress the Windows Error Reporting dialog box, but that API is not
  // available in the OS-supplied C Runtime, msvcrt.dll, that we currently
  // use (it is available in the Visual Studio C runtime).
  //
  // Instead, we setup a SIGABRT handler, which is called in abort() right
  // before calling Windows Error Reporting. In the handler, we exit the
  // process just like abort() does.
  ASSERT_NE(SIG_ERR, signal(SIGABRT, ExitSignalAbortHandler));
#endif
}
/*
TEST(logging, CHECK) {
  ASSERT_DEATH({SuppressAbortUI(); CHECK(false);}, "Check failed: false ");
  CHECK(true);

  ASSERT_DEATH({SuppressAbortUI(); CHECK_EQ(0, 1);}, "Check failed: 0 == 1 ");
  CHECK_EQ(0, 0);

  ASSERT_DEATH({SuppressAbortUI(); CHECK_STREQ("foo", "bar");},
               R"(Check failed: "foo" == "bar")");
  CHECK_STREQ("foo", "foo");

  // Test whether CHECK() and CHECK_STREQ() have a dangling if with no else.
  bool flag = false;
  if (true)
    CHECK(true);
  else
    flag = true;
  EXPECT_FALSE(flag) << "CHECK macro probably has a dangling if with no else";

  flag = false;
  if (true)
    CHECK_STREQ("foo", "foo");
  else
    flag = true;
  EXPECT_FALSE(flag) << "CHECK_STREQ probably has a dangling if with no else";
}

TEST(logging, DCHECK) {
  if (cpputils::base::kEnableDChecks) {
    ASSERT_DEATH({SuppressAbortUI(); DCHECK(false);}, "DCheck failed: false ");
  }
  DCHECK(true);

  if (cpputils::base::kEnableDChecks) {
    ASSERT_DEATH({SuppressAbortUI(); DCHECK_EQ(0, 1);}, "DCheck failed: 0 == 1 ");
  }
  DCHECK_EQ(0, 0);

  if (cpputils::base::kEnableDChecks) {
    ASSERT_DEATH({SuppressAbortUI(); DCHECK_STREQ("foo", "bar");},
                 R"(DCheck failed: "foo" == "bar")");
  }
  DCHECK_STREQ("foo", "foo");

  // No testing whether we have a dangling else, possibly. That's inherent to the if (constexpr)
  // setup we intentionally chose to force type-checks of debug code even in release builds (so
  // we don't get more bit-rot).
}


#define CHECK_WOULD_LOG_DISABLED(severity)                                                 \
  static_assert(cpputils::base::severity < cpputils::base::FATAL, "Bad input");            \
  for (size_t i = static_cast<size_t>(cpputils::base::severity) + 1;                       \
       i <= static_cast<size_t>(cpputils::base::FATAL);                                    \
       ++i)                                                                                \
  {                                                                                        \
    {                                                                                      \
      cpputils::base::ScopedLogSeverity sls2(static_cast<cpputils::base::LogSeverity>(i)); \
      EXPECT_FALSE(WOULD_LOG(severity)) << i;                                              \
    }                                                                                      \
    {                                                                                      \
      cpputils::base::ScopedLogSeverity sls2(static_cast<cpputils::base::LogSeverity>(i)); \
      EXPECT_FALSE(WOULD_LOG(::cpputils::base::severity)) << i;                            \
    }                                                                                      \
  }

#define CHECK_WOULD_LOG_ENABLED(severity)                                                  \
  for (size_t i = static_cast<size_t>(cpputils::base::VERBOSE);                            \
       i <= static_cast<size_t>(cpputils::base::severity);                                 \
       ++i)                                                                                \
  {                                                                                        \
    {                                                                                      \
      cpputils::base::ScopedLogSeverity sls2(static_cast<cpputils::base::LogSeverity>(i)); \
      EXPECT_TRUE(WOULD_LOG(severity)) << i;                                               \
    }                                                                                      \
    {                                                                                      \
      cpputils::base::ScopedLogSeverity sls2(static_cast<cpputils::base::LogSeverity>(i)); \
      EXPECT_TRUE(WOULD_LOG(::cpputils::base::severity)) << i;                             \
    }                                                                                      \
  }

TEST(logging, WOULD_LOG_FATAL)
{
  CHECK_WOULD_LOG_ENABLED(FATAL);
}

TEST(logging, WOULD_LOG_FATAL_WITHOUT_ABORT_disabled)
{
  CHECK_WOULD_LOG_DISABLED(FATAL_WITHOUT_ABORT);
}

TEST(logging, WOULD_LOG_FATAL_WITHOUT_ABORT_enabled)
{
  CHECK_WOULD_LOG_ENABLED(FATAL_WITHOUT_ABORT);
}

TEST(logging, WOULD_LOG_ERROR_disabled)
{
  CHECK_WOULD_LOG_DISABLED(ERROR);
}

TEST(logging, WOULD_LOG_ERROR_enabled)
{
  CHECK_WOULD_LOG_ENABLED(ERROR);
}

TEST(logging, WOULD_LOG_WARNING_disabled)
{
  CHECK_WOULD_LOG_DISABLED(WARNING);
}

TEST(logging, WOULD_LOG_WARNING_enabled)
{
  CHECK_WOULD_LOG_ENABLED(WARNING);
}

TEST(logging, WOULD_LOG_INFO_disabled)
{
  CHECK_WOULD_LOG_DISABLED(INFO);
}

TEST(logging, WOULD_LOG_INFO_enabled)
{
  CHECK_WOULD_LOG_ENABLED(INFO);
}

TEST(logging, WOULD_LOG_DEBUG_disabled)
{
  CHECK_WOULD_LOG_DISABLED(DEBUG);
}

TEST(logging, WOULD_LOG_DEBUG_enabled)
{
  CHECK_WOULD_LOG_ENABLED(DEBUG);
}

TEST(logging, WOULD_LOG_VERBOSE_disabled)
{
  CHECK_WOULD_LOG_DISABLED(VERBOSE);
}

TEST(logging, WOULD_LOG_VERBOSE_enabled)
{
  CHECK_WOULD_LOG_ENABLED(VERBOSE);
}

#undef CHECK_WOULD_LOG_DISABLED
#undef CHECK_WOULD_LOG_ENABLED

#if !defined(_WIN32)
static std::string make_log_pattern(cpputils::base::LogSeverity severity,
                                    const char *message)
{
  static const char log_characters[] = "VDIWEFF";
  static_assert(arraysize(log_characters) - 1 == cpputils::base::FATAL + 1,
                "Mismatch in size of log_characters and values in LogSeverity");
  char log_char = log_characters[severity];
  std::string holder(__FILE__);
  return cpputils::base::StringPrintf(
      "%c \\d+-\\d+ \\d+:\\d+:\\d+ \\s*\\d+ \\s*\\d+ %s:\\d+] %s",
      log_char, basename(&holder[0]), message);
}
#endif

static void CheckMessage(const std::string &output, cpputils::base::LogSeverity severity,
                         const char *expected, const char *expected_tag = nullptr)
{
  // We can't usefully check the output of any of these on Windows because we
  // don't have std::regex, but we can at least make sure we printed at least as
  // many characters are in the log message.
  ASSERT_GT(output.length(), strlen(expected));
  ASSERT_NE(nullptr, strstr(output.c_str(), expected)) << output;
  if (expected_tag != nullptr)
  {
    ASSERT_NE(nullptr, strstr(output.c_str(), expected_tag)) << output;
  }

#if !defined(_WIN32)
  std::string regex_str;
  if (expected_tag != nullptr)
  {
    regex_str.append(expected_tag);
    regex_str.append(" ");
  }
  regex_str.append(make_log_pattern(severity, expected));
  std::regex message_regex(regex_str);
  ASSERT_TRUE(std::regex_search(output, message_regex)) << output;
#endif
}

static void CheckMessage(CapturedStderr &cap, cpputils::base::LogSeverity severity,
                         const char *expected, const char *expected_tag = nullptr)
{
  cap.Stop();
  std::string output = cap.str();
  return CheckMessage(output, severity, expected, expected_tag);
}

#define CHECK_LOG_STREAM_DISABLED(severity)                        \
  {                                                                \
    cpputils::base::ScopedLogSeverity sls1(cpputils::base::FATAL); \
    CapturedStderr cap1;                                           \
    LOG_STREAM(severity) << "foo bar";                             \
    cap1.Stop();                                                   \
    ASSERT_EQ("", cap1.str());                                     \
  }                                                                \
  {                                                                \
    cpputils::base::ScopedLogSeverity sls1(cpputils::base::FATAL); \
    CapturedStderr cap1;                                           \
    LOG_STREAM(::cpputils::base::severity) << "foo bar";           \
    cap1.Stop();                                                   \
    ASSERT_EQ("", cap1.str());                                     \
  }

#define CHECK_LOG_STREAM_ENABLED(severity)                            \
  {                                                                   \
    cpputils::base::ScopedLogSeverity sls2(cpputils::base::severity); \
    CapturedStderr cap2;                                              \
    LOG_STREAM(severity) << "foobar";                                 \
    CheckMessage(cap2, cpputils::base::severity, "foobar");           \
  }                                                                   \
  {                                                                   \
    cpputils::base::ScopedLogSeverity sls2(cpputils::base::severity); \
    CapturedStderr cap2;                                              \
    LOG_STREAM(::cpputils::base::severity) << "foobar";               \
    CheckMessage(cap2, cpputils::base::severity, "foobar");           \
  }

TEST(logging, LOG_STREAM_FATAL_WITHOUT_ABORT_disabled)
{
  CHECK_LOG_STREAM_DISABLED(FATAL_WITHOUT_ABORT);
}

TEST(logging, LOG_STREAM_FATAL_WITHOUT_ABORT_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_STREAM_ENABLED(FATAL_WITHOUT_ABORT));
}

TEST(logging, LOG_STREAM_ERROR_disabled)
{
  CHECK_LOG_STREAM_DISABLED(ERROR);
}

TEST(logging, LOG_STREAM_ERROR_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_STREAM_ENABLED(ERROR));
}

TEST(logging, LOG_STREAM_WARNING_disabled)
{
  CHECK_LOG_STREAM_DISABLED(WARNING);
}

TEST(logging, LOG_STREAM_WARNING_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_STREAM_ENABLED(WARNING));
}

TEST(logging, LOG_STREAM_INFO_disabled)
{
  CHECK_LOG_STREAM_DISABLED(INFO);
}

TEST(logging, LOG_STREAM_INFO_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_STREAM_ENABLED(INFO));
}

TEST(logging, LOG_STREAM_DEBUG_disabled)
{
  CHECK_LOG_STREAM_DISABLED(DEBUG);
}

TEST(logging, LOG_STREAM_DEBUG_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_STREAM_ENABLED(DEBUG));
}

TEST(logging, LOG_STREAM_VERBOSE_disabled)
{
  CHECK_LOG_STREAM_DISABLED(VERBOSE);
}

TEST(logging, LOG_STREAM_VERBOSE_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_STREAM_ENABLED(VERBOSE));
}

#undef CHECK_LOG_STREAM_DISABLED
#undef CHECK_LOG_STREAM_ENABLED

#define CHECK_LOG_DISABLED(severity)                               \
  {                                                                \
    cpputils::base::ScopedLogSeverity sls1(cpputils::base::FATAL); \
    CapturedStderr cap1;                                           \
    LOG(severity) << "foo bar";                                    \
    cap1.Stop();                                                   \
    ASSERT_EQ("", cap1.str());                                     \
  }                                                                \
  {                                                                \
    cpputils::base::ScopedLogSeverity sls1(cpputils::base::FATAL); \
    CapturedStderr cap1;                                           \
    LOG(::cpputils::base::severity) << "foo bar";                  \
    cap1.Stop();                                                   \
    ASSERT_EQ("", cap1.str());                                     \
  }

#define CHECK_LOG_ENABLED(severity)                                   \
  {                                                                   \
    cpputils::base::ScopedLogSeverity sls2(cpputils::base::severity); \
    CapturedStderr cap2;                                              \
    LOG(severity) << "foobar";                                        \
    CheckMessage(cap2, cpputils::base::severity, "foobar");           \
  }                                                                   \
  {                                                                   \
    cpputils::base::ScopedLogSeverity sls2(cpputils::base::severity); \
    CapturedStderr cap2;                                              \
    LOG(::cpputils::base::severity) << "foobar";                      \
    CheckMessage(cap2, cpputils::base::severity, "foobar");           \
  }

TEST(logging, LOG_FATAL)
{
  ASSERT_DEATH({SuppressAbortUI(); LOG(FATAL) << "foobar"; }, "foobar");
  ASSERT_DEATH({SuppressAbortUI(); LOG(::cpputils::base::FATAL) << "foobar"; }, "foobar");
}

TEST(logging, LOG_FATAL_WITHOUT_ABORT_disabled)
{
  CHECK_LOG_DISABLED(FATAL_WITHOUT_ABORT);
}

TEST(logging, LOG_FATAL_WITHOUT_ABORT_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_ENABLED(FATAL_WITHOUT_ABORT));
}

TEST(logging, LOG_ERROR_disabled)
{
  CHECK_LOG_DISABLED(ERROR);
}

TEST(logging, LOG_ERROR_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_ENABLED(ERROR));
}

TEST(logging, LOG_WARNING_disabled)
{
  CHECK_LOG_DISABLED(WARNING);
}

TEST(logging, LOG_WARNING_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_ENABLED(WARNING));
}

TEST(logging, LOG_INFO_disabled)
{
  CHECK_LOG_DISABLED(INFO);
}

TEST(logging, LOG_INFO_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_ENABLED(INFO));
}

TEST(logging, LOG_DEBUG_disabled)
{
  CHECK_LOG_DISABLED(DEBUG);
}

TEST(logging, LOG_DEBUG_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_ENABLED(DEBUG));
}

TEST(logging, LOG_VERBOSE_disabled)
{
  CHECK_LOG_DISABLED(VERBOSE);
}

TEST(logging, LOG_VERBOSE_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_LOG_ENABLED(VERBOSE));
}

#undef CHECK_LOG_DISABLED
#undef CHECK_LOG_ENABLED

TEST(logging, LOG_complex_param)
{
#define CHECK_LOG_COMBINATION(use_scoped_log_severity_info, use_logging_severity_info)           \
  {                                                                                              \
    cpputils::base::ScopedLogSeverity sls(                                                       \
        (use_scoped_log_severity_info) ? ::cpputils::base::INFO : ::cpputils::base::WARNING);    \
    CapturedStderr cap;                                                                          \
    LOG((use_logging_severity_info) ? ::cpputils::base::INFO : ::cpputils::base::WARNING)        \
        << "foobar";                                                                             \
    if ((use_scoped_log_severity_info) || !(use_logging_severity_info))                          \
    {                                                                                            \
      ASSERT_NO_FATAL_FAILURE(CheckMessage(                                                      \
          cap, (use_logging_severity_info) ? ::cpputils::base::INFO : ::cpputils::base::WARNING, \
          "foobar"));                                                                            \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
      cap.Stop();                                                                                \
      ASSERT_EQ("", cap.str());                                                                  \
    }                                                                                            \
  }

  CHECK_LOG_COMBINATION(false, false);
  CHECK_LOG_COMBINATION(false, true);
  CHECK_LOG_COMBINATION(true, false);
  CHECK_LOG_COMBINATION(true, true);

#undef CHECK_LOG_COMBINATION
}

TEST(logging, LOG_does_not_clobber_errno)
{
  CapturedStderr cap;
  errno = 12345;
  LOG(INFO) << (errno = 67890);
  EXPECT_EQ(12345, errno) << "errno was not restored";

  ASSERT_NO_FATAL_FAILURE(CheckMessage(cap, cpputils::base::INFO, "67890"));
}

TEST(logging, PLOG_does_not_clobber_errno)
{
  CapturedStderr cap;
  errno = 12345;
  PLOG(INFO) << (errno = 67890);
  EXPECT_EQ(12345, errno) << "errno was not restored";

  ASSERT_NO_FATAL_FAILURE(CheckMessage(cap, cpputils::base::INFO, "67890"));
}

TEST(logging, LOG_does_not_have_dangling_if)
{
  CapturedStderr cap; // So the logging below has no side-effects.

  // Do the test two ways: once where we hypothesize that LOG()'s if
  // will evaluate to true (when severity is high enough) and once when we
  // expect it to evaluate to false (when severity is not high enough).
  bool flag = false;
  if (true)
    LOG(INFO) << "foobar";
  else
    flag = true;

  EXPECT_FALSE(flag) << "LOG macro probably has a dangling if with no else";

  flag = false;
  if (true)
    LOG(VERBOSE) << "foobar";
  else
    flag = true;

  EXPECT_FALSE(flag) << "LOG macro probably has a dangling if with no else";
}

#define CHECK_PLOG_DISABLED(severity)                              \
  {                                                                \
    cpputils::base::ScopedLogSeverity sls1(cpputils::base::FATAL); \
    CapturedStderr cap1;                                           \
    PLOG(severity) << "foo bar";                                   \
    cap1.Stop();                                                   \
    ASSERT_EQ("", cap1.str());                                     \
  }                                                                \
  {                                                                \
    cpputils::base::ScopedLogSeverity sls1(cpputils::base::FATAL); \
    CapturedStderr cap1;                                           \
    PLOG(severity) << "foo bar";                                   \
    cap1.Stop();                                                   \
    ASSERT_EQ("", cap1.str());                                     \
  }

#define CHECK_PLOG_ENABLED(severity)                                                   \
  {                                                                                    \
    cpputils::base::ScopedLogSeverity sls2(cpputils::base::severity);                  \
    CapturedStderr cap2;                                                               \
    errno = ENOENT;                                                                    \
    PLOG(severity) << "foobar";                                                        \
    CheckMessage(cap2, cpputils::base::severity, "foobar: No such file or directory"); \
  }                                                                                    \
  {                                                                                    \
    cpputils::base::ScopedLogSeverity sls2(cpputils::base::severity);                  \
    CapturedStderr cap2;                                                               \
    errno = ENOENT;                                                                    \
    PLOG(severity) << "foobar";                                                        \
    CheckMessage(cap2, cpputils::base::severity, "foobar: No such file or directory"); \
  }

TEST(logging, PLOG_FATAL)
{
  ASSERT_DEATH({SuppressAbortUI(); PLOG(FATAL) << "foobar"; }, "foobar");
  ASSERT_DEATH({SuppressAbortUI(); PLOG(::cpputils::base::FATAL) << "foobar"; }, "foobar");
}

TEST(logging, PLOG_FATAL_WITHOUT_ABORT_disabled)
{
  CHECK_PLOG_DISABLED(FATAL_WITHOUT_ABORT);
}

TEST(logging, PLOG_FATAL_WITHOUT_ABORT_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_PLOG_ENABLED(FATAL_WITHOUT_ABORT));
}

TEST(logging, PLOG_ERROR_disabled)
{
  CHECK_PLOG_DISABLED(ERROR);
}

TEST(logging, PLOG_ERROR_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_PLOG_ENABLED(ERROR));
}

TEST(logging, PLOG_WARNING_disabled)
{
  CHECK_PLOG_DISABLED(WARNING);
}

TEST(logging, PLOG_WARNING_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_PLOG_ENABLED(WARNING));
}

TEST(logging, PLOG_INFO_disabled)
{
  CHECK_PLOG_DISABLED(INFO);
}

TEST(logging, PLOG_INFO_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_PLOG_ENABLED(INFO));
}

TEST(logging, PLOG_DEBUG_disabled)
{
  CHECK_PLOG_DISABLED(DEBUG);
}

TEST(logging, PLOG_DEBUG_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_PLOG_ENABLED(DEBUG));
}

TEST(logging, PLOG_VERBOSE_disabled)
{
  CHECK_PLOG_DISABLED(VERBOSE);
}

TEST(logging, PLOG_VERBOSE_enabled)
{
  ASSERT_NO_FATAL_FAILURE(CHECK_PLOG_ENABLED(VERBOSE));
}

#undef CHECK_PLOG_DISABLED
#undef CHECK_PLOG_ENABLED

TEST(logging, UNIMPLEMENTED)
{
  std::string expected = cpputils::base::StringPrintf("%s unimplemented ", __PRETTY_FUNCTION__);

  CapturedStderr cap;
  errno = ENOENT;
  UNIMPLEMENTED(ERROR);
  ASSERT_NO_FATAL_FAILURE(CheckMessage(cap, cpputils::base::ERROR, expected.c_str()));
}

static void NoopAborter(const char *msg ATTRIBUTE_UNUSED)
{
  LOG(ERROR) << "called noop";
}

TEST(logging, LOG_FATAL_NOOP_ABORTER)
{
  CapturedStderr cap;
  {
    cpputils::base::SetAborter(NoopAborter);

    cpputils::base::ScopedLogSeverity sls(cpputils::base::ERROR);
    LOG(FATAL) << "foobar";
    cap.Stop();

    cpputils::base::SetAborter(cpputils::base::DefaultAborter);
  }
  std::string output = cap.str();
  ASSERT_NO_FATAL_FAILURE(CheckMessage(output, cpputils::base::FATAL, "foobar"));
  ASSERT_NO_FATAL_FAILURE(CheckMessage(output, cpputils::base::ERROR, "called noop"));

  ASSERT_DEATH({SuppressAbortUI(); LOG(FATAL) << "foobar"; }, "foobar");
}

struct CountLineAborter
{
  static void CountLineAborterFunction(const char *msg)
  {
    while (*msg != 0)
    {
      if (*msg == '\n')
      {
        newline_count++;
      }
      msg++;
    }
  }
  static size_t newline_count;
};
size_t CountLineAborter::newline_count = 0;

TEST(logging, LOG_FATAL_ABORTER_MESSAGE)
{
  CountLineAborter::newline_count = 0;
  cpputils::base::SetAborter(CountLineAborter::CountLineAborterFunction);

  cpputils::base::ScopedLogSeverity sls(cpputils::base::ERROR);
  CapturedStderr cap;
  LOG(FATAL) << "foo\nbar";

  EXPECT_EQ(CountLineAborter::newline_count, 1U + 1U); // +1 for final '\n'.
}

__attribute__((constructor)) void TestLoggingInConstructor()
{
  LOG(ERROR) << "foobar";
}

TEST(logging, SetDefaultTag)
{
  constexpr const char *expected_tag = "test_tag";
  constexpr const char *expected_msg = "foobar";
  CapturedStderr cap;
  {
    std::string old_default_tag = cpputils::base::GetDefaultTag();
    cpputils::base::SetDefaultTag(expected_tag);
    cpputils::base::ScopedLogSeverity sls(cpputils::base::LogSeverity::INFO);
    LOG(INFO) << expected_msg;
    cpputils::base::SetDefaultTag(old_default_tag);
  }
  ASSERT_NO_FATAL_FAILURE(
      CheckMessage(cap, cpputils::base::LogSeverity::INFO, expected_msg, expected_tag));
}

TEST(logging, StdioLogger)
{
  CapturedStderr cap_err;
  CapturedStdout cap_out;
  cpputils::base::SetLogger(cpputils::base::StdioLogger);
  LOG(INFO) << "out";
  LOG(ERROR) << "err";
  cap_err.Stop();
  cap_out.Stop();

  // For INFO we expect just the literal "out\n".
  ASSERT_EQ("out\n", cap_out.str());
  // Whereas ERROR logging includes the program name.
  ASSERT_EQ(cpputils::base::Basename(cpputils::base::GetExecutablePath()) + ": err\n", cap_err.str());
}
*/