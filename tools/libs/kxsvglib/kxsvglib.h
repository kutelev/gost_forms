#ifndef __KXSVGLIB_H__
#define __KXSVGLIB_H__

#include <stdio.h>

namespace KXSvg {

enum Alignment { Left, Center, Right };

int drawRect(FILE* file, const double x, const double y, const double width, const double height, const bool bold = true);

int drawLine(FILE* file, const double x1, const double y1, const double x2, const double y2, const bool bold = true);

int drawText(FILE* file,
             const double x,
             const double y,
             const double font_size,
             const char* const text,
             const Alignment alignment = Center,
             const int angle = 0);

int drawMultilineText(FILE* file, double x, double y, double font_size, const char* multiline_text, Alignment alignment = Center, int angle = 0);

}

class Page {
public:
    enum PageSize { A0, A1, A2, A3, A4 };

    Page(const Page::PageSize page_size, const bool landscape);

    int setSize(const Page::PageSize page_size, const bool landscape);

    double getWidth() const { return width; }
    double getHeight() const { return height; }

    const char* const formatName() const;

    int begin(FILE* file) const;
    int end(FILE* file) const;

    bool isEnded() const { return is_ended; }

protected:
    Page::PageSize page_size;

    double width;
    double height;

    mutable bool is_ended;

private:
    Page();
};

class Element {
public:
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags) = 0;

protected:
    int setOffset(const double x_offs, const double y_offs);
    double absx(double x) const { return x + x_offs; }
    double absy(double y) const { return y + y_offs; }

    double x_offs, y_offs;
};

class OutFile {
public:
    OutFile()
        : file_(NULL)
    {
    }
    ~OutFile() { close(); }

    int open(const char* file_name);
    void close();
    FILE* file() { return file_; }
    operator FILE*() { return file(); }

protected:
    FILE* file_;
};

#endif