
#pragma once
#include <string>
#include <vector>
#include <cpputils-base/macros.h>

namespace cpputils
{
    namespace base
    {
        namespace Unix
        {
            class Shell
            {
            public:
                Shell();
                Shell(const std::string &command);
                ~Shell();
                void updateCommand(const std::string &newCommand);
                void setOutPath(const std::string &path);
                std::string getCommand() const;
                std::string getOutPath() const;
                std::string getErrorMessage() const;
                std::vector<std::string> &getOutputLines();
                bool execute(bool toDevNull = false);
                bool getLastStatus() const;
                bool isShellAvailable() const;

            private:
                DISALLOW_COPY_AND_ASSIGN(Shell);

            protected:
                void execCommand();

            protected:
                std::string m_command;
                std::string m_output_path;
                std::vector<std::string> m_output_lines;
                std::string m_error_msg;
                bool m_exec_status;
                bool m_shell_available;
            };
        } // namespace Unix

    } // namespace base

} // namespace cpputils
