#include <kxcomapplib.h>

class Table : public AbstractTable {
public:
    Table()
        : AbstractTable(Page::A4, false)
    {
        configureDimensions(Page::A4, false);
        m_min_line_height = 8.0;
    }

protected:
    double headerHeight(unsigned int page_num, unsigned int row) const override { return 15.0; }
    unsigned int columnCount() const override { return 7U; }
    double columnWidth(unsigned int num) const override
    {
        static const double widths[] = { 6.0, 6.0, 8.0, 70.0, 63.0, 10.0, 22.0 };
        return widths[num];
    }
    KXSvg::Alignment columnAlignment(unsigned int num) const override
    {
        static const KXSvg::Alignment alignments[] = { KXSvg::Center, KXSvg::Center, KXSvg::Center, KXSvg::Left, KXSvg::Left, KXSvg::Center, KXSvg::Center };
        return alignments[num];
    }
    int columnAngle(unsigned int num, unsigned int row) const override
    {
        static const int angles[] = { -90, -90, -90, 0, 0, -90, 0 };
        return angles[num];
    }
    const char* columnName(unsigned int column, unsigned int row) const override
    {
        static const char* names[] = { "Формат", "Зона", "Поз.", "Обозначение", "Наименование", "Кол.", "Примечание" };
        return names[column];
    }
    unsigned int headerColumnCount() const override { return 4U; }
};

int main(int argc, char** argv)
{
    return Table().generate(argc, argv);
}
