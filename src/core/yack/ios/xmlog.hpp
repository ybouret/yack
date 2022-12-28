//! \file

#ifndef YACK_IOS_XMLOG_INCLUDED
#define YACK_IOS_XMLOG_INCLUDED 1

#include "yack/string.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! quick helper to display XML-style logging
    //
    //__________________________________________________________________________
    class xmlog
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        typedef auto_ptr<const string> strptr; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! constructor with prefix, stream, an reference to verbosity
        template <typename PREFIX> inline
        explicit xmlog(const PREFIX &p,
                       std::ostream &f,
                       const bool   &v) :
        verbose(v),
        prefix( verbose ? new string(p) : NULL),
        output(f),
        indent(0)
        {
        }

        //! cleanup
        virtual ~xmlog() throw();

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        std::ostream & operator*()      const; //!< return formatted/indented ostream
        virtual void   incr()   const throw(); //!< increase indentation
        virtual void   decr()   const throw(); //!< decrease indentatiomn
        std::ostream & operator()(void) const; //!< return raw ostream

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const bool   &verbose; //!< verbosity from user
        const strptr  prefix;  //!< prefix for output
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(xmlog);
        std::ostream &output;
        const int     indent;
    public:

        //______________________________________________________________________
        //
        //! subsection start/end
        //______________________________________________________________________
        class msg
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! create a default parent mark
            template <
            typename MARKUP>
            explicit msg(const xmlog  &parent,
                         const MARKUP &markup) :
            host(parent),
            lone(false),
            mark( host.verbose ? new string(markup) : NULL )
            {
                if(host.verbose)
                {
                    std::ostream &os = *host;
                    os << '<' << mark  << '>' << std::endl;
                }
                host.incr();
            }

            //! create a parent mark with option.
            /**
             if option ends with a slash, assume it's a
             standalone mark.
             */
            template <
            typename MARKUP,
            typename OPTION>
            explicit msg(const xmlog  &parent,
                         const MARKUP &markup,
                         const OPTION &option) :
            host(parent),
            lone(false),
            mark(host.verbose ? new string(markup) : NULL )
            {
                if(host.verbose)
                {
                    const string  opt(option);
                    std::ostream &os = *host;
                    os << '<' << mark;
                    if(opt.size())
                    {
                        if('/'==opt.back())
                        {
                            os << opt;
                            coerce(lone) = true;
                        }
                        else
                            os << opt;
                    }
                    os << '>' << std::endl;
                }
                host.incr();
            }

            //! cleanup
            virtual ~msg() throw();



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(msg);
            const xmlog  &host;
            const bool    lone;
            const strptr  mark;
        };
    };

    //__________________________________________________________________________
    //
    //! send a message to the host
    //__________________________________________________________________________
#define YACK_XMLOG(HOST,MSG) do {\
const yack::xmlog &host = (HOST); if(host.verbose) { *host << MSG << std::endl; }\
} while(false)


    //__________________________________________________________________________
    //
    //! create the msg name
    //__________________________________________________________________________
#define YACK_XMLSUB__(X,Y) X##Y

    //__________________________________________________________________________
    //
    //! instantiate the msg name
    //__________________________________________________________________________
#define YACK_XMLSUB0_(HOST,ID,MARKUP) volatile yack::xmlog::msg  YACK_XMLSUB__(__msg,ID)(HOST,MARKUP)

    //__________________________________________________________________________
    //
    //! instantiate the msg name+opt
    //__________________________________________________________________________
#define YACK_XMLSUB1_(HOST,ID,MARKUP,OPTION) volatile yack::xmlog::msg  YACK_XMLSUB__(__msg,ID)(HOST,MARKUP,OPTION)


    //__________________________________________________________________________
    //! start a sub-level
    //__________________________________________________________________________
#define YACK_XMLSUB(HOST,MARKUP) YACK_XMLSUB0_(HOST,__LINE__,MARKUP)

    //__________________________________________________________________________
    //
    //! start a sub-level with option
    //__________________________________________________________________________
#define YACK_XMLSUB_OPT(HOST,MARKUP,OPTION) YACK_XMLSUB1_(HOST,__LINE__,MARKUP,OPTION)

    //__________________________________________________________________________
    //
    //! single mark
    //__________________________________________________________________________
#define YACK_XMLOUT(HOST,MARKUP) do{ YACK_XMLSUB1_(HOST,__LINE__,MARKUP,'/'); } while(false)


}

#endif

