//! \file

#ifndef YACK_CSV_INCLUDED
#define YACK_CSV_INCLUDED

#include "yack/jive/parser.hpp"
#include "yack/jive/syntax/translator.hpp"

namespace yack
{

    namespace CSV
    {

        class Cell : public string
        {
        public:
            enum Type
            {
                isRaw,
                isStr
            };

            //! empty raw cell
            explicit Cell() : string(), type(isRaw), next(0), prev(0) {}

            template <typename DATA> inline
            explicit Cell(const DATA &s, const Type t) : string(s), type(t), next(0), prev(0) {}

            virtual ~Cell() noexcept {}
            
            Cell(const Cell &other) : string(other), type(other.type), next(0), prev(0) {}

            Type  type;
            Cell *next;
            Cell *prev;

        private:
            YACK_DISABLE_ASSIGN(Cell);

        };

        typedef cxx_list_of<Cell> Cells;

        class Line : public Cells
        {
        public:
            explicit Line() noexcept : Cells(), next(0), prev(0) {}
            virtual ~Line() noexcept {}
            Line(const Line &other) : Cells(other), next(0), prev(0) {}

            Line *next;
            Line *prev;

        private:
            YACK_DISABLE_ASSIGN(Line);
        };

        typedef cxx_list_of<Line> Lines;

        class Document : public Lines
        {
        public:
            explicit Document() noexcept : Lines() {}
            virtual ~Document() noexcept {}

            friend std::ostream & operator<<(std::ostream &os, const Document &doc)
            {
                for(const Line *line=doc.head;line;line=line->next)
                {
                    os << *line << std::endl;
                }
                return os;
            }

        private:
            YACK_DISABLE_ASSIGN(Document);
        };

        class Parser : public jive::parser
        {
        public:
            explicit Parser();
            virtual ~Parser() noexcept;

            Document * operator()(jive::module *m);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Parser);
            class Translator;
            Translator *tr;
        };
    }

}

#endif


