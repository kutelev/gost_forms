#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <wchar.h>
#include <locale.h>
#include <math.h>

#include <kxsvglib.h>
#include <kxparsetablelib.h>

#include "kxcomapplib.h"

int parseArgs(int argc, char ** argv, char ** file_name, char ** out_dir)
{
    int c;

    *file_name = NULL;

    struct option long_options[] = {
           {"input",      required_argument, NULL, 0},
           {"dir",        required_argument, NULL, 0},
           {0,            0,                 0,    0},
    };

    while (1) {
        int option_index = 0;

        c = getopt_long(argc, argv, "i:d:", long_options, &option_index);
        if(c == -1)break;

        switch (c) {
            case 0:
                switch(option_index) {
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
                break;

            default:
                fprintf(stderr, "getopt_long returned unexpected code\n");
        }
    }

    if(*file_name == NULL)return -1;

    return 0;
}

char * basename(char * path)
{
    char * base = strrchr(path, '/');
    return base ? base + 1 : path;
}

void printUsage(char * app_name)
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

AbstractTable::AbstractTable(Page::PageSize page_size, bool landscape) : out_dir("."), file_name(NULL),
    columns(MAX_COLUMNS_NUM, MAX_LINE_LEN), page(page_size, landscape), font_size(::font_size)
{
}

int AbstractTable::buildTable()
{
    KXSvg::drawRect(out_file, leftMargin(), topMargin(), payload_width, header_height + payload_height);

    double y1 = topMargin() + header_height;

    for(int i = 0; i < lines_per_page; i++) {
        KXSvg::drawLine(out_file, leftMargin(), y1, leftMargin() + payload_width, y1);
        y1 += line_height;
    }

    int columns_num = columnsNum();

    double x = leftMargin();
    y1 = topMargin() + header_height;
    double y2 = topMargin() + header_height + payload_height;

    for(int i = 0; i < columns_num - 1; i++) {
        x += columnWidth(i);
        KXSvg::drawLine(out_file, x, y1, x, y2);
    }

    int rows_num = headerRowsNum();
    for(int i = 0; i < rows_num; i++)buildTable(i);

    return 0;
}

int AbstractTable::buildTable(int row)
{
    double x = leftMargin();

    double y = topMargin();

    for(int i = 0; i < row; i++)y += headerHeight(0, i);

    int columns_num = columnsNum();
    int rows_num = headerRowsNum();

    for(int i = 0; i < columns_num; x+=columnWidth(i),i++) {
        if(columnName(i, row) == NULL)continue;
        KXSvg::drawMultilineText(out_file, x + headerCellWidth(i, row) / 2.0,
            y + headerCellHeight(i, row) / 2.0, font_size, columnName(i, row),
            KXSvg::Center, columnAngle(i));

        if(i > 0)KXSvg::drawLine(out_file, x, y, x, y + headerCellHeight(i, row));

        if(row + headerVerticalSpan(i, row) - 1 < rows_num - 1)
            KXSvg::drawLine(out_file, x, y + headerCellHeight(i, row),
                x + headerCellWidth(i, row), y + headerCellHeight(i, row));
    }

    return 0;
}

int AbstractTable::fillTable(double y)
{
    if(strcmp(columns[0], "H") == 0 && columns[1] && strlen(columns[1])) {
        double len = _strlen(columns[1]); /* TODO */
        len = len * 0.75;

        KXSvg::drawText(out_file, header_x_skip, y, font_size, columns[1]);
        KXSvg::drawLine(out_file, header_x_skip - len, y + line_height / 4, header_x_skip + len, y + line_height / 4, false);
    } else if(strcmp(columns[0], "H2") == 0 && columns[1] && strlen(columns[1])) {
        KXSvg::drawText(out_file, header_x_skip, y, font_size, columns[1]);
    } else if(strcmp(columns[0], "L") == 0) {

        double x = leftMargin();

        int columns_num = columnsNum();

        for(int i = 0, j = 1; i < columns_num; x+=columnWidth(i),i++,j++) {
            if(columns[j] == NULL || columns[j][0] == '\0')continue;
            double skip = 0.0;
            if(columnAlignment(i) == KXSvg::Left)skip = 1.0;
            else if(columnAlignment(i) == KXSvg::Center)skip = columnWidth(i) / 2.0;
            KXSvg::drawText(out_file, x + skip, y, font_size, columns[j], columnAlignment(i));
        }
    } else return -1;

    return 0;
}

int AbstractTable::generate(int argc, char ** argv)
{
    setlocale(LC_ALL, "en_US.utf8");

    char* out_dir = (char *)".";
    char* file_name = NULL;

    if(parseArgs(argc, argv, &file_name, &out_dir)) {
        printUsage(argv[0]);
        return -1;
    }

    in_file.open(file_name, std::ios_base::in);
    if(!in_file.is_open()) {
        fprintf(stderr, "Could not open file: \"%s\"\n", file_name);
        return -1;
    }

    int page_num = 1;
    double y = 0;

    char out_file_name[256];

    int num;

    do {
        if(out_file == NULL) {
            sprintf(out_file_name, "%s/Sheet_%02u.svg", out_dir, page_num);
            out_file.open(out_file_name);

            if(out_file == NULL) {
                fprintf(stderr, "Could not open file for writing: \"%s\"\n",
                    out_file_name);
                return -1;
            }

            reconfigureDimensions(page_num);

            y = topMargin() + header_height + (line_height / 2.0L);

            page.begin(out_file);

            if(page_num <= emptyPagesNum()) {
                page.end(out_file);
                out_file.close();
                page_num++;
                continue;
            }

            buildTable();
        }

        in_file.getline(single_line, MAX_LINE_LEN - 1);

        if(strlen(single_line) == 0) {
            lines_per_page--;
            continue;
        }

        num = columns.parseLine(single_line, '\t');

        if(fillTable(y))continue;

        lines_per_page--;
        y += line_height;

        if( (lines_per_page == 0) || (in_file.eof()) ) {
            page_num++;
            page.end(out_file);
            out_file.close();
        }

    } while (!in_file.eof());

    if(!page.isEnded()) {
        page.end(out_file);
        out_file.close();
    }

    return 0;
}

void AbstractTable::configureDimensions(Page::PageSize page_size, bool landscape)
{
    Page dummy_page(page_size, landscape);
    available_height = dummy_page.getHeight() - topMargin() - bottomMargin();
    payload_width = dummy_page.getWidth() - leftMargin() - rightMargin();

    int num = headerColumnNum();
    header_x_skip = leftMargin();
    for(int i = 0; i < num; i++)header_x_skip += columnWidth(i);
    header_x_skip += columnWidth(num) / 2.0;
}

void AbstractTable::reconfigureDimensions(int page_num)
{
    int rows_num = headerRowsNum();
    header_height = 0.0;
    for(int i = 0; i < rows_num; i++)header_height += headerHeight(page_num, i);
    payload_height = available_height - header_height - footerHeight(page_num);
    lines_per_page = ceil(payload_height / min_line_height);
    line_height = payload_height / (double)lines_per_page;
}

double AbstractTable::headerCellWidth(int column, int row) const
{
    double width = 0.0;
    int span = headerHorizontalSpan(column, row);
    for(int i = 0; i < span; i++)width += columnWidth(column + i);
    return width;
}

double AbstractTable::headerCellHeight(int column, int row) const
{
    double height = 0.0;
    int span = headerVerticalSpan(column, row);
    for(int i = 0; i < span; i++)height += headerHeight(0, row + i);
    return height;
}

double AbstractTable::footerHeight(int page_num) const
{
    if(page_num == 1)return 40.0L;
    else return 15.0L;
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

int AbstractTable::columnAngle(int num, int row) const
{
    return 0;
}

int AbstractTable::headerRowsNum() const
{
    return 1;
}

int AbstractTable::headerHorizontalSpan(int column, int row) const
{
    return 1;
}

int AbstractTable::headerVerticalSpan(int column, int row) const
{
    return 1;
}

int AbstractTable::emptyPagesNum() const
{
    return 0;
}

size_t AbstractTable::_strlen(const char* string)
{
    mbstate_t mbs;
    memset(&mbs, 0, sizeof(mbs)); /* MUST BE!!! */

    size_t len = mbsrtowcs(NULL, &string, 0, &mbs);

    return len > 50 ? 50 : len;
}
