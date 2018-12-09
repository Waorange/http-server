#include "replay.hpp"
#include <unordered_map> 

#define VERSION "HTTP/1.0"

static std::unordered_map<int, std::string> codetodes_map{
    {200, "OK"},
    {204, "No Content"},
    {206, "Partial Content"},
    {400, "Bad Request"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {500, "Internal Server Error"},
};


void Replay::MakeStatusLine()
{
    rep_line_ += VERSION;
    rep_line_ += " ";
    rep_line_ += IntToString(code_);
    rep_line_ += " ";
    rep_line_ += codetodes_map[code_];
    rep_line_ += '\n';
}

void Replay::MakeReplayHand(bool cgi, Resourse & res)
{
    if(!cgi)
    {
        rep_head_ += "Content-Length: ";
        rep_head_ += IntToString(res.GetResSize());
        rep_head_ += '\n';
        rep_head_ += "Content-Type: ";
        rep_head_ += res.GetResType();
        rep_head_ += '\n';
    }
    else
    {
        rep_head_ += "Content-Length: ";
        rep_head_ += IntToString(rep_text_.size());
        rep_head_ += '\n';
        rep_head_ += "Content-Type: text/html\n";
    }
}
void Replay::MakeReplayBlank()
{
    rep_blank_ += '\n';
}
std::string & Replay::MakeReplayText()
{
    return rep_text_;
}
