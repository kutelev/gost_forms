#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "kxgostformlib.h"

static const double font_size = 13.3231441;

int Frame::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
{
    const double page_width = page->getWidth();
    const double page_height = page->getHeight();

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, 20.0, 5.0, page_width - 25.0, page_height - 10.0);

    std::string tmp = "Формат ";
    tmp.append(page->formatName());

    KXSvg::drawText(file, page_width - 30.0, page_height - 2.5, font_size, tmp.c_str());

    fprintf(file, "    </g>\n");

    return 0;
}

int Form::setField(const unsigned int field_num, const unsigned int sub_field_num, const char* const text)
{
    switch (field_num) {
        case 1:
            if (sub_field_num <= 4)
                field_1[sub_field_num] = text;
            else if (sub_field_num == 5) {
                char* end_ptr;
                errno = 0;
                field_1_name_lines = strtol(text, &end_ptr, 10);
                if ((errno == ERANGE && (field_1_name_lines == LONG_MAX || field_1_name_lines == LONG_MIN)) || (errno != 0 && field_1_name_lines == 0) ||
                    (end_ptr == text) || (*end_ptr != '\0')) {
                    fprintf(stderr, "Invalid sub field (%u) value (%s)!\n", field_num, text);
                    field_1_name_lines = -1;
                }
            }
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 2:
            if (sub_field_num == 0) {
                field_2 = text;
                field_26 = text;
            }
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 3:
            if (sub_field_num <= 2)
                field_3[sub_field_num] = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 4:
            if (sub_field_num <= 2)
                field_4[sub_field_num] = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 5:
            if (sub_field_num == 0)
                field_5 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 6:
            if (sub_field_num == 0)
                field_6 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 7:
            if (sub_field_num == 0)
                field_7 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 8:
            if (sub_field_num == 0)
                field_8 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 11:
            if (sub_field_num <= 5)
                field_11[sub_field_num] = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 19:
            if (sub_field_num == 0)
                field_19 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 21:
            if (sub_field_num == 0)
                field_21 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 22:
            if (sub_field_num == 0)
                field_22 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 24:
            if (sub_field_num == 0)
                field_24 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 25:
            if (sub_field_num == 0)
                field_25 = text;
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        case 26:
            if (sub_field_num == 0) {
                field_2 = text;
                field_26 = text;
            }
            else
                fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
            break;
        default:
            fprintf(stderr, "Invalid sub field number (%u)!\n", field_num);
    }

    return 0;
}

int Form::copyFields(const Form* form)
{
    field_1[0] = form->field_1[0];
    field_1[1] = form->field_1[1];
    field_1[2] = form->field_1[2];
    field_1[3] = form->field_1[3];
    field_1[4] = form->field_1[4];
    field_2 = form->field_2;
    field_3[0] = form->field_3[0];
    field_3[1] = form->field_3[1];
    field_3[2] = form->field_3[2];
    field_4[0] = form->field_4[0];
    field_4[1] = form->field_4[1];
    field_4[2] = form->field_4[2];
    field_5 = form->field_5;
    field_6 = form->field_6;
    field_11[0] = form->field_11[0];
    field_11[1] = form->field_11[1];
    field_11[2] = form->field_11[2];
    field_11[3] = form->field_11[3];
    field_11[4] = form->field_11[4];
    field_11[5] = form->field_11[5];

    field_19 = form->field_19;
    field_21 = form->field_21;
    field_22 = form->field_22;

    field_24 = form->field_24;
    field_25 = form->field_25;

    field_26 = form->field_26;

    return 0;
}

int FormA1::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
{
    unsigned int sub_fields_cnt = 0, i, y_start, line_skip = 5.0;

    setOffset(page->getWidth() - 190.0, page->getHeight() - 60.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absx(0.0), absy(0.0), 185.0, 55.0);

    KXSvg::drawLine(file, absx(65.0), absy(15.0), absx(185.0), absy(15.0));
    KXSvg::drawLine(file, absx(0.0), absy(20.0), absx(65.0), absy(20.0));
    KXSvg::drawLine(file, absx(135.0), absy(20.0), absx(185.0), absy(20.0));
    KXSvg::drawLine(file, absx(0.0), absy(25.0), absx(65.0), absy(25.0));
    KXSvg::drawLine(file, absx(135.0), absy(35.0), absx(185.0), absy(35.0));
    KXSvg::drawLine(file, absx(65.0), absy(40.0), absx(185.0), absy(40.0));

    KXSvg::drawLine(file, absx(7.0), absy(0.0), absx(7.0), absy(25.0));
    KXSvg::drawLine(file, absx(17.0), absy(0.0), absx(17.0), absy(55.0));
    KXSvg::drawLine(file, absx(40.0), absy(0.0), absx(40.0), absy(55.0));
    KXSvg::drawLine(file, absx(55.0), absy(0.0), absx(55.0), absy(55.0));
    KXSvg::drawLine(file, absx(65.0), absy(0.0), absx(65.0), absy(55.0));
    KXSvg::drawLine(file, absx(135.0), absy(15.0), absx(135.0), absy(55.0));
    KXSvg::drawLine(file, absx(150.0), absy(15.0), absx(150.0), absy(35.0));
    KXSvg::drawLine(file, absx(155.0), absy(35.0), absx(155.0), absy(40.0));
    KXSvg::drawLine(file, absx(167.0), absy(15.0), absx(167.0), absy(35.0));

    KXSvg::drawLine(file, absx(0.0), absy(5.0), absx(65.0), absy(5.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(10.0), absx(65.0), absy(10.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(15.0), absx(65.0), absy(15.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(30.0), absx(65.0), absy(30.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(35.0), absx(65.0), absy(35.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(40.0), absx(65.0), absy(40.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(45.0), absx(65.0), absy(45.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(50.0), absx(65.0), absy(50.0), false);

    KXSvg::drawLine(file, absx(140.0), absy(20.0), absx(140.0), absy(35.0), false);
    KXSvg::drawLine(file, absx(145.0), absy(20.0), absx(145.0), absy(35.0), false);

    KXSvg::drawText(file, absx(1.0), absy(22.5), font_size, "Изм.", KXSvg::Left);
    KXSvg::drawText(file, absx(8.0), absy(22.5), font_size, "Лист", KXSvg::Left);
    KXSvg::drawText(file, absx(18.0), absy(22.5), font_size, "№ докум", KXSvg::Left);
    KXSvg::drawText(file, absx(41.0), absy(22.5), font_size, "Подп.", KXSvg::Left);
    KXSvg::drawText(file, absx(56.0), absy(22.5), font_size, "Дата", KXSvg::Left);

    KXSvg::drawText(file, absx(1.0), absy(27.5), font_size, "Разраб.", KXSvg::Left);
    KXSvg::drawText(file, absx(1.0), absy(32.5), font_size, "Пров.", KXSvg::Left);
    KXSvg::drawText(file, absx(1.0), absy(37.5), font_size, "Т. контр.", KXSvg::Left);
    KXSvg::drawText(file, absx(1.0), absy(47.5), font_size, "Н. контр.", KXSvg::Left);
    KXSvg::drawText(file, absx(1.0), absy(52.5), font_size, "Утв.", KXSvg::Left);

    KXSvg::drawText(file, absx(136.0), absy(17.5), font_size, "Лит.", KXSvg::Left);
    KXSvg::drawText(file, absx(151.0), absy(17.5), font_size, "Масса", KXSvg::Left);
    KXSvg::drawText(file, absx(168.0), absy(17.5), font_size, "Масштаб", KXSvg::Left);

    sub_fields_cnt = 0;
    for (i = 0; i < 5; i++)
        if (field_1[i].length() > 0)
            sub_fields_cnt = i + 1;

    y_start = 15.0 + (25.0 / 2.0) - ((sub_fields_cnt - 1) * line_skip / 2.0);

    if (field_1_name_lines == -1)
        field_1_name_lines = sub_fields_cnt - 1;
    if ((unsigned int)field_1_name_lines > sub_fields_cnt)
        field_1_name_lines = field_1_name_lines;

    for (i = 0; i < sub_fields_cnt; i++) {
        if (field_1[i].length() > 0)
            KXSvg::drawText(
                file, absx(100.0), absy(y_start + i * line_skip), i < (unsigned int)field_1_name_lines ? font_size + 2.0 : font_size, field_1[i].c_str());
    }

    if (field_2.length() > 0)
        KXSvg::drawText(file, absx(125.0), absy(7.5), 2.0 * font_size, field_2.c_str());

    sub_fields_cnt = 0;
    for (i = 0; i < 3; i++)
        if (field_3[i].length() > 0)
            sub_fields_cnt = i + 1;

    y_start = 40.0 + (15.0 / 2.0) - ((sub_fields_cnt - 1) * line_skip / 2.0);

    for (i = 0; i < sub_fields_cnt; i++) {
        if (field_1[i].length() > 0)
            KXSvg::drawText(file, absx(100.0), absy(y_start + i * line_skip), font_size, field_3[i].c_str());
    }

    sub_fields_cnt = 0;
    for (i = 0; i < 3; i++)
        if (field_4[i].length() > 0)
            sub_fields_cnt = i + 1;

    for (i = 0; i < sub_fields_cnt; i++) {
        if (field_4[i].length() > 0)
            KXSvg::drawText(file, absx(137.5 + 5.0 * i), absy(27.5), font_size, field_4[i].c_str());
    }

    if (field_5.length() > 0)
        KXSvg::drawText(file, absx(158.5), absy(27.5), font_size, field_5.c_str());

    if (field_6.length() > 0)
        KXSvg::drawText(file, absx(176.0), absy(27.5), font_size, field_6.c_str());

    if (field_7.length() > 0)
        field_7 = "Лист " + field_7;
    else
        field_7 = "Лист";
    KXSvg::drawText(file, absx(145.0), absy(37.5), font_size, field_7.c_str());

    if (field_8.length() > 0)
        field_8 = "Листов " + field_8;
    else
        field_8 = "Листов";
    KXSvg::drawText(file, absx(170.0), absy(37.5), font_size, field_8.c_str());

    for (i = 0; i < 6; i++) {
        if (field_11[i].length() > 0)
            KXSvg::drawText(file, absx(18.0), absy(27.5 + 5.0 * i), font_size, field_11[i].c_str(), KXSvg::Left);
    }

    fprintf(file, "    </g>\n");

    return 0;
}

int FormA2::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
{
    unsigned int sub_fields_cnt = 0, i, y_start, line_skip = 5.0;

    setOffset(page->getWidth() - 190.0, page->getHeight() - 45.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absx(0.0), absy(0.0), 185.0, 40.0);

    KXSvg::drawLine(file, absx(0.0), absy(10.0), absx(65.0), absy(10.0));
    KXSvg::drawLine(file, absx(0.0), absy(15.0), absx(185.0), absy(15.0));
    KXSvg::drawLine(file, absx(135.0), absy(20.0), absx(185.0), absy(20.0));
    KXSvg::drawLine(file, absx(135.0), absy(25.0), absx(185.0), absy(25.0));

    KXSvg::drawLine(file, absx(7.0), absy(0.0), absx(7.0), absy(15.0));
    KXSvg::drawLine(file, absx(17.0), absy(0.0), absx(17.0), absy(40.0));
    KXSvg::drawLine(file, absx(40.0), absy(0.0), absx(40.0), absy(40.0));
    KXSvg::drawLine(file, absx(55.0), absy(0.0), absx(55.0), absy(40.0));
    KXSvg::drawLine(file, absx(65.0), absy(0.0), absx(65.0), absy(40.0));
    KXSvg::drawLine(file, absx(135.0), absy(15.0), absx(135.0), absy(40.0));
    KXSvg::drawLine(file, absx(150.0), absy(15.0), absx(150.0), absy(25.0));
    KXSvg::drawLine(file, absx(165.0), absy(15.0), absx(165.0), absy(25.0));

    KXSvg::drawLine(file, absx(0.0), absy(5.0), absx(65.0), absy(5.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(20.0), absx(65.0), absy(20.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(25.0), absx(65.0), absy(25.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(30.0), absx(65.0), absy(30.0), false);
    KXSvg::drawLine(file, absx(0.0), absy(35.0), absx(65.0), absy(35.0), false);

    KXSvg::drawLine(file, absx(140.0), absy(20.0), absx(140.0), absy(25.0), false);
    KXSvg::drawLine(file, absx(145.0), absy(20.0), absx(145.0), absy(25.0), false);

    KXSvg::drawText(file, absx(1.0), absy(12.5), font_size, "Изм.", KXSvg::Left);
    KXSvg::drawText(file, absx(8.0), absy(12.5), font_size, "Лист", KXSvg::Left);
    KXSvg::drawText(file, absx(18.0), absy(12.5), font_size, "№ докум", KXSvg::Left);
    KXSvg::drawText(file, absx(41.0), absy(12.5), font_size, "Подп.", KXSvg::Left);
    KXSvg::drawText(file, absx(56.0), absy(12.5), font_size, "Дата", KXSvg::Left);

    KXSvg::drawText(file, absx(1.0), absy(17.5), font_size, "Разраб.", KXSvg::Left);
    KXSvg::drawText(file, absx(1.0), absy(22.5), font_size, "Пров.", KXSvg::Left);

    KXSvg::drawText(file, absx(1.0), absy(32.5), font_size, "Н. контр.", KXSvg::Left);
    KXSvg::drawText(file, absx(1.0), absy(37.5), font_size, "Утв.", KXSvg::Left);

    KXSvg::drawText(file, absx(142.5), absy(17.5), font_size, "Лит.");
    KXSvg::drawText(file, absx(157.5), absy(17.5), font_size, "Лист");
    KXSvg::drawText(file, absx(175.0), absy(17.5), font_size, "Листов");

    sub_fields_cnt = 0;
    for (i = 0; i < 5; i++)
        if (field_1[i].length() > 0)
            sub_fields_cnt = i + 1;

    y_start = 15.0 + (25.0 / 2.0) - ((sub_fields_cnt - 1) * line_skip / 2.0);

    if (field_1_name_lines == -1)
        field_1_name_lines = sub_fields_cnt - 1;
    if ((unsigned int)field_1_name_lines > sub_fields_cnt)
        field_1_name_lines = field_1_name_lines;

    for (i = 0; i < sub_fields_cnt; i++) {
        if (field_1[i].length() > 0)
            KXSvg::drawText(
                file, absx(100.0), absy(y_start + i * line_skip), i < (unsigned int)field_1_name_lines ? font_size + 2.0 : font_size, field_1[i].c_str());
    }

    if (field_2.length() > 0)
        KXSvg::drawText(file, absx(125.0), absy(7.5), 2.0 * font_size, field_2.c_str());

    sub_fields_cnt = 0;
    for (i = 0; i < 3; i++)
        if (field_4[i].length() > 0)
            sub_fields_cnt = i + 1;

    for (i = 0; i < sub_fields_cnt; i++) {
        if (field_4[i].length() > 0)
            KXSvg::drawText(file, absx(137.5 + 5.0 * i), absy(22.5), font_size, field_4[i].c_str());
    }

    if (field_7.length() > 0)
        KXSvg::drawText(file, absx(157.5), absy(22.5), font_size, field_7.c_str());

    if (field_8.length() > 0)
        KXSvg::drawText(file, absx(175.0), absy(22.5), font_size, field_8.c_str());

    for (i = 0; i < 5; i++) {
        if (field_11[i].length() > 0)
            KXSvg::drawText(file, absx(18.0), absy(17.5 + 5.0 * i), font_size, field_11[i].c_str(), KXSvg::Left);
    }

    fprintf(file, "    </g>\n");

    return 0;
}

int FormA3::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
{
    setOffset(page->getWidth() - 190.0, page->getHeight() - 20.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absx(0.0), absy(0.0), 185.0, 15.0);

    KXSvg::drawLine(file, absx(0.0), absy(10.0), absx(65.0), absy(10.0));
    KXSvg::drawLine(file, absx(175.0), absy(7.0), absx(185.0), absy(7.0));

    KXSvg::drawLine(file, absx(7.0), absy(0.0), absx(7.0), absy(15.0));
    KXSvg::drawLine(file, absx(17.0), absy(0.0), absx(17.0), absy(15.0));
    KXSvg::drawLine(file, absx(40.0), absy(0.0), absx(40.0), absy(15.0));
    KXSvg::drawLine(file, absx(55.0), absy(0.0), absx(55.0), absy(15.0));
    KXSvg::drawLine(file, absx(65.0), absy(0.0), absx(65.0), absy(15.0));
    KXSvg::drawLine(file, absx(175.0), absy(0.0), absx(175.0), absy(15.0));

    KXSvg::drawLine(file, absx(0.0), absy(5.0), absx(65.0), absy(5.0), false);

    KXSvg::drawText(file, absx(1.0), absy(12.5), font_size, "Изм.", KXSvg::Left);
    KXSvg::drawText(file, absx(8.0), absy(12.5), font_size, "Лист", KXSvg::Left);
    KXSvg::drawText(file, absx(18.0), absy(12.5), font_size, "№ докум", KXSvg::Left);
    KXSvg::drawText(file, absx(41.0), absy(12.5), font_size, "Подп.", KXSvg::Left);
    KXSvg::drawText(file, absx(56.0), absy(12.5), font_size, "Дата", KXSvg::Left);

    KXSvg::drawText(file, absx(180.0), absy(3.5), font_size, "Лист");

    if (field_2.length() > 0)
        KXSvg::drawText(file, absx(120.0), absy(7.5), 2.0 * font_size, field_2.c_str());

    if (field_7.length() > 0)
        KXSvg::drawText(file, absx(180.0), absy(11.0), font_size, field_7.c_str());

    fprintf(file, "    </g>\n");

    return 0;
}

int FormB::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
{
    setOffset(8.0, page->getHeight() - 150.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absx(0.0), absy(0.0), 12.0, 145.0);

    KXSvg::drawLine(file, absx(0.0), absy(35.0), absx(12.0), absy(35.0));
    KXSvg::drawLine(file, absx(0.0), absy(60.0), absx(12.0), absy(60.0));
    KXSvg::drawLine(file, absx(0.0), absy(85.0), absx(12.0), absy(85.0));
    KXSvg::drawLine(file, absx(0.0), absy(120.0), absx(12.0), absy(120.0));

    KXSvg::drawLine(file, absx(5.0), absy(0.0), absx(5.0), absy(145.0));

    KXSvg::drawText(file, absx(2.5), absy(144.0), font_size, "Инв. № подл.", KXSvg::Left, -90);
    KXSvg::drawText(file, absx(2.5), absy(119.0), font_size, "Подп. и дата", KXSvg::Left, -90);
    KXSvg::drawText(file, absx(2.5), absy(84.0), font_size, "Взам. инв. №", KXSvg::Left, -90);
    KXSvg::drawText(file, absx(2.5), absy(59.0), font_size, "Инв. № дубл.", KXSvg::Left, -90);
    KXSvg::drawText(file, absx(2.5), absy(34.0), font_size, "Подп. и дата", KXSvg::Left, -90);

    if (field_19.length() > 0)
        KXSvg::drawText(file, absx(8.5), absy(144.0), font_size, field_19.c_str(), KXSvg::Left, -90);

    if (field_21.length() > 0)
        KXSvg::drawText(file, absx(8.5), absy(84.0), font_size, field_21.c_str(), KXSvg::Left, -90);

    if (field_22.length() > 0)
        KXSvg::drawText(file, absx(8.5), absy(59.0), font_size, field_22.c_str(), KXSvg::Left, -90);

    fprintf(file, "    </g>\n");

    return 0;
}

int FormC::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
{
    setOffset(8.0, page->getHeight() - 292.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absx(0.0), absy(0.0), 12.0, 120.0);

    KXSvg::drawLine(file, absx(0.0), absy(60.0), absx(12.0), absy(60.0));

    KXSvg::drawLine(file, absx(5.0), absy(0.0), absx(5.0), absy(120.0));

    KXSvg::drawText(file, absx(2.5), absy(119.0), font_size, "Справ. №", KXSvg::Left, -90);
    KXSvg::drawText(file, absx(2.5), absy(59.0), font_size, "Перв. примен.", KXSvg::Left, -90);

    if (field_24.length() > 0)
        KXSvg::drawText(file, absx(8.5), absy(119.0), font_size, field_24.c_str(), KXSvg::Left, -90);

    if (field_25.length() > 0)
        KXSvg::drawText(file, absx(8.5), absy(59.0), font_size, field_25.c_str(), KXSvg::Left, -90);

    fprintf(file, "    </g>\n");

    return 0;
}

int FormD::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
{
    setOffset(20.0, 5.0);

    fprintf(file, "    <g>\n");

    KXSvg::drawRect(file, absx(0.0), absy(0.0), 70.0, 14.0);

    if (field_26.length() > 0)
        KXSvg::drawText(file, absx(35.0), absy(7.0), 2.0 * font_size, field_26.c_str(), KXSvg::Center, 180);

    fprintf(file, "    </g>\n");

    return 0;
}

int FormDrawing::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
{
    if (page_number == 0)
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

int FormText::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
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

int FormTextPlus::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
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

int ProgramText::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
{
    if (page_number == 0)
        return -1;

    if (page_number < 2)
        FormB::draw(file, page, page_number, flags);

    return 0;
}

int ProgramTextPlus::draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags)
{
    if (page_number == 0)
        return -1;

    if (page_number < 3)
        FormB::draw(file, page, page_number, flags);

    return 0;
}
