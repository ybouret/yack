#include "yack/ios/ascii/loader.hpp"
#include "yack/exception.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/string/tokenizer.hpp"

namespace yack
{

    namespace ios
    {
        namespace ascii
        {

            loader::column:: ~column() throw()
            {
            }

            void loader::column:: free() throw()
            {
                drop(*this);
            }

            void loader::column:: push(const char *buff,const size_t size, const char *ctx)
            {
                grow(*this,buff,size,ctx);
            }
            

            int loader::column:: compare(const column *lhs, const column *rhs) throw()
            {
                return comparison::increasing(lhs->indx,rhs->indx);
            }

            loader:: ~loader() throw()
            {
            }

            loader:: loader() throw() : cols()
            {
            }

            void loader:: check_index(const size_t  indx,
                                      const string &name) const
            {
                if(indx<=0) throw exception("zero index for sequence<%s>",name());
            }



            void loader:: use(column *col) throw()
            {
                cols.push_back(col);
                merge_list_of<column>::sort(cols,column::compare);
            }


            static inline bool is_sep(const int ch) throw()
            {
                return ' ' == ch || '\t' == ch;
            }

            size_t loader::load(ios::istream &fp,size_t nskip)
            {

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                for(column *col=cols.head;col;col=col->next)
                {
                    col->free();
                }
                size_t iline=0;

                //--------------------------------------------------------------
                //
                // skip
                //
                //--------------------------------------------------------------
                characters line;
                while(nskip-- > 0)
                {
                    if(!fp.gets(line)) return 0;
                    ++iline;
                }

                //--------------------------------------------------------------
                //
                // load
                //
                //--------------------------------------------------------------
                size_t     res = 0;
                while(fp.gets(line))
                {
                    ++iline;
                    if(line.size<=0||'#'== **line.head) continue;

                    ++res;
                    const string tmp = line.to_string();
                    tokenizer    cut(tmp);
                    column      *col = cols.head;
                    while(cut.find(is_sep))
                    {
                        // index of current token
                        const size_t indx = cut.count();
                        if(!col) goto NEXT_LINE;

                        // advance colum until matching index
                        assert(col);
                        while(col->indx<indx)
                        {
                            col=col->next;
                            if(!col) goto NEXT_LINE;
                        }

                        // read all columns with same matching index
                        assert(col);
                        while(indx==col->indx)
                        {
                            col->push(cut.token(),cut.units(),NULL);
                            col=col->next;
                            if(!col) goto NEXT_LINE;
                        }
                    }
                    if(NULL!=col)
                    {
                        throw exception("missing data for column #%u after #line=%u", unsigned(col->indx), unsigned(iline));
                    }
                NEXT_LINE:;
                }

                return res;
            }

        }
    }
}
