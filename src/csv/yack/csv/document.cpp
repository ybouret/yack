
#include "yack/csv/document.hpp"


namespace yack
{

    namespace CSV
    {

        Document::  Document() noexcept : Lines() {}
        Document:: ~Document() noexcept {}


        std::ostream & operator<<(std::ostream &os, const Document &doc)
        {
            for(const Line *line=doc.head;line;line=line->next)
            {
                os << *line << std::endl;
            }
            return os;
        }

    }

}

