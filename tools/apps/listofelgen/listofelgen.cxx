#include <fstream>
#include <getopt.h>
#include <iostream>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <kxsvglib.h>

#define MAX_LINE_LEN 1024

static Page page(Page::A4, false);
static std::ifstream in_file;
static FILE* out_file = NULL;

static char* out_dir = (char*)".";
static char* file_name = NULL;

static int parseArgs(int argc, char** argv)
{
    int c;

    struct option long_options[] = {
        { "input", required_argument, NULL, 0 },
        { "dir", required_argument, NULL, 0 },
        { 0, 0, 0, 0 },
    };

    while (1) {
        int option_index = 0;

        c = getopt_long(argc, argv, "i:d:", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        file_name = optarg;
                        break;
                    case 1:
                        out_dir = optarg;
                        break;
                    default:
                        fprintf(stderr, "Invalid long argument\n");
                        return -1;
                }

                break;
            case 'i':
                file_name = optarg;
                break;
            case 'd':
                out_dir = optarg;
                break;
            case '?':
                fprintf(stderr, "Invalid short argument\n");
                return -1;
                break;

            default:
                fprintf(stderr, "getopt_long returned unexpected code\n");
        }
    }

    return 0;
}

static void printUsage()
{
    fprintf(stderr, "===============================\n");
    fprintf(stderr, "Usage: listofelgen [options]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "-i | --input    Input file name\n");
    fprintf(stderr, "-d | --dir      Output directory [.]\n");
    fprintf(stderr, "===============================\n");
}

void bye(void)
{
    if (in_file.is_open())
        in_file.close();
    if (out_file != NULL)
        fclose(out_file);
}

static const double font_size = 13.3231441;

int buildForm(FILE* out_file, const double payload_height, const unsigned int lines_per_page)
{
    const double line_height = payload_height / lines_per_page;

    KXSvg::drawRect(out_file, 20.0, 5.0, 185.0, 15.0 + payload_height);

    for (unsigned int i = 0; i < lines_per_page; i++)
        KXSvg::drawLine(out_file, 20.0, 20.0 + i * line_height, 205.0, 20.0 + i * line_height);

    KXSvg::drawLine(out_file, 40.0, 5.0, 40.0, 20.0 + payload_height);
    KXSvg::drawLine(out_file, 150.0, 5.0, 150.0, 20.0 + payload_height);
    KXSvg::drawLine(out_file, 160.0, 5.0, 160.0, 20.0 + payload_height);

    KXSvg::drawText(out_file, 30.0, 12.5 - 2.5, font_size, "Поз. обоз-");
    KXSvg::drawText(out_file, 30.0, 12.5 + 2.5, font_size, "начение");
    KXSvg::drawText(out_file, 95.0, 12.5, font_size, "Наименование");
    KXSvg::drawText(out_file, 155.0, 12.5, font_size, "Кол.");
    KXSvg::drawText(out_file, 182.5, 12.5, font_size, "Примечание");

    return 0;
}

static size_t _strlen(const char** const string)
{
    mbstate_t mbs;
    memset(&mbs, 0, sizeof(mbs)); /* MUST BE!!! */

    size_t len = mbsrtowcs(NULL, string, 0, &mbs);

    return len > 50 ? 50 : len;
}

int main(int argc, char** argv)
{
    char single_line[MAX_LINE_LEN];

    const char* token0;
    const char* token1;
    const char* token2;
    const char* token3;
    const char* token4;

    setlocale(LC_ALL, "en_US.utf8");

    atexit(bye);

    parseArgs(argc, argv);

    if (file_name == NULL) {
        printUsage();
        return -1;
    }

    in_file.open(file_name, std::ios_base::in);
    if (!in_file.is_open()) {
        fprintf(stderr, "Could not open file: \"%s\"\n", file_name);
        return -1;
    }

    unsigned int page_num = 1;
    double payload_height;
    unsigned int lines_per_page = 0;
    double line_height = 0, y = 0;

    char file_name[256];

    do {
        if (out_file == NULL) {
            sprintf(file_name, "%s/Sheet_%02u.svg", out_dir, page_num);
            out_file = fopen(file_name, "w");

            if (out_file == NULL) {
                fprintf(stderr, "Could not open file fo writing: \"%s\"\n", file_name);
                return -1;
            }

            if (page_num == 1) {
                payload_height = 232.0;
                lines_per_page = 29;
            }
            else {
                payload_height = 257.0;
                lines_per_page = 32;
            }

            line_height = payload_height / lines_per_page;
            y = 5.0 + 15.0 + (line_height / 2);

            page.begin(out_file);

            buildForm(out_file, payload_height, lines_per_page);
        }

        in_file.getline(single_line, MAX_LINE_LEN - 1);

        if (strlen(single_line) == 0) {
            lines_per_page--;
            continue;
        }

        token0 = strtok(single_line, "\t");
        token1 = token0 ? strtok(NULL, "\t") : NULL;

        if (strcmp(token0, "H") == 0) {
            if (token1 && strlen(token1)) {
                double len = _strlen(&token1);
                len = len * 0.75;

                KXSvg::drawText(out_file, 95.0, y, font_size, token1);
                KXSvg::drawLine(out_file, 95.0 - len, y + line_height / 4, 95.0 + len, y + line_height / 4, false);
            }
        }
        else {

            token2 = token1 ? strtok(NULL, "\t") : NULL;
            token3 = token2 ? strtok(NULL, "\t") : NULL;
            token4 = token3 ? strtok(NULL, "\t") : NULL;

            if (token1 && strlen(token1))
                KXSvg::drawText(out_file, 30.0, y, font_size, token1);

            if (token2 && strlen(token2))
                KXSvg::drawText(out_file, 41.0, y, font_size, token2, KXSvg::Left);

            if (token3 && strlen(token3))
                KXSvg::drawText(out_file, 155.0, y, font_size, token3);

            if (token4 && strlen(token4))
                KXSvg::drawText(out_file, 182.0, y, font_size, token4);
        }

        lines_per_page--;
        y += line_height;

        if ((lines_per_page == 0) || (in_file.eof())) {
            page_num++;
            page.end(out_file);
            fclose(out_file);
            out_file = NULL;
        }

    } while (!in_file.eof());

    return 0;
}
