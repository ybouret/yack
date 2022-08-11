
#if 0
namespace
{

    static const char * sp_term[] =
    {
        "NAME", //!< 0
        "+",    //!< 1
        "-"     //!< 2
    };


    //! translator for
    class sp_linker  : public large_object, public syntax::translator
    {
    public:

        //! setup with keywords
        inline explicit sp_linker() : syntax::translator(),
        thash( YACK_HASHING_PERFECT(sp_term) )
        {
        }

        //! cleanup
        inline virtual ~sp_linker() throw() {}

        inline virtual void on_terminal(const lexeme &lxm)
        {
            assert(NULL!=data);
            const string     &tid = *lxm.name;
            nucleus::sp_info &msg = *static_cast<nucleus::sp_info *>(data);

            switch( thash(tid) )
            {
                case 0: assert(sp_term[0]==tid);
                    msg.name = lxm.data.to_string();
                    break;

                case 1:assert(sp_term[1]==tid);
                    ++msg.z;
                    break;

                case 2:assert(sp_term[2]==tid);
                    --msg.z;
                    break;

                default:
                    raise_error(tid);
            }
        }
        
        inline virtual void on_internal(const string &uid, const size_t n)
        {
            assert(NULL!=data);
            if(uid!="SPECIES") raise_error(uid);
            nucleus::sp_info &msg = *static_cast<nucleus::sp_info *>(data);
            if(n != size_t(abs(msg.z)+1) ) raise_error(msg.name);
        }





    private:
        YACK_DISABLE_COPY_AND_ASSIGN(sp_linker);
        const hashing::perfect thash;

        static inline void raise_error(const string &word)
        {
            throw imported::exception(designer::call_sign,"corrupted species linker with <%s>",word());
        }

    };

}
#endif
