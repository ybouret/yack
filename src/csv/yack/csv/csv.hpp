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

            explicit Cell() : string(), type(isRaw), next(0), prev(0) {}
            virtual ~Cell() noexcept;
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

        private:
            YACK_DISABLE_ASSIGN(Document);
        };

        class Parser : public jive::parser
        {
        public:
            explicit Parser();
            virtual ~Parser() noexcept;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Parser);
            auto_ptr<jive::syntax::translator> tr;
        };
    }

}

#endif


