#include <stdio.h>
#include <string>

#include <kxparsetablelib.h>

#include "kxsvglib.h"

static const char* const textStyle(const KXSvg::Alignment alignmet)
{
    switch (alignmet) {
        case KXSvg::Left:
            return (const char* const) "text-align:start;text-anchor:start";
        case KXSvg::Center:
            return (const char* const) "text-align:center;text-anchor:middle";
        case KXSvg::Right:
            return (const char* const) "text-align:end;text-anchor:end";
        default:
            fprintf(stderr, "Invalid alignmet type!\n");
            return (const char* const) "text-align:center;text-anchor:middle";
    }
}

namespace KXSvg {

int drawRect(FILE* file, const double x, const double y, const double width, const double height, const bool bold)
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

int drawLine(FILE* file, const double x1, const double y1, const double x2, const double y2, const bool bold)
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

int drawText(FILE* file, const double x, const double y, const double font_size, const char* const text, const Alignment alignment, const int angle)
{
    const double k1 = 3.54330700988902143941095308362109464599991908268;
    const double k2 = 4.65675812022197213706391400520004656758120221972;

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

int drawMultilineText(FILE* file, double x, double y, double font_size, const char* multiline_text, Alignment alignment, int angle)
{
    int num;
    double line_skip;
    double x_step = 0.0;
    double y_step = 0.0;

    TextLine columns(4, 128);

    num = columns.parseLine(multiline_text, '\n');

    if (num < 0 || num > 4)
        return -1;
    if (num == 0)
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

    for (int i = 0; i < num; i++, x += x_step, y += y_step) {
        if (columns[i][0] != '\0')
            KXSvg::drawText(file, x, y, font_size, columns[i], KXSvg::Center, angle);
    }

    return 0;
}

}

Page::Page(const Page::PageSize page_size, const bool landscape)
{
    setSize(page_size, landscape);
    is_ended = false;
}

int Page::setSize(const Page::PageSize page_size, const bool landscape)
{
    Page::page_size = page_size;

    switch (page_size) {
        case A0:
            width = 841.0;
            height = 1189.0;
            break;
        case A1:
            width = 594.0;
            height = 841.0;
            break;
        case A2:
            width = 420.0;
            height = 594.0;
            break;
        case A3:
            width = 297.0;
            height = 420.0;
            break;
        case A4:
            width = 210.0;
            height = 297.0;
            break;
        default:
            fprintf(stderr, "Invalid page size!\n");
            width = 210.0;
            height = 297.0;
    }

    if (landscape) {
        double tmp;
        tmp = width;
        width = height;
        height = tmp;
    }

    return 0;
}

const char* const Page::formatName() const
{
    switch (page_size) {
        case A0:
            return (const char* const) "A0";
        case A1:
            return (const char* const) "A1";
        case A2:
            return (const char* const) "A2";
        case A3:
            return (const char* const) "A3";
        case A4:
            return (const char* const) "A4";
        default:
            fprintf(stderr, "Invalid page size!\n");
            return (const char* const) "??";
    }
}

int Page::begin(FILE* file) const
{
    is_ended = false;

    fprintf(file, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
    fprintf(file, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
    fprintf(file, "    \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
    fprintf(file,
            "<svg width=\"%.6lfmm\" height=\"%.6lfmm\" "
            "viewBox=\"0 0 %.6lf %.6lf\" version=\"1.1\"\n",
            width,
            height,
            width,
            height);
    fprintf(file, "    xmlns=\"http://www.w3.org/2000/svg\">\n");

    return 0;
}

int Page::end(FILE* file) const
{
    fprintf(file, "</svg>\n");

    is_ended = true;

    return 0;
}

int Element::setOffset(const double x_offs, const double y_offs)
{
    Element::x_offs = x_offs;
    Element::y_offs = y_offs;

    return 0;
}

int OutFile::open(const char* file_name)
{
    file_ = fopen(file_name, "w");
    if (file_ == NULL)
        return -1;
    return 0;
}

void OutFile::close()
{
    if (file_ != NULL)
        fclose(file_);
    file_ = NULL;
}
