#ifndef __REPLAY_HPP__
#define __REPLAY_HPP__
#include <string>

class Replay
{
public:
    void CgiReplay();
    void NoCgiReplay();

    void MakeStatusLine();
    void MakeReplayHand();
    void MakeReplayBlank();
    void MakeReplayText();
    
    const std::string & GetReplayLine();
    const std::string & GetReplayHead();
    const std::string & GetReplayBlank();
    const std::string & GetReplayText();
private:
    std::string rep_line_;
    std::string rep_head_;
    std::string rep_blank_;
    std::string rep_text_;

    int code_;
};


#endif
