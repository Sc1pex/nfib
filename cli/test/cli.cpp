#include <gtest/gtest.h>
#include <vector>

extern "C" {
#include "cli.h"
}

std::vector<std::string> split_space(std::string s) {
    std::vector<std::string> res;
    size_t start = 0, end;

    while ((end = s.find(" ", start)) != std::string::npos) {
        auto word = s.substr(start, end - start);
        res.push_back(word);

        // " " is one character
        start = end + 1;
    }
    res.push_back(s.substr(start));

    return res;
}

std::vector<char*> convert_to_argv(std::vector<std::string>& v) {
    std::vector<char*> res;
    for (auto& s : v) {
        res.push_back(const_cast<char*>(s.c_str()));
    }
    return res;
}

TEST(Cli, only_required) {
    auto argv_str = "nfib 69..420 matrix";
    auto argv = split_space(argv_str);
    auto cargv = convert_to_argv(argv);

    Cli c;
    EXPECT_EQ(cli_parse(&c, cargv.size(), &cargv[0]), true);

    EXPECT_EQ(c.min_num, 69);
    EXPECT_EQ(c.max_num, 420);

    EXPECT_FALSE(cli_impl(&c, CliImpl::Naive));
    EXPECT_TRUE(cli_impl(&c, CliImpl::Matrix));

    EXPECT_EQ(c.runs, 3);

    EXPECT_EQ(c.csv_file_output, nullptr);

    EXPECT_EQ(c.threads, (int) sysconf(_SC_NPROCESSORS_ONLN) - 1);
}

TEST(Cli, all) {
    auto argv_str = "nfib 69..420 naive -r 10 -e output.csv -t 16";
    auto argv = split_space(argv_str);
    auto cargv = convert_to_argv(argv);

    Cli c;
    EXPECT_EQ(cli_parse(&c, cargv.size(), &cargv[0]), true);

    EXPECT_EQ(c.min_num, 69);
    EXPECT_EQ(c.max_num, 420);

    EXPECT_TRUE(cli_impl(&c, CliImpl::Naive));
    EXPECT_FALSE(cli_impl(&c, CliImpl::Matrix));

    EXPECT_EQ(c.runs, 10);

    EXPECT_TRUE(std::string("output.csv") == c.csv_file_output);

    EXPECT_EQ(c.threads, 16);
}
