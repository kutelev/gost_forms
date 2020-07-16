#include <clocale>
#include <cmath>
#include <cstring>
#include <getopt.h>
#include <iostream>

#include <kxparsetablelib.h>
#include <kxsvglib.h>

#include "kxcomapplib.h"

int parseArgs(int argc, char** argv, char** file_name, char** out_dir)
{
    int c;

    *file_name = nullptr;

    struct option long_options[] = {
        { "input", required_argument, nullptr, 0 },
        { "dir", required_argument, nullptr, 0 },
        { nullptr, 0, nullptr, 0 },
    };

    while (true) {
        int option_index = 0;

        c = getopt_long(argc, argv, "i:d:", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        *file_name = optarg;
                        break;
                    case 1:
                        *out_dir = optarg;
                        break;
                    default:
                        fprintf(stderr, "Invalid long argument\n");
                        return -1;
                }

                break;
            case 'i':
                *file_name = optarg;
                break;
            case 'd':
                *out_dir = optarg;
                break;
            case '?':
                fprintf(stderr, "Invalid short argument\n");
                return -1;
            default:
                fprintf(stderr, "getopt_long returned unexpected code\n");
                return -1;
        }
    }

    if (*file_name == nullptr)
        return -1;

    return 0;
}

void printUsage(char* app_name)
{
    fprintf(stderr, "===============================\n");
    fprintf(stderr, "Usage: %s [options]\n", basename(app_name));
    fprintf(stderr, "\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "-i | --input    Input file name\n");
    fprintf(stderr, "-d | --dir      Output directory [.]\n");
    fprintf(stderr, "===============================\n");
}

static const double font_size = 13.3231441;

AbstractTable::AbstractTable(Page::PageSize page_size, bool landscape)
    : m_out_dir(".")
    , m_file_name(nullptr)
    , m_columns(MAX_COLUMNS_NUM, MAX_LINE_LEN)
    , m_page(page_size, landscape)
    , m_font_size(::font_size)
{
}

int AbstractTable::buildTable()
{
    KXSvg::drawRect(m_out_file, leftMargin(), topMargin(), m_payload_width, m_header_height + m_payload_height);

    double y1 = topMargin() + m_header_height;

    for (unsigned int i = 0U; i < m_lines_per_page; i++) {
        KXSvg::drawLine(m_out_file, leftMargin(), y1, leftMargin() + m_payload_width, y1);
        y1 += m_line_height;
    }

    const unsigned int column_count = columnCount();

    double x = leftMargin();
    y1 = topMargin() + m_header_height;
    double y2 = topMargin() + m_header_height + m_payload_height;

    for (unsigned int column = 0U; column < column_count - 1; ++column) {
        x += columnWidth(column);
        KXSvg::drawLine(m_out_file, x, y1, x, y2);
    }

    const unsigned int row_count = headerRowCount();
    for (unsigned int row = 0U; row < row_count; ++row)
        buildTable(row);

    return 0;
}

int AbstractTable::buildTable(unsigned int row)
{
    double x = leftMargin();

    double y = topMargin();

    for (unsigned int i = 0U; i < row; ++i)
        y += headerHeight(0U, i);

    const unsigned int column_count = columnCount();
    const unsigned int row_count = headerRowCount();

    for (unsigned int column = 0U; column < column_count; x += columnWidth(column), ++column) {
        if (columnName(column, row) == nullptr)
            continue;
        KXSvg::drawMultilineText(m_out_file,
                                 x + headerCellWidth(column, row) / 2.0,
                                 y + headerCellHeight(column, row) / 2.0,
                                 m_font_size,
                                 columnName(column, row),
                                 columnAngle(column, 0));

        if (column > 0U)
            KXSvg::drawLine(m_out_file, x, y, x, y + headerCellHeight(column, row));

        if (row + headerVerticalSpan(column, row) - 1U < row_count - 1U)
            KXSvg::drawLine(m_out_file, x, y + headerCellHeight(column, row), x + headerCellWidth(column, row), y + headerCellHeight(column, row));
    }

    return 0;
}

int AbstractTable::fillTable(double y)
{
    if (strcmp(m_columns[0], "H") == 0 && m_columns[1] && strlen(m_columns[1])) {
        const double len = 0.75 * AbstractTable::strlen(m_columns[1]); /* TODO */

        KXSvg::drawText(m_out_file, m_header_x_skip, y, m_font_size, m_columns[1]);
        KXSvg::drawLine(m_out_file, m_header_x_skip - len, y + m_line_height / 4, m_header_x_skip + len, y + m_line_height / 4, false);
    }
    else if (strcmp(m_columns[0], "H2") == 0 && m_columns[1] && strlen(m_columns[1])) {
        KXSvg::drawText(m_out_file, m_header_x_skip, y, m_font_size, m_columns[1]);
    }
    else if (strcmp(m_columns[0], "L") == 0) {
        double x = leftMargin();

        const unsigned int column_count = columnCount();

        for (unsigned int i = 0; i < column_count; x += columnWidth(i), ++i) {
            if (m_columns[i + 1] == nullptr || m_columns[i + 1][0] == '\0')
                continue;
            double skip = 0.0;
            if (columnAlignment(i) == KXSvg::Left)
                skip = 1.0;
            else if (columnAlignment(i) == KXSvg::Center)
                skip = columnWidth(i) / 2.0;
            KXSvg::drawText(m_out_file, x + skip, y, m_font_size, m_columns[i + 1], columnAlignment(i));
        }
    }
    else
        return -1;

    return 0;
}

int AbstractTable::generate(int argc, char** argv)
{
    setlocale(LC_ALL, "en_US.utf8");

    char* out_dir = (char*)".";
    char* file_name = nullptr;

    if (parseArgs(argc, argv, &file_name, &out_dir)) {
        printUsage(argv[0]);
        return -1;
    }

    m_in_file.open(file_name, std::ios_base::in);
    if (!m_in_file.is_open()) {
        fprintf(stderr, "Could not open file: \"%s\"\n", file_name);
        return -1;
    }

    unsigned int page_num = 1;
    double y = 0;

    char out_file_name[256];

    do {
        if (m_out_file == nullptr) {
            sprintf(out_file_name, "%s/Sheet_%02u.svg", out_dir, page_num);
            m_out_file.open(out_file_name);

            if (m_out_file == nullptr) {
                fprintf(stderr, "Could not open file for writing: \"%s\"\n", out_file_name);
                return -1;
            }

            reconfigureDimensions(page_num);

            y = topMargin() + m_header_height + (m_line_height / 2.0);

            m_page.begin(m_out_file);

            if (page_num <= emptyPageCount()) {
                m_page.end(m_out_file);
                m_out_file.close();
                page_num++;
                continue;
            }

            buildTable();
        }

        m_in_file.getline(m_single_line, MAX_LINE_LEN - 1);

        if (strlen(m_single_line) == 0) {
            m_lines_per_page--;
            continue;
        }

        m_columns.parseLine(m_single_line, '\t');

        if (fillTable(y))
            continue;

        m_lines_per_page--;
        y += m_line_height;

        if ((m_lines_per_page == 0U) || (m_in_file.eof())) {
            page_num++;
            m_page.end(m_out_file);
            m_out_file.close();
        }

    } while (!m_in_file.eof());

    if (!m_page.isEnded()) {
        m_page.end(m_out_file);
        m_out_file.close();
    }

    return 0;
}

void AbstractTable::configureDimensions(Page::PageSize page_size, bool landscape)
{
    Page dummy_page(page_size, landscape);
    m_available_height = dummy_page.getHeight() - topMargin() - bottomMargin();
    m_payload_width = dummy_page.getWidth() - leftMargin() - rightMargin();

    const unsigned int column_count = headerColumnCount();
    m_header_x_skip = leftMargin();
    for (unsigned int i = 0U; i < column_count; ++i)
        m_header_x_skip += columnWidth(i);
    m_header_x_skip += columnWidth(column_count) / 2.0;
}

void AbstractTable::reconfigureDimensions(unsigned int page_num)
{
    const unsigned int row_count = headerRowCount();
    m_header_height = 0.0;
    for (unsigned int i = 0; i < row_count; i++)
        m_header_height += headerHeight(page_num, i);
    m_payload_height = m_available_height - m_header_height - footerHeight(page_num);
    m_lines_per_page = static_cast<unsigned int>(ceil(m_payload_height / m_min_line_height));
    m_line_height = m_payload_height / m_lines_per_page;
}

double AbstractTable::headerCellWidth(unsigned int column, unsigned int row) const
{
    double width = 0.0;
    const unsigned int span = headerHorizontalSpan(column, row);
    for (unsigned int i = 0U; i < span; ++i)
        width += columnWidth(column + i);
    return width;
}

double AbstractTable::headerCellHeight(unsigned int column, unsigned int row) const
{
    double height = 0.0;
    const unsigned int span = headerVerticalSpan(column, row);
    for (unsigned int i = 0U; i < span; ++i)
        height += headerHeight(0U, row + i);
    return height;
}

double AbstractTable::footerHeight(unsigned int page_num) const
{
    return page_num == 1U ? 40.0: 15.0;
}

double AbstractTable::leftMargin() const
{
    return 20.0;
}

double AbstractTable::rightMargin() const
{
    return 5.0;
}

double AbstractTable::topMargin() const
{
    return 5.0;
}

double AbstractTable::bottomMargin() const
{
    return 5.0;
}

int AbstractTable::columnAngle(unsigned int num, unsigned int row) const
{
    return 0;
}

unsigned int AbstractTable::headerRowCount() const
{
    return 1U;
}

unsigned int AbstractTable::headerHorizontalSpan(unsigned int column, unsigned int row) const
{
    return 1U;
}

unsigned int AbstractTable::headerVerticalSpan(unsigned int column, unsigned int row) const
{
    return 1U;
}

unsigned int AbstractTable::emptyPageCount() const
{
    return 0U;
}

size_t AbstractTable::strlen(const char* string)
{
    mbstate_t mbs{};
    size_t len = mbsrtowcs(nullptr, &string, 0, &mbs);
    return len > 50 ? 50 : len;
}
