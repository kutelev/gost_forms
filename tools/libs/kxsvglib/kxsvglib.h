#ifndef __KXSVGLIB_H__
#define __KXSVGLIB_H__

#include <cstdio>

namespace KXSvg {

enum Alignment { Left, Center, Right };

int drawRect(FILE* file, double x, double y, double width, double height, bool bold = true);

int drawLine(FILE* file, double x1, double y1, double x2, double y2, bool bold = true);

int drawText(FILE* file, double x, double y, double font_size, const char* text, Alignment alignment = Center, int angle = 0);

int drawMultilineText(FILE* file, double x, double y, double font_size, const char* multiline_text, int angle = 0);

}

class Page final {
public:
    enum PageSize { A0, A1, A2, A3, A4 };

    Page() = delete;
    Page(PageSize page_size, bool landscape);

    int setSize(PageSize page_size, bool landscape);

    double getWidth() const { return m_width; }
    double getHeight() const { return m_height; }

    const char* formatName() const;

    int begin(FILE* file) const;
    int end(FILE* file) const;

    bool isEnded() const { return m_is_ended; }

protected:
    Page::PageSize m_page_size{ PageSize::A4 };

    double m_width{ 210.0 };
    double m_height{ 297.0 };

    mutable bool m_is_ended{ false };
};

class Element {
public:
    virtual ~Element() = default;

    virtual int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) = 0;

protected:
    void setOffset(double x_offs, double y_offs);
    double absX(double x) const { return x + m_x_offs; }
    double absY(double y) const { return y + m_y_offs; }

    double m_x_offs{};
    double m_y_offs{};
};

class OutFile final {
public:
    ~OutFile() { close(); }

    int open(const char* file_name);
    void close();
    FILE* file() { return m_file; }
    operator FILE*() { return file(); }

protected:
    FILE* m_file{};
};

#endif
