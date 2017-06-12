#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <kxsvglib.h>
#include <kxparsetablelib.h>
#include <kxcomapplib.h>
#include <math.h>

class Table : public AbstractTable {
    public:
        Table();

    protected:
        virtual double headerHeight(int page_num, int row = 0) const;

        virtual int columnsNum() const;
        virtual double columnWidth(int num) const;
        virtual KXSvg::Alignment columnAlignment(int num) const;
        virtual int columnAngle(int num, int row = 0) const;
        virtual const char * columnName(int column, int row = 0) const;
        virtual int headerColumnNum() const;
};

Table::Table() : AbstractTable(Page::A4, false)
{
    configureDimensions(Page::A4, false);
    min_line_height = 8.0L;
}

double Table::headerHeight(int page_num, int row) const
{
    return 15.0L;
}

int Table::columnsNum() const
{
    return 7;
}

double Table::columnWidth(int num) const
{
    static const double widths[] = {6.0, 6.0, 8.0, 70.0, 63.0, 10.0, 22.0};
    return widths[num];
}

KXSvg::Alignment Table::columnAlignment(int num) const
{
    static const KXSvg::Alignment alignments[] = {KXSvg::Center, KXSvg::Center, KXSvg::Center, KXSvg::Left, KXSvg::Left, KXSvg::Center, KXSvg::Center};
    return alignments[num];
}

int Table::columnAngle(int num, int row) const
{
    static const int angles[] = {-90, -90, -90, 0, 0, -90, 0};
    return angles[num];
}

const char * Table::columnName(int column, int row) const
{
    static const char * names[] = {"Формат", "Зона", "Поз.", "Обозначение", "Наименование", "Кол.", "Примечание"};
    return names[column];
}

int Table::headerColumnNum() const
{
    return 4;
}

int main(int argc, char** argv)
{
    Table form;
    return form.generate(argc, argv);
}
