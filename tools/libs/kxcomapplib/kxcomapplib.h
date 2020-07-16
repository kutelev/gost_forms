#ifndef __KXCOMAPPLIB_H__
#define __KXCOMAPPLIB_H__

#include <fstream>

int parseArgs(int argc, char ** argv, char ** file_name, char ** out_dir);
void printUsage(char * app_name);

class AbstractTable {
    public:
        AbstractTable(Page::PageSize page_size, bool landscape);

        int generate(int argc, char ** argv);

    protected:
        virtual int buildTable();
        virtual int buildTable(int row);
        virtual int fillTable(double y);

        void configureDimensions(Page::PageSize page_size, bool landscape);
        void reconfigureDimensions(int page_num);

        virtual double headerHeight(int page_num, int row = 0) const = 0;
        virtual double headerCellWidth(int column, int row) const;
        virtual double headerCellHeight(int column, int row) const;
        virtual double footerHeight(int page_num) const;

        virtual double leftMargin() const;
        virtual double rightMargin() const;
        virtual double topMargin() const;
        virtual double bottomMargin() const;

        virtual int columnsNum() const = 0;
        virtual double columnWidth(int num) const = 0;
        virtual KXSvg::Alignment columnAlignment(int num) const = 0;
        virtual int columnAngle(int num, int row = 0) const;
        virtual const char * columnName(int column, int row = 0) const = 0;
        virtual int headerColumnNum() const = 0;
        virtual int headerRowsNum() const;
        virtual int headerHorizontalSpan(int column, int row) const;
        virtual int headerVerticalSpan(int column, int row) const;

        virtual int emptyPagesNum() const;

        size_t _strlen(const char* string);

        const char * out_dir;
        char * file_name;

        std::ifstream in_file;
        OutFile out_file;
        char single_line[MAX_LINE_LEN];
        TextLine columns;
        Page page;

        double available_height;
        double header_height;
        double payload_height;
        double payload_width;
        double min_line_height;
        double line_height;
        int lines_per_page;

        const double font_size;
        double header_x_skip;
};

#endif