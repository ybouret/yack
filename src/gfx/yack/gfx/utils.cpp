#include "yack/gfx/utils.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/exception.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/string/ops.hpp"

namespace yack
{
    namespace graphic
    {

        static inline
        const string & sclr(string &s)
        {
            return strops::clean_with(" \t",2,s);
        }
        
        coord command_line:: WxH(const string &args)
        {
            vector<string> words;
            tokenizer::split_with('x',words,args);
            if(words.size()<2) throw exception("command_line::WxH(invalid metrics)");


            const unit_t width  = ios::ascii::convert::to<unit_t>(sclr(words[1]),"width");
            const unit_t height = ios::ascii::convert::to<unit_t>(sclr(words[2]),"height");

            return coord(width,height);
        }

        coord command_line:: WxH(const char *txt)
        {
            const string _(txt);
            return WxH(_);
        }

    }

}

