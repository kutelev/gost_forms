#include <string.h>
#include <stdio.h>

#include "kxparsetablelib.h"

#define BEGIN           0
#define END             1

static void copy(char* dest, const char* src, size_t n)
{
    unsigned int i;
    for(i = 0; i < n; i++) {
        if(src[0] == '\0') {
            dest[0] = '\0';
            break;
        }
        if( (src[0] == '\\') && (src[1] == '%') )src++;
        dest[0] = src[0];
        dest++;
        src++;
    }
    dest[0] = '\0';
}

static int parseLine(const char*const line, const unsigned int max_columns_cnt,
    char*const*const columns, const unsigned int max_column_len, const char sep)
{
    unsigned int i, j, idx = 0, col_len, trailing = 0;
    unsigned int pos[2*MAX_COLUMNS_NUM];
    int          flag = BEGIN;

    if(max_columns_cnt > MAX_COLUMNS_NUM)return -1;

    if(line[0] == '%')return 0;

    for(i = 0; line[i] != '\0'; i++) {
        if(idx == (2*max_columns_cnt))break;

        if(flag == BEGIN && line[i] != ' ') {
            pos[idx++] = i;
            trailing = 0;
            flag = END;
        }

        if(flag == END) {
            if( line[i] == sep && (i == 0 || line[i-1] != '\\') ) {
                pos[idx++] = i + 1 - trailing;
                flag = BEGIN;
            } else if(line[i] == ' ')trailing++;
            else if(line[i] != '%')trailing = 0;
            else if(line[i-1] != '\\')break;
        }
    }

    if(flag == END)pos[idx] = i + 1 - trailing;
    else pos[idx] = i - trailing; /* TODO TODO TODO TODO */

    if(pos[idx] >= 2 && ( line[pos[idx] - 2] == '\012' ||
                         line[pos[idx] - 2] == '\015' ) )pos[idx]--;
    if(pos[idx] >= 2 && ( line[pos[idx] - 2] == '\015' ||
                         line[pos[idx] - 2] == '\012' ) )pos[idx]--;

    idx++;

    if(idx == 2 && (pos[1] - pos[0] == 1))return 0; /* Skip empty line */

    for(i = 0, j = 0; i < idx; i+=2, j++) {
        if(pos[i + 1] > pos[i])col_len = pos[i + 1] - pos[i] - 1;
        else col_len = 0;
        if(col_len)copy(columns[j], &line[pos[i]],
            col_len >= max_column_len ? max_column_len - 1 : col_len);
        columns[j][col_len >= max_column_len ? max_column_len - 1 : col_len] = '\0';
    }

    return j;
}

TextLine::TextLine(int max_columns_num, int max_column_len)
{
    TextLine::max_columns_num = max_columns_num;
    TextLine::max_column_len = max_column_len;
    columns_num = 0;
    columns_data = new char*[max_columns_num];
    for(int i = 0; i < max_columns_num; i++) {
        columns_data[i] = new char[max_column_len];
        columns_data[i][0] = '\0';
    }
}

TextLine::~TextLine()
{
    for(int i = 0; i < max_columns_num; i++)delete [] columns_data[i];
    delete [] columns_data;
}

int TextLine::parseLine(const char * line, char delimiter)
{
    for(int i = 0; i < max_columns_num; i++)columns_data[i][0] = '\0';
    columns_num = ::parseLine(line, max_columns_num, columns_data, max_column_len, delimiter);
    return columns_num;
}
