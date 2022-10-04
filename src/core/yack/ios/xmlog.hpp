//! \file

#ifndef YACK_IOS_XMLOG_INCLUDED
#define YACK_IOS_XMLOG_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{

    class xmlog
    {
    public:
        template <typename PREFIX> inline
        explicit xmlog(const PREFIX &p,
                       std::ostream &f,
                       const bool   &v) :
        prefix(p),
        output(f),
        indent(0),
        verbose(v)
        {
        }


        std::ostream & operator*() const;

        virtual ~xmlog() throw();

        virtual  void incr() const throw();
        virtual  void decr() const throw();
        

        class msg
        {
        public:
            template <
            typename MARKUP,
            typename OPTION>
            explicit msg(const xmlog  &parent,
                         const MARKUP &markup,
                         const OPTION &option) :
            host(parent),
            mark(markup)
            {
                if(host.verbose)
                {
                    const string  opt(option);
                    std::ostream &os = *host;
                    os << '<' << mark;
                    if(opt.size()) os << ' ' << opt;
                    os << '>' << std::endl;
                }
                host.incr();
            }

            virtual ~msg() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(msg);
            const xmlog  &host;
        public:
            const string  mark;
        };

        const string  prefix;
    private:
        std::ostream &output;
        const int     indent;
    public:
        const bool   &verbose;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(xmlog);
    };
    


}

#endif

