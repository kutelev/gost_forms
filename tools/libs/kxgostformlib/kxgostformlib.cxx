#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "kxgostformlib.h"

constexpr double font_size = 13.3231441;

int Frame::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    const double page_width = page.getWidth();
    const double page_height = page.getHeight();

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, 20.0, 5.0, page_width - 25.0, page_height - 10.0);

    std::string tmp = "Формат ";
    tmp.append(page.formatName());

    KXSvg::drawText(file, page_width - 30.0, page_height - 2.5, font_size, tmp.c_str());

    fprintf(file, "    </g>\n");

    return 0;
}

int Form::setField(unsigned int field_num, unsigned int sub_field, const char* text)
{
    switch (field_num) {
        case 1:
            if (sub_field <= 4U)
                m_field_1[sub_field] = text;
            else if (sub_field == 5U) {
                char* end_ptr;
                errno = 0;
                m_field_1_name_lines = strtol(text, &end_ptr, 10);
                if ((errno == ERANGE && (m_field_1_name_lines == LONG_MAX || m_field_1_name_lines == LONG_MIN)) || (errno != 0 && m_field_1_name_lines == 0) ||
                    (end_ptr == text) || (*end_ptr != '\0')) {
                    fprintf(stderr, "Invalid sub field (%u) value (%s)!\n", field_num, text);
                    m_field_1_name_lines = -1;
                }
            }
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 2:
            if (sub_field == 0U) {
                m_field_2 = text;
                m_field_26 = text;
            }
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 3:
            if (sub_field <= 2U)
                m_field_3[sub_field] = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 4:
            if (sub_field <= 2U)
                m_field_4[sub_field] = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 5:
            if (sub_field == 0U)
                m_field_5 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 6:
            if (sub_field == 0U)
                m_field_6 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 7:
            if (sub_field == 0U)
                m_field_7 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 8:
            if (sub_field == 0U)
                m_field_8 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 11:
            if (sub_field <= 5U)
                m_field_11[sub_field] = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 19:
            if (sub_field == 0U)
                m_field_19 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 21:
            if (sub_field == 0U)
                m_field_21 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 22:
            if (sub_field == 0U)
                m_field_22 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 24:
            if (sub_field == 0U)
                m_field_24 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 25:
            if (sub_field == 0U)
                m_field_25 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 26:
            if (sub_field == 0U) {
                m_field_2 = text;
                m_field_26 = text;
            }
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        default:
            fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
    }

    return 0;
}

int FormA1::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    constexpr unsigned int line_skip = 5U;

    setOffset(page.getWidth() - 190.0, page.getHeight() - 60.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absX(0.0), absY(0.0), 185.0, 55.0);

    KXSvg::drawLine(file, absX(65.0), absY(15.0), absX(185.0), absY(15.0));
    KXSvg::drawLine(file, absX(0.0), absY(20.0), absX(65.0), absY(20.0));
    KXSvg::drawLine(file, absX(135.0), absY(20.0), absX(185.0), absY(20.0));
    KXSvg::drawLine(file, absX(0.0), absY(25.0), absX(65.0), absY(25.0));
    KXSvg::drawLine(file, absX(135.0), absY(35.0), absX(185.0), absY(35.0));
    KXSvg::drawLine(file, absX(65.0), absY(40.0), absX(185.0), absY(40.0));

    KXSvg::drawLine(file, absX(7.0), absY(0.0), absX(7.0), absY(25.0));
    KXSvg::drawLine(file, absX(17.0), absY(0.0), absX(17.0), absY(55.0));
    KXSvg::drawLine(file, absX(40.0), absY(0.0), absX(40.0), absY(55.0));
    KXSvg::drawLine(file, absX(55.0), absY(0.0), absX(55.0), absY(55.0));
    KXSvg::drawLine(file, absX(65.0), absY(0.0), absX(65.0), absY(55.0));
    KXSvg::drawLine(file, absX(135.0), absY(15.0), absX(135.0), absY(55.0));
    KXSvg::drawLine(file, absX(150.0), absY(15.0), absX(150.0), absY(35.0));
    KXSvg::drawLine(file, absX(155.0), absY(35.0), absX(155.0), absY(40.0));
    KXSvg::drawLine(file, absX(167.0), absY(15.0), absX(167.0), absY(35.0));

    KXSvg::drawLine(file, absX(0.0), absY(5.0), absX(65.0), absY(5.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(10.0), absX(65.0), absY(10.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(15.0), absX(65.0), absY(15.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(30.0), absX(65.0), absY(30.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(35.0), absX(65.0), absY(35.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(40.0), absX(65.0), absY(40.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(45.0), absX(65.0), absY(45.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(50.0), absX(65.0), absY(50.0), false);

    KXSvg::drawLine(file, absX(140.0), absY(20.0), absX(140.0), absY(35.0), false);
    KXSvg::drawLine(file, absX(145.0), absY(20.0), absX(145.0), absY(35.0), false);

    KXSvg::drawText(file, absX(1.0), absY(22.5), font_size, "Изм.", KXSvg::Left);
    KXSvg::drawText(file, absX(8.0), absY(22.5), font_size, "Лист", KXSvg::Left);
    KXSvg::drawText(file, absX(18.0), absY(22.5), font_size, "№ докум", KXSvg::Left);
    KXSvg::drawText(file, absX(41.0), absY(22.5), font_size, "Подп.", KXSvg::Left);
    KXSvg::drawText(file, absX(56.0), absY(22.5), font_size, "Дата", KXSvg::Left);

    KXSvg::drawText(file, absX(1.0), absY(27.5), font_size, "Разраб.", KXSvg::Left);
    KXSvg::drawText(file, absX(1.0), absY(32.5), font_size, "Пров.", KXSvg::Left);
    KXSvg::drawText(file, absX(1.0), absY(37.5), font_size, "Т. контр.", KXSvg::Left);
    KXSvg::drawText(file, absX(1.0), absY(47.5), font_size, "Н. контр.", KXSvg::Left);
    KXSvg::drawText(file, absX(1.0), absY(52.5), font_size, "Утв.", KXSvg::Left);

    KXSvg::drawText(file, absX(136.0), absY(17.5), font_size, "Лит.", KXSvg::Left);
    KXSvg::drawText(file, absX(151.0), absY(17.5), font_size, "Масса", KXSvg::Left);
    KXSvg::drawText(file, absX(168.0), absY(17.5), font_size, "Масштаб", KXSvg::Left);

    unsigned int sub_fields_cnt = 0U;
    for (unsigned int i = 0U; i < 5; ++i) {
        if (m_field_1[i].length() > 0)
            sub_fields_cnt = i + 1U;
    }

    auto y_start = static_cast<unsigned int>(round(15.0 + (25.0 / 2.0) - ((sub_fields_cnt - 1) * line_skip / 2.0)));

    if (m_field_1_name_lines == -1)
        m_field_1_name_lines = sub_fields_cnt - 1;
    if (m_field_1_name_lines > sub_fields_cnt)
        m_field_1_name_lines = sub_fields_cnt;

    for (unsigned int i = 0U; i < sub_fields_cnt; ++i) {
        if (m_field_1[i].length() > 0)
            KXSvg::drawText(
                file, absX(100.0), absY(y_start + i * line_skip), i < (unsigned int)m_field_1_name_lines ? font_size + 2.0 : font_size, m_field_1[i].c_str());
    }

    if (m_field_2.length() > 0)
        KXSvg::drawText(file, absX(125.0), absY(7.5), 2.0 * font_size, m_field_2.c_str());

    sub_fields_cnt = 0U;
    for (unsigned int i = 0U; i < 3U; ++i) {
        if (m_field_3[i].length() > 0)
            sub_fields_cnt = i + 1U;
    }

    y_start = static_cast<unsigned int>(round(40.0 + (15.0 / 2.0) - ((sub_fields_cnt - 1) * line_skip / 2.0)));

    for (unsigned int i = 0; i < sub_fields_cnt; ++i) {
        if (m_field_1[i].length() > 0)
            KXSvg::drawText(file, absX(100.0), absY(y_start + i * line_skip), font_size, m_field_3[i].c_str());
    }

    sub_fields_cnt = 0U;
    for (unsigned int i = 0U; i < 3; ++i) {
        if (m_field_4[i].length() > 0)
            sub_fields_cnt = i + 1U;
    }

    for (unsigned int i = 0U; i < sub_fields_cnt; i++) {
        if (m_field_4[i].length() > 0)
            KXSvg::drawText(file, absX(137.5 + 5.0 * i), absY(27.5), font_size, m_field_4[i].c_str());
    }

    if (m_field_5.length() > 0)
        KXSvg::drawText(file, absX(158.5), absY(27.5), font_size, m_field_5.c_str());

    if (m_field_6.length() > 0)
        KXSvg::drawText(file, absX(176.0), absY(27.5), font_size, m_field_6.c_str());

    if (m_field_7.length() > 0)
        m_field_7 = "Лист " + m_field_7;
    else
        m_field_7 = "Лист";
    KXSvg::drawText(file, absX(145.0), absY(37.5), font_size, m_field_7.c_str());

    if (m_field_8.length() > 0)
        m_field_8 = "Листов " + m_field_8;
    else
        m_field_8 = "Листов";
    KXSvg::drawText(file, absX(170.0), absY(37.5), font_size, m_field_8.c_str());

    for (unsigned int i = 0; i < 6; ++i) {
        if (m_field_11[i].length() > 0)
            KXSvg::drawText(file, absX(18.0), absY(27.5 + 5.0 * i), font_size, m_field_11[i].c_str(), KXSvg::Left);
    }

    fprintf(file, "    </g>\n");

    return 0;
}

int FormA2::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    unsigned int y_start, line_skip = 5.0;

    setOffset(page.getWidth() - 190.0, page.getHeight() - 45.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absX(0.0), absY(0.0), 185.0, 40.0);

    KXSvg::drawLine(file, absX(0.0), absY(10.0), absX(65.0), absY(10.0));
    KXSvg::drawLine(file, absX(0.0), absY(15.0), absX(185.0), absY(15.0));
    KXSvg::drawLine(file, absX(135.0), absY(20.0), absX(185.0), absY(20.0));
    KXSvg::drawLine(file, absX(135.0), absY(25.0), absX(185.0), absY(25.0));

    KXSvg::drawLine(file, absX(7.0), absY(0.0), absX(7.0), absY(15.0));
    KXSvg::drawLine(file, absX(17.0), absY(0.0), absX(17.0), absY(40.0));
    KXSvg::drawLine(file, absX(40.0), absY(0.0), absX(40.0), absY(40.0));
    KXSvg::drawLine(file, absX(55.0), absY(0.0), absX(55.0), absY(40.0));
    KXSvg::drawLine(file, absX(65.0), absY(0.0), absX(65.0), absY(40.0));
    KXSvg::drawLine(file, absX(135.0), absY(15.0), absX(135.0), absY(40.0));
    KXSvg::drawLine(file, absX(150.0), absY(15.0), absX(150.0), absY(25.0));
    KXSvg::drawLine(file, absX(165.0), absY(15.0), absX(165.0), absY(25.0));

    KXSvg::drawLine(file, absX(0.0), absY(5.0), absX(65.0), absY(5.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(20.0), absX(65.0), absY(20.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(25.0), absX(65.0), absY(25.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(30.0), absX(65.0), absY(30.0), false);
    KXSvg::drawLine(file, absX(0.0), absY(35.0), absX(65.0), absY(35.0), false);

    KXSvg::drawLine(file, absX(140.0), absY(20.0), absX(140.0), absY(25.0), false);
    KXSvg::drawLine(file, absX(145.0), absY(20.0), absX(145.0), absY(25.0), false);

    KXSvg::drawText(file, absX(1.0), absY(12.5), font_size, "Изм.", KXSvg::Left);
    KXSvg::drawText(file, absX(8.0), absY(12.5), font_size, "Лист", KXSvg::Left);
    KXSvg::drawText(file, absX(18.0), absY(12.5), font_size, "№ докум", KXSvg::Left);
    KXSvg::drawText(file, absX(41.0), absY(12.5), font_size, "Подп.", KXSvg::Left);
    KXSvg::drawText(file, absX(56.0), absY(12.5), font_size, "Дата", KXSvg::Left);

    KXSvg::drawText(file, absX(1.0), absY(17.5), font_size, "Разраб.", KXSvg::Left);
    KXSvg::drawText(file, absX(1.0), absY(22.5), font_size, "Пров.", KXSvg::Left);

    KXSvg::drawText(file, absX(1.0), absY(32.5), font_size, "Н. контр.", KXSvg::Left);
    KXSvg::drawText(file, absX(1.0), absY(37.5), font_size, "Утв.", KXSvg::Left);

    KXSvg::drawText(file, absX(142.5), absY(17.5), font_size, "Лит.");
    KXSvg::drawText(file, absX(157.5), absY(17.5), font_size, "Лист");
    KXSvg::drawText(file, absX(175.0), absY(17.5), font_size, "Листов");

    unsigned int sub_fields_cnt = 0U;
    for (unsigned int i = 0U; i < 5U; ++i) {
        if (m_field_1[i].length() > 0)
            sub_fields_cnt = i + 1U;
    }

    y_start = static_cast<unsigned int>(round(15.0 + (25.0 / 2.0) - ((sub_fields_cnt - 1) * line_skip / 2.0)));

    if (m_field_1_name_lines == -1)
        m_field_1_name_lines = sub_fields_cnt - 1;
    if (static_cast<unsigned int>(m_field_1_name_lines) > sub_fields_cnt)
        m_field_1_name_lines = m_field_1_name_lines;

    for (unsigned int i = 0U; i < sub_fields_cnt; ++i) {
        if (m_field_1[i].length() > 0)
            KXSvg::drawText(file,
                            absX(100.0),
                            absY(y_start + i * line_skip),
                            i < static_cast<unsigned int>(m_field_1_name_lines) ? font_size + 2.0 : font_size,
                            m_field_1[i].c_str());
    }

    if (m_field_2.length() > 0)
        KXSvg::drawText(file, absX(125.0), absY(7.5), 2.0 * font_size, m_field_2.c_str());

    sub_fields_cnt = 0U;
    for (unsigned int i = 0U; i < 3U; ++i) {
        if (m_field_4[i].length() > 0)
            sub_fields_cnt = i + 1U;
    }

    for (unsigned int i = 0U; i < sub_fields_cnt; ++i) {
        if (m_field_4[i].length() > 0)
            KXSvg::drawText(file, absX(137.5 + 5.0 * i), absY(22.5), font_size, m_field_4[i].c_str());
    }

    if (m_field_7.length() > 0)
        KXSvg::drawText(file, absX(157.5), absY(22.5), font_size, m_field_7.c_str());

    if (m_field_8.length() > 0)
        KXSvg::drawText(file, absX(175.0), absY(22.5), font_size, m_field_8.c_str());

    for (unsigned int i = 0U; i < 5U; ++i) {
        if (m_field_11[i].length() > 0)
            KXSvg::drawText(file, absX(18.0), absY(17.5 + 5.0 * i), font_size, m_field_11[i].c_str(), KXSvg::Left);
    }

    fprintf(file, "    </g>\n");

    return 0;
}

int FormA3::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    setOffset(page.getWidth() - 190.0, page.getHeight() - 20.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absX(0.0), absY(0.0), 185.0, 15.0);

    KXSvg::drawLine(file, absX(0.0), absY(10.0), absX(65.0), absY(10.0));
    KXSvg::drawLine(file, absX(175.0), absY(7.0), absX(185.0), absY(7.0));

    KXSvg::drawLine(file, absX(7.0), absY(0.0), absX(7.0), absY(15.0));
    KXSvg::drawLine(file, absX(17.0), absY(0.0), absX(17.0), absY(15.0));
    KXSvg::drawLine(file, absX(40.0), absY(0.0), absX(40.0), absY(15.0));
    KXSvg::drawLine(file, absX(55.0), absY(0.0), absX(55.0), absY(15.0));
    KXSvg::drawLine(file, absX(65.0), absY(0.0), absX(65.0), absY(15.0));
    KXSvg::drawLine(file, absX(175.0), absY(0.0), absX(175.0), absY(15.0));

    KXSvg::drawLine(file, absX(0.0), absY(5.0), absX(65.0), absY(5.0), false);

    KXSvg::drawText(file, absX(1.0), absY(12.5), font_size, "Изм.", KXSvg::Left);
    KXSvg::drawText(file, absX(8.0), absY(12.5), font_size, "Лист", KXSvg::Left);
    KXSvg::drawText(file, absX(18.0), absY(12.5), font_size, "№ докум", KXSvg::Left);
    KXSvg::drawText(file, absX(41.0), absY(12.5), font_size, "Подп.", KXSvg::Left);
    KXSvg::drawText(file, absX(56.0), absY(12.5), font_size, "Дата", KXSvg::Left);

    KXSvg::drawText(file, absX(180.0), absY(3.5), font_size, "Лист");

    if (m_field_2.length() > 0)
        KXSvg::drawText(file, absX(120.0), absY(7.5), 2.0 * font_size, m_field_2.c_str());

    if (m_field_7.length() > 0)
        KXSvg::drawText(file, absX(180.0), absY(11.0), font_size, m_field_7.c_str());

    fprintf(file, "    </g>\n");

    return 0;
}

int FormB::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    setOffset(8.0, page.getHeight() - 150.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absX(0.0), absY(0.0), 12.0, 145.0);

    KXSvg::drawLine(file, absX(0.0), absY(35.0), absX(12.0), absY(35.0));
    KXSvg::drawLine(file, absX(0.0), absY(60.0), absX(12.0), absY(60.0));
    KXSvg::drawLine(file, absX(0.0), absY(85.0), absX(12.0), absY(85.0));
    KXSvg::drawLine(file, absX(0.0), absY(120.0), absX(12.0), absY(120.0));

    KXSvg::drawLine(file, absX(5.0), absY(0.0), absX(5.0), absY(145.0));

    KXSvg::drawText(file, absX(2.5), absY(144.0), font_size, "Инв. № подл.", KXSvg::Left, -90);
    KXSvg::drawText(file, absX(2.5), absY(119.0), font_size, "Подп. и дата", KXSvg::Left, -90);
    KXSvg::drawText(file, absX(2.5), absY(84.0), font_size, "Взам. инв. №", KXSvg::Left, -90);
    KXSvg::drawText(file, absX(2.5), absY(59.0), font_size, "Инв. № дубл.", KXSvg::Left, -90);
    KXSvg::drawText(file, absX(2.5), absY(34.0), font_size, "Подп. и дата", KXSvg::Left, -90);

    if (m_field_19.length() > 0)
        KXSvg::drawText(file, absX(8.5), absY(144.0), font_size, m_field_19.c_str(), KXSvg::Left, -90);

    if (m_field_21.length() > 0)
        KXSvg::drawText(file, absX(8.5), absY(84.0), font_size, m_field_21.c_str(), KXSvg::Left, -90);

    if (m_field_22.length() > 0)
        KXSvg::drawText(file, absX(8.5), absY(59.0), font_size, m_field_22.c_str(), KXSvg::Left, -90);

    fprintf(file, "    </g>\n");

    return 0;
}

int FormC::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    setOffset(8.0, page.getHeight() - 292.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absX(0.0), absY(0.0), 12.0, 120.0);

    KXSvg::drawLine(file, absX(0.0), absY(60.0), absX(12.0), absY(60.0));

    KXSvg::drawLine(file, absX(5.0), absY(0.0), absX(5.0), absY(120.0));

    KXSvg::drawText(file, absX(2.5), absY(119.0), font_size, "Справ. №", KXSvg::Left, -90);
    KXSvg::drawText(file, absX(2.5), absY(59.0), font_size, "Перв. примен.", KXSvg::Left, -90);

    if (m_field_24.length() > 0)
        KXSvg::drawText(file, absX(8.5), absY(119.0), font_size, m_field_24.c_str(), KXSvg::Left, -90);

    if (m_field_25.length() > 0)
        KXSvg::drawText(file, absX(8.5), absY(59.0), font_size, m_field_25.c_str(), KXSvg::Left, -90);

    fprintf(file, "    </g>\n");

    return 0;
}

int FormD::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    setOffset(20.0, 5.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absX(0.0), absY(0.0), 70.0, 14.0);

    if (m_field_26.length() > 0)
        KXSvg::drawText(file, absX(35.0), absY(7.0), 2.0 * font_size, m_field_26.c_str(), KXSvg::Center, 180);

    fprintf(file, "    </g>\n");

    return 0;
}

int FormDrawing::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    if (page_number == 0U)
        return -1;

    Frame::draw(file, page, page_number, flags);
    if (page_number == 1)
        FormA1::draw(file, page, page_number, flags);
    else
        FormA3::draw(file, page, page_number, flags);
    FormB::draw(file, page, page_number, flags);
    if (page_number == 1)
        FormC::draw(file, page, page_number, flags);
    if (flags == 0)
        FormD::draw(file, page, page_number, flags);

    return 0;
}

int FormText::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    if (page_number == 0)
        return -1;

    Frame::draw(file, page, page_number, flags);
    if (page_number == 1)
        FormA2::draw(file, page, page_number, flags);
    else
        FormA3::draw(file, page, page_number, flags);
    FormB::draw(file, page, page_number, flags);
    if (page_number == 1)
        FormC::draw(file, page, page_number, flags);

    return 0;
}

int FormTextPlus::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    if (page_number == 0)
        return -1;

    Frame::draw(file, page, page_number, flags);
    if (page_number == 2)
        FormA2::draw(file, page, page_number, flags);
    else if (page_number > 2)
        FormA3::draw(file, page, page_number, flags);
    FormB::draw(file, page, page_number, flags);
    if (page_number == 2)
        FormC::draw(file, page, page_number, flags);

    return 0;
}

int ProgramText::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    if (page_number == 0)
        return -1;

    if (page_number < 2)
        FormB::draw(file, page, page_number, flags);

    return 0;
}

int ProgramTextPlus::draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags)
{
    if (page_number == 0)
        return -1;

    if (page_number < 3)
        FormB::draw(file, page, page_number, flags);

    return 0;
}
