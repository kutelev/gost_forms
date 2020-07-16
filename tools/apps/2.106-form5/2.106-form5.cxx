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
        virtual int headerRowsNum() const;
        virtual int headerHorizontalSpan(int column, int row) const;
        virtual int headerVerticalSpan(int column, int row) const;
};

Table::Table() : AbstractTable(Page::A3, true)
{
    configureDimensions(Page::A3, true);
    min_line_height = 8.0L;
}

double Table::headerHeight(int page_num, int row) const
{
    static const double heights[] = {9.0, 18.0};
    return heights[row];
}

int Table::columnsNum() const
{
    return 11;
}

double Table::columnWidth(int num) const
{
    static const double widths[] = {7.0, 60.0, 45.0, 70.0, 55.0, 70.0, 16.0, 16.0, 16.0, 16.0, 24.0};
    return widths[num];
}

KXSvg::Alignment Table::columnAlignment(int num) const
{
    static const KXSvg::Alignment alignments[] = {KXSvg::Center, KXSvg::Left, KXSvg::Left, KXSvg::Left, KXSvg::Left, KXSvg::Left, KXSvg::Center, KXSvg::Center, KXSvg::Center, KXSvg::Center, KXSvg::Center};
    return alignments[num];
}

int Table::columnAngle(int num, int row) const
{
    if(num == 0 && row == 0)return -90;
    else return 0;
}

const char * Table::columnName(int column, int row) const
{
    static const char * names_row0[] = {"№ строки", "Наименование", "Код\nпродукции", "Обозначение\nдокумента на\nпоставки", "Поставщик", "Куда входит\n(обозначение)", "Количество", NULL, NULL, NULL, "Примечание"};
    static const char * names_row1[] = {NULL, NULL, NULL, NULL, NULL, NULL, "На из-\nделие", "В комп-\nлекты", "На\nрегулир.", "Всего", NULL};
    if(row == 0)return names_row0[column];
    if(row == 1)return names_row1[column];
    return NULL;
}

int Table::headerColumnNum() const
{
    return 1;
}

int Table::headerRowsNum() const
{
    return 2;
}

int Table::headerHorizontalSpan(int column, int row) const
{
    static int span_row0[] = {1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 1};
    static int span_row1[] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0};
    if(row == 0)return span_row0[column];
    if(row == 1)return span_row1[column];
    return 0;
}

int Table::headerVerticalSpan(int column, int row) const
{
    static int span_row0[] = {2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 2};
    static int span_row1[] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0};
    if(row == 0)return span_row0[column];
    if(row == 1)return span_row1[column];
    return 0;
}

int main(int argc, char** argv)
{
    Table form;
    return form.generate(argc, argv);
}
