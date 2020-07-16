#include <cstdio>
#include <string>

#include <kxparsetablelib.h>

#include "kxsvglib.h"

static const char* textStyle(const KXSvg::Alignment alignmet)
{
    switch (alignmet) {
        case KXSvg::Left:
            return "text-align:start;text-anchor:start";
        case KXSvg::Center:
            return "text-align:center;text-anchor:middle";
        case KXSvg::Right:
            return "text-align:end;text-anchor:end";
        default:
            fprintf(stderr, "Invalid alignmet type!\n");
            return "text-align:center;text-anchor:middle";
    }
}

namespace KXSvg {

int drawRect(FILE* file, double x, double y, double width, double height, bool bold)
{
    fprintf(file,
            "        <rect x=\"%.6lf\" y=\"%.6lf\" width=\"%.6lf\" "
            "height=\"%.6lf\" fill=\"none\" stroke=\"black\" "
            "stroke-width=\"%s\" />\n",
            x,
            y,
            width,
            height,
            bold ? "0.8" : "0.3");

    return 0;
}

int drawLine(FILE* file, double x1, double y1, double x2, double y2, bool bold)
{
    fprintf(file,
            "        <line x1=\"%.6lf\" y1=\"%.6lf\" x2=\"%.6lf\" "
            "y2=\"%.6lf\" stroke=\"black\" stroke-width=\"%s\" />\n",
            x1,
            y1,
            x2,
            y2,
            bold ? "0.8" : "0.3");

    return 0;
}

int drawText(FILE* file, double x, double y, double font_size, const char* text, Alignment alignment, int angle)
{
    constexpr double k1 = 3.54330700988902143941095308362109464599991908268;
    constexpr double k2 = 4.65675812022197213706391400520004656758120221972;

    switch (angle) {
        case 0:
            fprintf(file,
                    "        <text x=\"%.6lf\" y=\"%.6lf\" "
                    "style=\"%s\" "
                    "font-family=\"GOST type A\" font-size=\"%.06f\" "
                    "font-style=\"italic\" fill=\"black\">%s</text>\n",
                    x,
                    y + (font_size / k2) / 2.0,
                    textStyle(alignment),
                    font_size / k1,
                    text);
            break;
        case -90:
            fprintf(file,
                    "        <text x=\"%.6lf\" y=\"%.6lf\" "
                    "style=\"%s\" "
                    "font-family=\"GOST type A\" font-size=\"%.06f\" "
                    "font-style=\"italic\" fill=\"black\" "
                    "transform=\"rotate(-90,%.06f,%.06f)\">%s</text>\n",
                    -y,
                    x + (font_size / k2) / 2.0,
                    textStyle(alignment),
                    font_size / k1,
                    0.0,
                    0.0,
                    text);
            break;
        case -180:
        case 180:
            fprintf(file,
                    "        <text x=\"%.6lf\" y=\"%.6lf\" "
                    "style=\"%s\" "
                    "font-family=\"GOST type A\" font-size=\"%.06f\" "
                    "font-style=\"italic\" fill=\"black\" "
                    "transform=\"rotate(180,%.06f,%.06f)\">%s</text>\n",
                    -x,
                    -(y - (font_size / k2) / 2.0),
                    textStyle(alignment),
                    font_size / k1,
                    0.0,
                    0.0,
                    text);
            break;
        default:
            fprintf(stderr, "Unsupported angle!\n");
            fprintf(file,
                    "        <text x=\"%.6lf\" y=\"%.6lf\" "
                    "style=\"%s\" "
                    "font-family=\"GOST type A\" font-size=\"%.06f\" "
                    "font-style=\"italic\" fill=\"black\">%s</text>\n",
                    x,
                    y + (font_size / k2) / 2.0,
                    textStyle(alignment),
                    font_size / k1,
                    text);
    }

    return 0;
}

int drawMultilineText(FILE* file, double x, double y, double font_size, const char* multiline_text, int angle)
{
    unsigned int num;
    double line_skip;
    double x_step = 0.0;
    double y_step = 0.0;

    TextLine columns(4U, 128U);

    num = columns.parseLine(multiline_text, '\n');

    if (num > 4U)
        return -1;
    if (num == 0U)
        return 0;

    switch (angle) {
        case 0:
            line_skip = 5.0;
            y = y - (num - 1) * line_skip / 2.0;
            y_step = line_skip;
            break;
        case -90:
            line_skip = 3.0;
            x = x - (num - 1) * line_skip / 2.0;
            x_step = line_skip;
            break;
        default:
            return -1;
    }

    for (unsigned int i = 0U; i < num; ++i) {
        if (columns[i][0] != '\0')
            KXSvg::drawText(file, x, y, font_size, columns[i], KXSvg::Center, angle);
        x += x_step;
        y += y_step;
    }

    return 0;
}

}

Page::Page(PageSize page_size, bool landscape)
{
    setSize(page_size, landscape);
    m_is_ended = false;
}

int Page::setSize(PageSize page_size, bool landscape)
{
    Page::m_page_size = page_size;

    switch (page_size) {
        case A0:
            m_width = 841.0;
            m_height = 1189.0;
            break;
        case A1:
            m_width = 594.0;
            m_height = 841.0;
            break;
        case A2:
            m_width = 420.0;
            m_height = 594.0;
            break;
        case A3:
            m_width = 297.0;
            m_height = 420.0;
            break;
        case A4:
            m_width = 210.0;
            m_height = 297.0;
            break;
        default:
            fprintf(stderr, "Invalid page size!\n");
            m_width = 210.0;
            m_height = 297.0;
    }

    if (landscape)
        std::swap(m_width, m_height);

    return 0;
}

const char* Page::formatName() const
{
    switch (m_page_size) {
        case A0:
            return "A0";
        case A1:
            return "A1";
        case A2:
            return "A2";
        case A3:
            return "A3";
        case A4:
            return "A4";
        default:
            fprintf(stderr, "Invalid page size!\n");
            return "??";
    }
}

int Page::begin(FILE* file) const
{
    m_is_ended = false;

    fprintf(file, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
    fprintf(file, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
    fprintf(file, "    \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
    fprintf(file,
            "<svg width=\"%.6lfmm\" height=\"%.6lfmm\" "
            "viewBox=\"0 0 %.6lf %.6lf\" version=\"1.1\"\n",
            m_width,
            m_height,
            m_width,
            m_height);
    fprintf(file, "    xmlns=\"http://www.w3.org/2000/svg\">\n");

    return 0;
}

int Page::end(FILE* file) const
{
    fprintf(file, "</svg>\n");

    m_is_ended = true;

    return 0;
}

void Element::setOffset(double x_offs, double y_offs)
{
    m_x_offs = x_offs;
    m_y_offs = y_offs;
}

int OutFile::open(const char* file_name)
{
    m_file = fopen(file_name, "w");
    if (m_file == nullptr)
        return -1;
    return 0;
}

void OutFile::close()
{
    if (m_file != nullptr)
        fclose(m_file);
    m_file = nullptr;
}
