#ifndef __REPLAY_HPP__
#define __REPLAY_HPP__
#include <string>
#include <sstream>
#include "resourse.hpp"

class Replay
{
public:
    int & SetCode()
    {
        return code_;
    }
    void CgiReplay();
    void NoCgiReplay();

    void MakeStatusLine();
    void MakeReplayHand(bool cgi, Resourse & res);
    void MakeReplayBlank();
    std::string & MakeReplayText();
    static std::string IntToString(int n)
    {
        std::stringstream ss; 
        ss << n;
        return ss.str();
    }
    
    const std::string & GetReplayLine()
    {
        return rep_line_;
    }
    const std::string & GetReplayHead()
    {
        return rep_head_;
    }
    const std::string & GetReplayBlank()
    {
        return rep_blank_;
    }
    const std::string & GetReplayText()
    {
        return rep_text_;
    }
    
private:
    std::string rep_line_;
    std::string rep_head_;
    std::string rep_blank_;
    std::string rep_text_;

    int code_;
};


#endif
