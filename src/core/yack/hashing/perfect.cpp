#include "yack/hashing/perfect.hpp"

namespace yack
{

    namespace hashing
    {

        perfect::node_type:: node_type(node_type    *parent,
                                       const uint8_t encode) throw() :
        next(0),
        prev(0),
        from(parent),
        freq(0),
        chld(),
        hash(-1),
        code(encode)
        {
        }

        perfect::node_type:: ~node_type() throw()
        {
            while(chld.size) delete chld.pop_back();
        }

    }
}

#include "yack/exception.hpp"
#include <cstring>

namespace yack
{

    namespace hashing
    {


        perfect:: ~perfect() throw()
        {
            assert(root);
            delete root;
            root = NULL;
        }

        perfect:: perfect() :
        root( new node_type(0,0) ),
        hash(0)
        {
        }

        perfect:: perfect(const char * keys[], const size_t size) :
        root( new node_type(0,0) ),
        hash(0)
        {
            try
            {
                assert(keys);
                for(size_t i=0;i<size;++i)
                {
                    insert(keys[i]);
                }
            }
            catch(...)
            {
                delete root;
                root = NULL;
                throw;
            }
        }



        void perfect:: insert(const void   *addr,
                              size_t        size)
        {
            assert(yack_good(addr,size));
            const uint8_t *data = static_cast<const uint8_t *>(addr);
            node_type     *node = root;
            while(size>0)
            {
                const uint8_t  code = *(data++);
                for(node_type *scan = node->chld.head;scan;scan=scan->next)
                {
                    assert(scan->from==node);
                    if(code==scan->code)
                    {
                        // found
                        node=scan;
                        goto DONE;
                    }
                }

                //--------------------------------------------------------------
                // not found
                //--------------------------------------------------------------
                node = node->chld.push_back( new node_type(node,code) );
            DONE:
                --size;
            }
            if(node->hash>=0)
            {
                throw exception("hashing::perfect(multiple keys)");
            }

            //--------------------------------------------------------------
            // update
            //--------------------------------------------------------------
            node->hash = hash++;

            while(true)
            {
                node->freq++;
                node_type *from = node->from;
                if(!from) break;
                list_type  &L = from->chld; assert(L.owns(node));

                while(node->prev && node->prev->freq<node->freq)
                    L.towards_front(node);
                
                node=from;
            }

        }

        void perfect:: insert(const char *text)
        {
            insert( text, text ? strlen(text) : 0 );
        }

        void perfect:: insert(const memory::ro_buffer &buff)
        {
            insert(buff.ro_addr(),buff.measure());
        }


        int  perfect:: operator()(const void *addr, size_t size) const throw()
        {
            assert(yack_good(addr,size));
            const uint8_t   *data = static_cast<const uint8_t *>(addr);
            const node_type *node = root;

            while(size>0)
            {
                const uint8_t code = *(data++);
                for(const node_type *scan=node->chld.head;scan;scan=scan->next)
                {
                    assert(scan->from==node);
                    if(code==scan->code)
                    {
                        node=scan;
                        goto NEXT;
                    }
                }
                return -1;

            NEXT:
                --size;
            }

            return node->hash;
        }

        int  perfect:: operator()(const char *text) const throw()
        {
            return (*this)( text, text ? strlen(text) : 0 );
        }


        int  perfect:: operator()(const memory::ro_buffer &buff) const throw()
        {
            return (*this)(buff.ro_addr(),buff.measure());
        }

    }
}


#include "yack/ios/gv/vizible.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/ios/ascii/encoded.hpp"
#include "yack/string.hpp"


namespace yack
{

    namespace hashing
    {

        void perfect::node_type:: gv(ios::ostream &fp, const bool isRoot) const
        {
            const node_type *self = this;
            ios::vizible::uuid(fp,self);

            fp << '[';
            fp << "label=\"";
            if(isRoot) fp << "root";  else fp <<ios::ascii::encoded[code];
            fp("#%lu",(unsigned long)freq);
            if(hash>=0)
            {
                fp("@%d",hash);
            }
            fp << "\"";
            if(hash>=0)
            {
                fp << ",shape=box";
            }
            else
            {
                fp << ",shape=oval";
            }

            if(isRoot)
            {
                fp << ",style=bold";
            }

            fp << ']' << ';' << '\n';

            unsigned   indx = 1;
            const bool show = chld.size>1;
            for(const node_type *node=chld.head;node;node=node->next,++indx)
            {
                node->gv(fp);
                ios::vizible::arrow(fp,self,node);
                if(show) fp("[label=\"%u\"]",indx);
                fp << ";\n";
            }
        }

        void perfect:: gv(const string &filename) const
        {
            {
                ios::ocstream fp(filename);
                ios::vizible::digraph_init(fp,"G");
                root->gv(fp,true);
                ios::vizible::digraph_quit(fp);
            }
            ios::vizible::render(filename);
        }

        void perfect:: gv(const char *filename) const
        {
            const string _(filename);
            gv(_);
        }
    }

}

