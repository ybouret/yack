#include "yack/ios/ascii/convert.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace ios
    {
        
        namespace ascii
        {
            
            void convert:: out_of_range_signed(const size_t bytes, const char *ctx)
            {
                throw libc::exception(EDOM,"out of range int%u_t for %s",unsigned(8*bytes),ctx?ctx:yack_unknown);
            }
            
            void convert:: out_of_range_unsigned(const size_t bytes, const char *ctx)
            {
                throw libc::exception(EDOM,"out of range uint%u_t for %s",unsigned(8*bytes),ctx?ctx:yack_unknown);
            }
            
            int64_t convert::to_int64(const char *args, const char *ctx)
            {
                assert(args);
                return to_int64(args,strlen(args),ctx);
            }
            
            int64_t convert::to_int64(const string &data, const char *ctx)
            {
                return to_int64(data(),data.size(),ctx);
            }
            
            int64_t convert::to_int64(const char *args, size_t size, const char *ctx)
            {
                static const char     fn[] = "ascii::convert::to_int";
                static const uint64_t max_pos = uint64_t(integral_for<int64_t>::maximum);
                static const uint64_t max_neg = max_pos+1;
                
                assert(!(NULL==args&&size>0));
                if(size<=0) throw libc::exception(EINVAL,"%s empty argument for %s",fn,ctx?ctx:yack_unknown);
                bool     neg=false;
                uint64_t res=0;
                
                switch(args[0])
                {
                    case '-':
                        neg = true; // FALLTHRU
                    case '+':
                        ++args;
                        --size;
                        if(size<=0) throw libc::exception(EINVAL,"%s empty argument after '%c' for %s",fn,args[0],ctx?ctx:yack_unknown);
                        break;
                    default:
                        break;
                }
                
                assert(size>0);
                while(size-- > 0)
                {
                    const char c = *(args++);
                    res *= 10;
                    switch(c)
                    {
                        case '0': break;
                        case '1': ++res;    break;
                        case '2': res += 2; break;
                        case '3': res += 3; break;
                        case '4': res += 4; break;
                        case '5': res += 5; break;
                        case '6': res += 6; break;
                        case '7': res += 7; break;
                        case '8': res += 8; break;
                        case '9': res += 9; break;
                        default:
                            throw libc::exception(EINVAL,"%s invalid char '%s' for %s",fn, hybrid[uint8_t(c)], ctx?ctx:yack_unknown);
                    }
                    if(neg)
                    {
                        if(res>max_neg) throw libc::exception(ERANGE,"%s underflow for %s",fn,ctx?ctx:yack_unknown);
                    }
                    else
                    {
                        if(res>max_pos) throw libc::exception(ERANGE,"%s overflow for %s",fn,ctx?ctx:yack_unknown);
                    }
                }
                
                return neg ? -int64_t(res): int64_t(res);
            }
            
        }
    }
}


namespace yack
{
    
    namespace ios
    {
        
        namespace ascii
        {
            
            uint64_t convert::to_uint64(const char *args, const char *ctx)
            {
                assert(args);
                return to_uint64(args,strlen(args),ctx);
            }
            
            uint64_t convert::to_uint64(const string &data, const char *ctx)
            {
                return to_uint64(data(),data.size(),ctx);
            }
            
            uint64_t convert:: to_uint64(const char *args, size_t size, const char *ctx)
            {
                static const char     fn[] = "ascii::convert::to_unsigned";
                
                assert(!(NULL==args&&size>0));
                if(size<=0) throw libc::exception(EINVAL,"%s empty argument for %s",fn,ctx?ctx:yack_unknown);
                uint64_t res=0;
                while(size-- > 0)
                {
                    const char     c   = *(args++);
                    const uint64_t old = res;
                    res *= 10;
                    switch(c)
                    {
                        case '0': break;
                        case '1': ++res;    break;
                        case '2': res += 2; break;
                        case '3': res += 3; break;
                        case '4': res += 4; break;
                        case '5': res += 5; break;
                        case '6': res += 6; break;
                        case '7': res += 7; break;
                        case '8': res += 8; break;
                        case '9': res += 9; break;
                        default:
                            throw libc::exception(EINVAL,"%s invalid char '%s' for %s",fn, hybrid[uint8_t(c)], ctx?ctx:yack_unknown);
                    }
                    if(res<old) throw libc::exception(EINVAL,"%s overflow for %s",fn,ctx?ctx:yack_unknown);
                }
                return res;
            }
        }
    }
}


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
                               const char  *where) throw() :
                    expr(entry),
                    curr(entry),
                    last(entry+count),
                    from(where),
                    fneg(false)
                    {
                    }
                    
                    inline ~a2r() throw() {}
                    
                    
                    const char * const expr;
                    const char *       curr;
                    const char * const last;
                    const char * const from;
                    
                    bool               fneg;
                    
                    void parse_sign()
                    {
                        if(curr<=last) throw exception("%sempty content for '%s'",fn,from);
                        switch(*curr)
                        {
                            case '-':
                                fneg = true;
                            case '+':
                                ++curr;
                                break;
                            default:
                                break;
                        }
                    }
                    
                    T operator()()
                    {
                        // check sign
                        parse_sign();
                    
                        
                        
                        return 0;
                    }
                    
                private:
                    YACK_DISABLE_COPY_AND_ASSIGN(a2r);
                };
                
            }
            
            template <>
            float convert::to_real<float>(const char *args, const size_t size, const char *ctx)
            {
                a2r<float> conv(args,size,ctx);
                return conv();
            }
            
            template <>
            double convert::to_real<double>(const char *args, const size_t size, const char *ctx)
            {
                a2r<double> conv(args,size,ctx);
                return conv();
            }
            
            template <>
            long double convert::to_real<long double>(const char *args, const size_t size, const char *ctx)
            {
                a2r<long double> conv(args,size,ctx);
                return conv();
            }
            
            
        }
        
    }
    
}

