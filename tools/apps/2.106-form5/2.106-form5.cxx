#include <kxcomapplib.h>

class Table : public AbstractTable {
public:
    Table()
        : AbstractTable(Page::A3, true)
    {
        configureDimensions(Page::A3, true);
        m_min_line_height = 8.0;
    }

protected:
    double headerHeight(unsigned int page_num, unsigned int row) const override
    {
        static const double heights[] = { 9.0, 18.0 };
        return heights[row];
    }
    unsigned int columnCount() const override { return 11U; }
    double columnWidth(unsigned int num) const override
    {
        static const double widths[] = { 7.0, 60.0, 45.0, 70.0, 55.0, 70.0, 16.0, 16.0, 16.0, 16.0, 24.0 };
        return widths[num];
    }
    KXSvg::Alignment columnAlignment(unsigned int num) const override
    {
        static const KXSvg::Alignment alignments[] = { KXSvg::Center, KXSvg::Left,   KXSvg::Left,   KXSvg::Left,   KXSvg::Left,  KXSvg::Left,
                                                       KXSvg::Center, KXSvg::Center, KXSvg::Center, KXSvg::Center, KXSvg::Center };
        return alignments[num];
    }
    int columnAngle(unsigned int num, unsigned int row) const override
    {
        if (num == 0 && row == 0)
            return -90;
        else
            return 0;
    }
    const char* columnName(unsigned int column, unsigned int row) const override
    {
        static const char* names_row0[] = { "№ строки",       "Наименование",
                                            "Код\nпродукции", "Обозначение\nдокумента на\nпоставки",
                                            "Поставщик",      "Куда входит\n(обозначение)",
                                            "Количество",     nullptr,
                                            nullptr,          nullptr,
                                            "Примечание" };
        static const char* names_row1[] = { nullptr,         nullptr,          nullptr,        nullptr, nullptr, nullptr,
                                            "На из-\nделие", "В комп-\nлекты", "На\nрегулир.", "Всего", nullptr };
        if (row == 0U)
            return names_row0[column];
        if (row == 1U)
            return names_row1[column];
        return nullptr;
    }
    unsigned int headerColumnCount() const override { return 1U; }
    unsigned int headerRowCount() const override { return 2U; }
    unsigned int headerHorizontalSpan(unsigned int column, unsigned int row) const override
    {
        static unsigned int span_row0[] = { 1U, 1U, 1U, 1U, 1U, 1U, 4U, 0U, 0U, 0U, 1U };
        static unsigned int span_row1[] = { 0U, 0U, 0U, 0U, 0U, 0U, 1U, 1U, 1U, 1U, 0U };
        if (row == 0U)
            return span_row0[column];
        if (row == 1U)
            return span_row1[column];
        return 0U;
    }
    unsigned int headerVerticalSpan(unsigned int column, unsigned int row) const override
    {
        static unsigned int span_row0[] = { 2U, 2U, 2U, 2U, 2U, 2U, 1U, 0U, 0U, 0U, 2U };
        static unsigned int span_row1[] = { 0U, 0U, 0U, 0U, 0U, 0U, 1U, 1U, 1U, 1U, 0U };
        if (row == 0U)
            return span_row0[column];
        if (row == 1U)
            return span_row1[column];
        return 0U;
    }
};

int main(int argc, char** argv)
{
    return Table().generate(argc, argv);
}
