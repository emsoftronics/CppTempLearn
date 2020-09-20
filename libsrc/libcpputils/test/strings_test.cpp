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

#include "cpputils-base/strings.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <set>
#include <unordered_set>

TEST(strings, split_empty) {
  std::vector<std::string> parts = cpputils::base::Split("", ",");
  ASSERT_EQ(1U, parts.size());
  ASSERT_EQ("", parts[0]);
}

TEST(strings, split_single) {
  std::vector<std::string> parts = cpputils::base::Split("foo", ",");
  ASSERT_EQ(1U, parts.size());
  ASSERT_EQ("foo", parts[0]);
}

TEST(strings, split_simple) {
  std::vector<std::string> parts = cpputils::base::Split("foo,bar,baz", ",");
  ASSERT_EQ(3U, parts.size());
  ASSERT_EQ("foo", parts[0]);
  ASSERT_EQ("bar", parts[1]);
  ASSERT_EQ("baz", parts[2]);
}

TEST(strings, split_with_empty_part) {
  std::vector<std::string> parts = cpputils::base::Split("foo,,bar", ",");
  ASSERT_EQ(3U, parts.size());
  ASSERT_EQ("foo", parts[0]);
  ASSERT_EQ("", parts[1]);
  ASSERT_EQ("bar", parts[2]);
}

TEST(strings, split_with_trailing_empty_part) {
  std::vector<std::string> parts = cpputils::base::Split("foo,bar,", ",");
  ASSERT_EQ(3U, parts.size());
  ASSERT_EQ("foo", parts[0]);
  ASSERT_EQ("bar", parts[1]);
  ASSERT_EQ("", parts[2]);
}

TEST(strings, split_null_char) {
  std::vector<std::string> parts =
      cpputils::base::Split(std::string("foo\0bar", 7), std::string("\0", 1));
  ASSERT_EQ(2U, parts.size());
  ASSERT_EQ("foo", parts[0]);
  ASSERT_EQ("bar", parts[1]);
}

TEST(strings, split_any) {
  std::vector<std::string> parts = cpputils::base::Split("foo:bar,baz", ",:");
  ASSERT_EQ(3U, parts.size());
  ASSERT_EQ("foo", parts[0]);
  ASSERT_EQ("bar", parts[1]);
  ASSERT_EQ("baz", parts[2]);
}

TEST(strings, split_any_with_empty_part) {
  std::vector<std::string> parts = cpputils::base::Split("foo:,bar", ",:");
  ASSERT_EQ(3U, parts.size());
  ASSERT_EQ("foo", parts[0]);
  ASSERT_EQ("", parts[1]);
  ASSERT_EQ("bar", parts[2]);
}

TEST(strings, trim_empty) {
  ASSERT_EQ("", cpputils::base::Trim(""));
}

TEST(strings, trim_already_trimmed) {
  ASSERT_EQ("foo", cpputils::base::Trim("foo"));
}

TEST(strings, trim_left) {
  ASSERT_EQ("foo", cpputils::base::Trim(" foo"));
}

TEST(strings, trim_right) {
  ASSERT_EQ("foo", cpputils::base::Trim("foo "));
}

TEST(strings, trim_both) {
  ASSERT_EQ("foo", cpputils::base::Trim(" foo "));
}

TEST(strings, trim_no_trim_middle) {
  ASSERT_EQ("foo bar", cpputils::base::Trim("foo bar"));
}

TEST(strings, trim_other_whitespace) {
  ASSERT_EQ("foo", cpputils::base::Trim("\v\tfoo\n\f"));
}

TEST(strings, join_nothing) {
  std::vector<std::string> list = {};
  ASSERT_EQ("", cpputils::base::Join(list, ','));
}

TEST(strings, join_single) {
  std::vector<std::string> list = {"foo"};
  ASSERT_EQ("foo", cpputils::base::Join(list, ','));
}

TEST(strings, join_simple) {
  std::vector<std::string> list = {"foo", "bar", "baz"};
  ASSERT_EQ("foo,bar,baz", cpputils::base::Join(list, ','));
}

TEST(strings, join_separator_in_vector) {
  std::vector<std::string> list = {",", ","};
  ASSERT_EQ(",,,", cpputils::base::Join(list, ','));
}

TEST(strings, join_simple_ints) {
  std::set<int> list = {1, 2, 3};
  ASSERT_EQ("1,2,3", cpputils::base::Join(list, ','));
}

TEST(strings, join_unordered_set) {
  std::unordered_set<int> list = {1, 2};
  ASSERT_TRUE("1,2" == cpputils::base::Join(list, ',') ||
              "2,1" == cpputils::base::Join(list, ','));
}

TEST(strings, StartsWith_empty) {
  ASSERT_FALSE(cpputils::base::StartsWith("", "foo"));
  ASSERT_TRUE(cpputils::base::StartsWith("", ""));
}

TEST(strings, StartsWithIgnoreCase_empty) {
  ASSERT_FALSE(cpputils::base::StartsWithIgnoreCase("", "foo"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("", ""));
}

TEST(strings, StartsWith_simple) {
  ASSERT_TRUE(cpputils::base::StartsWith("foo", ""));
  ASSERT_TRUE(cpputils::base::StartsWith("foo", "f"));
  ASSERT_TRUE(cpputils::base::StartsWith("foo", "fo"));
  ASSERT_TRUE(cpputils::base::StartsWith("foo", "foo"));
}

TEST(strings, StartsWithIgnoreCase_simple) {
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", ""));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "f"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "F"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "fo"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "fO"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "Fo"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "FO"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "foo"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "foO"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "fOo"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "fOO"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "Foo"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "FoO"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "FOo"));
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("foo", "FOO"));
}

TEST(strings, StartsWith_prefix_too_long) {
  ASSERT_FALSE(cpputils::base::StartsWith("foo", "foobar"));
}

TEST(strings, StartsWithIgnoreCase_prefix_too_long) {
  ASSERT_FALSE(cpputils::base::StartsWithIgnoreCase("foo", "foobar"));
  ASSERT_FALSE(cpputils::base::StartsWithIgnoreCase("foo", "FOOBAR"));
}

TEST(strings, StartsWith_contains_prefix) {
  ASSERT_FALSE(cpputils::base::StartsWith("foobar", "oba"));
  ASSERT_FALSE(cpputils::base::StartsWith("foobar", "bar"));
}

TEST(strings, StartsWithIgnoreCase_contains_prefix) {
  ASSERT_FALSE(cpputils::base::StartsWithIgnoreCase("foobar", "oba"));
  ASSERT_FALSE(cpputils::base::StartsWithIgnoreCase("foobar", "OBA"));
  ASSERT_FALSE(cpputils::base::StartsWithIgnoreCase("foobar", "bar"));
  ASSERT_FALSE(cpputils::base::StartsWithIgnoreCase("foobar", "BAR"));
}

TEST(strings, StartsWith_char) {
  ASSERT_FALSE(cpputils::base::StartsWith("", 'f'));
  ASSERT_TRUE(cpputils::base::StartsWith("foo", 'f'));
  ASSERT_FALSE(cpputils::base::StartsWith("foo", 'o'));
}

TEST(strings, EndsWith_empty) {
  ASSERT_FALSE(cpputils::base::EndsWith("", "foo"));
  ASSERT_TRUE(cpputils::base::EndsWith("", ""));
}

TEST(strings, EndsWithIgnoreCase_empty) {
  ASSERT_FALSE(cpputils::base::EndsWithIgnoreCase("", "foo"));
  ASSERT_FALSE(cpputils::base::EndsWithIgnoreCase("", "FOO"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("", ""));
}

TEST(strings, EndsWith_simple) {
  ASSERT_TRUE(cpputils::base::EndsWith("foo", ""));
  ASSERT_TRUE(cpputils::base::EndsWith("foo", "o"));
  ASSERT_TRUE(cpputils::base::EndsWith("foo", "oo"));
  ASSERT_TRUE(cpputils::base::EndsWith("foo", "foo"));
}

TEST(strings, EndsWithIgnoreCase_simple) {
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", ""));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "o"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "O"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "oo"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "oO"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "Oo"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "OO"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "foo"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "foO"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "fOo"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "fOO"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "Foo"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "FoO"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "FOo"));
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("foo", "FOO"));
}

TEST(strings, EndsWith_prefix_too_long) {
  ASSERT_FALSE(cpputils::base::EndsWith("foo", "foobar"));
}

TEST(strings, EndsWithIgnoreCase_prefix_too_long) {
  ASSERT_FALSE(cpputils::base::EndsWithIgnoreCase("foo", "foobar"));
  ASSERT_FALSE(cpputils::base::EndsWithIgnoreCase("foo", "FOOBAR"));
}

TEST(strings, EndsWith_contains_prefix) {
  ASSERT_FALSE(cpputils::base::EndsWith("foobar", "oba"));
  ASSERT_FALSE(cpputils::base::EndsWith("foobar", "foo"));
}

TEST(strings, EndsWithIgnoreCase_contains_prefix) {
  ASSERT_FALSE(cpputils::base::EndsWithIgnoreCase("foobar", "OBA"));
  ASSERT_FALSE(cpputils::base::EndsWithIgnoreCase("foobar", "FOO"));
}

TEST(strings, StartsWith_std_string) {
  ASSERT_TRUE(cpputils::base::StartsWith("hello", std::string{"hell"}));
  ASSERT_FALSE(cpputils::base::StartsWith("goodbye", std::string{"hell"}));
}

TEST(strings, StartsWithIgnoreCase_std_string) {
  ASSERT_TRUE(cpputils::base::StartsWithIgnoreCase("HeLlO", std::string{"hell"}));
  ASSERT_FALSE(cpputils::base::StartsWithIgnoreCase("GoOdByE", std::string{"hell"}));
}

TEST(strings, EndsWith_std_string) {
  ASSERT_TRUE(cpputils::base::EndsWith("hello", std::string{"lo"}));
  ASSERT_FALSE(cpputils::base::EndsWith("goodbye", std::string{"lo"}));
}

TEST(strings, EndsWithIgnoreCase_std_string) {
  ASSERT_TRUE(cpputils::base::EndsWithIgnoreCase("HeLlO", std::string{"lo"}));
  ASSERT_FALSE(cpputils::base::EndsWithIgnoreCase("GoOdByE", std::string{"lo"}));
}

TEST(strings, EndsWith_char) {
  ASSERT_FALSE(cpputils::base::EndsWith("", 'o'));
  ASSERT_TRUE(cpputils::base::EndsWith("foo", 'o'));
  ASSERT_FALSE(cpputils::base::EndsWith("foo", "f"));
}

TEST(strings, EqualsIgnoreCase) {
  ASSERT_TRUE(cpputils::base::EqualsIgnoreCase("foo", "FOO"));
  ASSERT_TRUE(cpputils::base::EqualsIgnoreCase("FOO", "foo"));
  ASSERT_FALSE(cpputils::base::EqualsIgnoreCase("foo", "bar"));
  ASSERT_FALSE(cpputils::base::EqualsIgnoreCase("foo", "fool"));
}

TEST(strings, ubsan_28729303) {
  cpputils::base::Split("/dev/null", ":");
}
