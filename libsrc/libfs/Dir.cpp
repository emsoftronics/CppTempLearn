#include <sys-fs/Dir.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdio>
#include <cpputils-base/logging.h>
#include <cpputils-base/strings.h>

using namespace cpputils::base;

namespace Sys
{
    namespace Fs
    {
        Dir::Dir()
            : FsObject::FsObject()
        {
            m_type = FsObject::Type::DIRECTORY;
            m_lastUpdate = 0;
        }

        Dir::Dir(const Dir &fsDir)
            : FsObject::FsObject(fsDir)
        {
            m_type = FsObject::Type::DIRECTORY;
            m_lastUpdate = 0;
        }

        Dir::Dir(Dir &&fsDir)
            : FsObject::FsObject(fsDir)
        {
            m_type = FsObject::Type::DIRECTORY;
            m_lastUpdate = 0;
        }

        Dir::Dir(const std::string &fsPath)
            : FsObject::FsObject(fsPath)
        {
            m_type = FsObject::Type::DIRECTORY;
            m_lastUpdate = 0;
        }

        Dir::Dir(std::string &&fsPath)
            : FsObject::FsObject(fsPath)
        {
            m_type = FsObject::Type::DIRECTORY;
            m_lastUpdate = 0;
        }

        Dir::Dir(const char *fsPath)
            : FsObject::FsObject(fsPath)
        {
            m_type = FsObject::Type::DIRECTORY;
            m_lastUpdate = 0;
        }

        Dir::~Dir()
        {
            m_entries.clear();
        }

        Dir &Dir::operator=(const Dir &Path)
        {
            Path::operator=(Path);
            m_lastUpdate = Path.m_lastUpdate;
            return *this;
        }

        Dir &Dir::operator=(Dir &&Path)
        {
            Path::operator=(Path);
            m_lastUpdate = 0;
            return *this;
        }

        std::vector<FsObject> &Dir::getEntries()
        {
            time_t mtime = getLastUpdateTime();
            if (m_lastUpdate != mtime)
            {
                m_lastUpdate = mtime;
                m_entries.clear();
                std::vector<std::string> outlines;
                std::string rpath = getAbsolutePath();
                if (execCommand("ls " + m_realPath + "/", &outlines))
                {
                    for (std::string line : outlines)
                    {
                        line = Trim(line);
                        if (!line.empty())
                        {
                            line = rpath + "/" + line;
                            FsObject fo(line);
                            if (fo.isExist())
                            {
                                m_entries.push_back(fo);
                            }
                        }
                    }
                }
            }
            return m_entries;
        }

    } // namespace Fs

} // namespace Sys
