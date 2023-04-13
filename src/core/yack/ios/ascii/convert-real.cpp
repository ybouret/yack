
#include "yack/ios/ascii/convert.hpp"
#include "yack/arith/ipower.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace ios
    {

        namespace ascii
        {

            namespace
            {
                static const char fn[] = "ascii::to_real: ";

                template <typename T>
                class a2r
                {
                public:
                    inline a2r(const char  *entry,
                               const size_t count,
                               const char  *where) noexcept :
                    expr(entry),
                    curr(entry),
                    last(entry+count),
                    from(where)
                    {
                    }

                    inline ~a2r() noexcept {}


                    const char * const expr;
                    const char *       curr;
                    const char * const last;
                    const char * const from;


                    inline void parse_sign(bool &s, const char *which)
                    {
                        if(curr>=last) throw exception("%sempty content for '%s' while looking for %s sign",fn,from,which);
                        switch(*curr)
                        {
                            case '-':
                                s = true;  // FALLTHRU
                            case '+':
                                ++curr;
                                break;
                            default:
                                break;
                        }
                    }


                    T operator()()
                    {
                        static const T ten(10);
                        static const T tenth(0.1);

                        //------------------------------------------------------
                        //
                        // check float sign
                        //
                        //------------------------------------------------------
                        bool       fneg = false;
                        parse_sign(fneg,"integer part");
                        if(curr>=last) throw exception("%sempty content for '%s'",fn,from);

                        //------------------------------------------------------
                        //
                        // parse integer part
                        //
                        //------------------------------------------------------
                        T          res = 0;
                        {
                            while(curr<last)
                            {
                                const char c = *(curr++);
                                switch(c)
                                {
                                    case '0':
                                    case '1':
                                    case '2':
                                    case '3':
                                    case '4':
                                    case '5':
                                    case '6':
                                    case '7':
                                    case '8':
                                    case '9':
                                        res *= ten;
                                        res += static_cast<T>(c-'0');
                                        break;

                                    case '.':
                                        goto DECIMAL;

                                    case 'e':
                                    case 'E':
                                    case 'd':
                                    case 'D':
                                        goto EXPONENT;

                                    case 'f':
                                    case 'L':
                                        goto DONE;

                                    default:
                                        throw libc::exception(EINVAL,"%sinvalid '%c' in integer part of '%s'",fn,c,expr);
                                }
                            }
                            goto DONE;
                        }

                        //------------------------------------------------------
                        //
                        // parse decimal part
                        //
                        //------------------------------------------------------

                    DECIMAL:
                        {
                            assert('.'==curr[-1]);
                            T              fac(1);
                            while(curr<last)
                            {
                                const char c = *(curr++);
                                switch(c)
                                {
                                    case '0':
                                    case '1':
                                    case '2':
                                    case '3':
                                    case '4':
                                    case '5':
                                    case '6':
                                    case '7':
                                    case '8':
                                    case '9':
                                        fac *= tenth;
                                        res += fac * static_cast<T>(c-'0');
                                        break;

                                    case 'e':
                                    case 'E':
                                    case 'd':
                                    case 'D':
                                        goto EXPONENT;

                                    case 'f':
                                    case 'L':
                                        goto DONE;

                                    default:
                                        throw libc::exception(EINVAL,"%sinvalid '%c' in decimal part of '%s'",fn,c,expr);
                                }
                            }
                            goto DONE;

                        }

                        //------------------------------------------------------
                        //
                        // parse exponent part
                        //
                        //------------------------------------------------------
                    EXPONENT:
                        {
                            bool      eneg = false;
                            unsigned  eval = 0;
                            parse_sign(eneg,"exponent");
                            if(curr>=last) throw exception("%smissing exponent value for '%s'",fn,from);
                            while(curr<last)
                            {
                                const char c = *(curr++);
                                switch(c)
                                {
                                    case '0':
                                    case '1':
                                    case '2':
                                    case '3':
                                    case '4':
                                    case '5':
                                    case '6':
                                    case '7':
                                    case '8':
                                    case '9':
                                        eval *= 10;
                                        eval += static_cast<int>(c-'0');
                                        break;

                                    case 'f':
                                    case 'L':
                                        goto EVAL;

                                    default:
                                        throw libc::exception(EINVAL,"%sinvalid '%c' in exponent of '%s'",fn,c,expr);
                                }
                            }
                        EVAL:
                            //std::cerr << "eval=" << eval << std::endl;
                            if(eval>0)
                            {
                                if(eneg) res *= ipower<T,unsigned>(tenth,eval); else res *= ipower<T,unsigned>(ten,eval);
                            }
                        }

                    DONE:
                        //------------------------------------------------------
                        //
                        // checking extraneous chars
                        //
                        //------------------------------------------------------
                        if(curr<last) throw libc::exception(EINVAL,"%sinvalid extraneous '%c' in '%s'",fn,curr[0],expr);
                        return fneg ? -res : res;
                    }

                private:
                    YACK_DISABLE_COPY_AND_ASSIGN(a2r);
                };

            }

            template <>
            float convert:: real<float>(const char *args, const size_t size, const char *ctx)
            {
                a2r<float> conv(args,size,ctx);
                return conv();
            }

            template <>
            double convert:: real<double>(const char *args, const size_t size, const char *ctx)
            {
                a2r<double> conv(args,size,ctx);
                return conv();
            }

            template <>
            long double convert:: real<long double>(const char *args, const size_t size, const char *ctx)
            {
                a2r<long double> conv(args,size,ctx);
                return conv();
            }


        }

    }

}
