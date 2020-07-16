#ifndef __KXGOSTFORMLIB_H__
#define __KXGOSTFORMLIB_H__

#include <string>

#include <kxsvglib.h>

#define LAST_PAGE 0x00000001

class Frame : public Element {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class Form : public Element {
public:
    int setField(unsigned int field_num, unsigned int sub_field, const char* text);

protected:
    std::string m_field_1[5];
    long m_field_1_name_lines{ -1 };
    std::string m_field_2;
    std::string m_field_3[3];
    std::string m_field_4[3];
    std::string m_field_5;
    std::string m_field_6;
    std::string m_field_7;
    std::string m_field_8;
    std::string m_field_11[6];

    std::string m_field_19;
    std::string m_field_21;
    std::string m_field_22;

    std::string m_field_24;
    std::string m_field_25;

    std::string m_field_26;
};

class FormA1 : virtual public Form {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class FormA2 : virtual public Form {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class FormA3 : virtual public Form {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class FormB : virtual public Form {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class FormC : virtual public Form {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class FormD : virtual public Form {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class FormDrawing
    : public Frame
    , public FormA1
    , public FormA3
    , public FormB
    , public FormC
    , public FormD {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class FormText
    : public Frame
    , public FormA2
    , public FormA3
    , public FormB
    , public FormC
    , public FormD {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class FormTextPlus
    : public Frame
    , public FormA2
    , public FormA3
    , public FormB
    , public FormC
    , public FormD {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class ProgramText : public FormB {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

class ProgramTextPlus : public FormB {
public:
    int draw(FILE* file, const Page& page, unsigned int page_number, unsigned int flags) override;
};

#endif
