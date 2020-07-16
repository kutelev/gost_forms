#ifndef __TABLE_H__
#define __TABLE_H__

#define MAX_COLUMNS_NUM 32
#define MAX_LINE_LEN 1024

class TextLine {
public:
    TextLine(int max_columns_num, int max_column_len);
    ~TextLine();

    int parseLine(const char* line, char delimiter);
    int columnsNum() { return columns_num; }
    const char* operator[](int num) const { return columns_data[num]; }

protected:
    int max_columns_num;
    int max_column_len;
    int columns_num;
    char** columns_data;
};

#endif
