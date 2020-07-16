#ifndef __KXCOMAPPLIB_H__
#define __KXCOMAPPLIB_H__

#include <fstream>
#include <cstddef>

#include <kxparsetablelib.h>
#include <kxsvglib.h>

int parseArgs(int argc, char** argv, char** file_name, char** out_dir);
void printUsage(char* app_name);

class AbstractTable {
public:
    AbstractTable(Page::PageSize page_size, bool landscape);

    static size_t strlen(const char* string);

    int generate(int argc, char** argv);

protected:
    virtual int buildTable();
    virtual int buildTable(unsigned int row);
    virtual int fillTable(double y);

    void configureDimensions(Page::PageSize page_size, bool landscape);
    void reconfigureDimensions(unsigned int page_num);

    virtual double headerHeight(unsigned int page_num, unsigned int row) const = 0;
    virtual double headerCellWidth(unsigned int column, unsigned int row) const;
    virtual double headerCellHeight(unsigned int column, unsigned int row) const;
    virtual double footerHeight(unsigned int page_num) const;

    virtual double leftMargin() const;
    virtual double rightMargin() const;
    virtual double topMargin() const;
    virtual double bottomMargin() const;

    virtual unsigned int columnCount() const = 0;
    virtual double columnWidth(unsigned int num) const = 0;
    virtual KXSvg::Alignment columnAlignment(unsigned int num) const = 0;
    virtual int columnAngle(unsigned int num, unsigned int row) const;
    virtual const char* columnName(unsigned int column, unsigned int row) const = 0;
    virtual unsigned int headerColumnCount() const = 0;
    virtual unsigned int headerRowCount() const;
    virtual unsigned int headerHorizontalSpan(unsigned int column, unsigned int row) const;
    virtual unsigned int headerVerticalSpan(unsigned int column, unsigned int row) const;

    virtual unsigned int emptyPageCount() const;

    const char* m_out_dir;
    char* m_file_name;

    std::ifstream m_in_file;
    OutFile m_out_file;
    char m_single_line[MAX_LINE_LEN];
    TextLine m_columns;
    Page m_page;

    double m_available_height;
    double m_header_height;
    double m_payload_height;
    double m_payload_width;
    double m_min_line_height;
    double m_line_height;
    unsigned int m_lines_per_page;

    const double m_font_size;
    double m_header_x_skip;
};

#endif