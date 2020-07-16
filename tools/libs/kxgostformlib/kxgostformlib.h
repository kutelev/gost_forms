#ifndef __KXGOSTFORMLIB_H__
#define __KXGOSTFORMLIB_H__

#include <kxsvglib.h>
#include <string>

#define LAST_PAGE 0x00000001

class Frame : public Element {
public:
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags) = 0;
};

class Form : public Element {
public:
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags) = 0;
    Form() { field_1_name_lines = -1; }
    virtual ~Form(){};

    int setField(const unsigned int field_num, const unsigned int sub_field, const char* const text);
    int copyFields(const Form* form);

protected:
    std::string field_1[5];
    int field_1_name_lines;
    std::string field_2;
    std::string field_3[3];
    std::string field_4[3];
    std::string field_5;
    std::string field_6;
    std::string field_7;
    std::string field_8;
    std::string field_11[6];

    std::string field_19;
    std::string field_21;
    std::string field_22;

    std::string field_24;
    std::string field_25;

    std::string field_26;
};

class FormA1 : virtual public Form {
public:
    virtual ~FormA1() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

class FormA2 : virtual public Form {
public:
    virtual ~FormA2() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

class FormA3 : virtual public Form {
public:
    virtual ~FormA3() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

class FormB : virtual public Form {
public:
    virtual ~FormB() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

class FormC : virtual public Form {
public:
    virtual ~FormC() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

class FormD : virtual public Form {
public:
    virtual ~FormD() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

class FormDrawing
    : public Frame
    , public FormA1
    , public FormA3
    , public FormB
    , public FormC
    , public FormD {
public:
    virtual ~FormDrawing() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

class FormText
    : public Frame
    , public FormA2
    , public FormA3
    , public FormB
    , public FormC
    , public FormD {
public:
    virtual ~FormText() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

class FormTextPlus
    : public Frame
    , public FormA2
    , public FormA3
    , public FormB
    , public FormC
    , public FormD {
public:
    virtual ~FormTextPlus() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

class ProgramText : public FormB {
public:
    virtual ~ProgramText() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

class ProgramTextPlus : public FormB {
public:
    virtual ~ProgramTextPlus() {}
    virtual int draw(FILE* file, const Page* const page, const unsigned int page_number, const unsigned int flags);
};

#endif
