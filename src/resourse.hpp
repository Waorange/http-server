#ifndef __RESOURSE_HPP__
#define __RESOURSE_HPP__
#include <string>
#include <unordered_map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define HOME_PAGE "index.html"

static std::unordered_map<std::string, std::string> suffixtotype_map{
    {".html","text/html"},
    {".htm", "text/html"},
    {".css", "text/css"},
    {".js", "application/x-javascript"},
    {".jpg", "image/jpeg"},
    {".png", "image/png"},
};

class Resourse
{
public:
    Resourse()
        :path_("httproot")
        ,size_(0)
    {}
    std::string & SetPath()
    {
        return path_;
    }
    const std::string & GetPath()
    {
        return path_;
    }
    bool IsPathLegal(bool & cgi)
    {
        //获取文件属性
        struct stat st_;
        if(stat(path_.c_str(), &st_) < 0)
        {
            return false;
        }
        if((st_.st_mode & S_IFMT) == S_IFDIR)
        {
            path_ += '/';
            path_ += HOME_PAGE;
        }
        else
        {
            //只要有任意可执行权限则需要cgi进行处理
            if((st_.st_mode & S_IXUSR) ||\
                    st_.st_mode & S_IXGRP ||\
                    st_.st_mode & S_IXOTH)
            {
                cgi = true;
            }
        }
        size_ = st_.st_size;
        return true;
    }
    const std::string GetResType()
    {
        std::string suffix_;
        size_t pos_ = path_.find('.');
        if(pos_ != std::string::npos)
        {
            suffix_ = path_.substr(pos_);
        }
        type_ = suffixtotype_map[suffix_];
        return type_;
    }
    int GetResSize()
    {
        return size_;
    }
private:
    std::string path_;
    std::string type_;
    int size_;
};

#endif
