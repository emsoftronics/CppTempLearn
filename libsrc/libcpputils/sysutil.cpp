#include <cpputils-base/sysutil.h>
#include <cpputils-base/logging.h>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <cpputils-base/strings.h>
#include <fcntl.h>

namespace cpputils
{
    namespace base
    {
        namespace Unix
        {

            Shell::Shell() : m_command(""), m_output_path(""), m_error_msg(""), m_exec_status(true)
            {
                m_output_lines.clear();
                m_shell_available = (system(NULL) == 1) ? true : false;
            }

            Shell::Shell(const std::string &command) : Shell()
            {
                updateCommand(command);
            }

            Shell::~Shell()
            {
                m_output_lines.clear();
            }

            void Shell::updateCommand(const std::string &newCommand)
            {
                std::string s = newCommand;
                std::size_t found = s.rfind('>');
                std::size_t quot = s.rfind('\"');
                if ((found != std::string::npos) &&
                    ((quot == std::string::npos) || (quot < found)))
                {
                    m_command = s.substr(0, found);
                    m_output_path = s.substr(found + 1, s.length() - found - 1);
                    m_output_path = Trim(m_output_path);
                }
                else
                {
                    m_command = newCommand;
                    m_output_path = "";
                }
                m_command = Trim(m_command);
                m_output_lines.clear();
            }

            void Shell::setOutPath(const std::string &path)
            {
                m_output_path = path;
            }

            std::string Shell::getCommand() const
            {
                return m_command;
            }

            std::string Shell::getOutPath() const
            {
                return m_output_path;
            }

            std::string Shell::getErrorMessage() const
            {
                return m_error_msg;
            }

            std::vector<std::string> &Shell::getOutputLines()
            {
                return m_output_lines;
            }

            bool Shell::execute(bool toDevNull)
            {
                if (toDevNull)
                    m_output_path = "/dev/null";

                execCommand();
                return m_exec_status;
            }

            bool Shell::getLastStatus() const
            {
                return m_exec_status;
            }

            bool Shell::isShellAvailable() const
            {
                return m_shell_available;
            }

            void Shell::execCommand()
            {
                m_exec_status = false;

                if (!isShellAvailable())
                {
                    LOG(ERROR) << "system : Shell is not supported.";
                    return;
                }

                m_output_lines.clear();

                if (m_command.empty())
                    return;

                std::string cm = m_command;

                if (!m_output_path.empty() && (m_output_path.find("/dev/null") != std::string::npos))
                {
                    cm += std::string(" > /dev/null");
                    m_exec_status = (system(cm.c_str()) != 0) ? false : true;
                    return;
                }

                int fds[2];
                if (pipe(fds) < 0)
                {
                    LOG(ERROR) << "pipe : " << strerror(errno);
                    return;
                }
                else
                {
                    int opt;
                    opt = fcntl(fds[0], F_GETFL);
                    if (opt < 0)
                    {
                        LOG(ERROR) << "fcntl : " << strerror(errno);
                        close(fds[0]);
                        close(fds[1]);
                        return;
                    }

                    opt |= O_NONBLOCK;

                    if (fcntl(fds[0], F_SETFL, opt) < 0)
                    {
                        LOG(ERROR) << "fcntl : " << strerror(errno);
                        close(fds[0]);
                        close(fds[1]);
                        return;
                    }
                }

                cm += std::string(" 2>&") + std::to_string(fds[1]);
                cm += std::string(" >&") + std::to_string(fds[1]);
                int ret = system(cm.c_str());
                int max_buf_size = 4096;
                char *tbuf = new char[max_buf_size];
                int len = 0;
                std::stringstream ss;
                while ((len = read(fds[0], tbuf, max_buf_size - 1)) > 0)
                {
                    tbuf[len] = '\0';
                    if (ret)
                    {
                        m_error_msg = std::string(tbuf);
                        break;
                    }
                    ss << tbuf;
                }

                std::string line;
                FILE *f = NULL;
                if (!m_output_path.empty() && !ret)
                {
                    if ((f = fopen(m_output_path.c_str(), "w")) == NULL)
                    {
                        LOG(ERROR) << "fopen : " << strerror(errno);
                    }
                }
                while (std::getline(ss, line))
                {
                    m_output_lines.push_back(line);
                    if (f)
                    {
                        fwrite(line.c_str(), sizeof(char), line.size(), f);
                        if (line.size() < max_buf_size - 1)
                            fputc('\n', f);
                    }
                }
                if (f)
                    fclose(f);
                close(fds[0]);
                close(fds[1]);
                m_exec_status = (ret) ? false : true;
            }

        } // namespace Unix

    } // namespace base

} // namespace cpputils
