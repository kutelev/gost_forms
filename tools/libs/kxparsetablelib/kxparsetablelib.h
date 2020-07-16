#ifndef __TABLE_H__
#define __TABLE_H__

#define MAX_COLUMNS_NUM 32
#define MAX_LINE_LEN 1024

class TextLine {
public:
    TextLine(unsigned int max_column_count, unsigned int max_column_len);
    ~TextLine();

    unsigned int parseLine(const char* line, char delimiter);
    const char* operator[](unsigned int num) const { return m_columns_data[num]; }

protected:
    unsigned int m_max_columns_num;
    unsigned int m_max_column_len;
    unsigned int m_columns_num;
    char** m_columns_data;
};

#endif
