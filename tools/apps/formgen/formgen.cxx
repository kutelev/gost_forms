#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <getopt.h>
#include <iostream>

#include <kxgostformlib.h>

#define MAX_LINE_LEN 1024

static Page* page = nullptr;
static Form* form = nullptr;
static std::ifstream in_file;
static FILE* out_file = nullptr;

static char* out_dir = (char*)".";
static char* file_name = nullptr;
static unsigned int num_pages = 1;

static int parseArgs(int argc, char** argv)
{
    int c;

    struct option long_options[] = {
        { "input", required_argument, nullptr, 0 },
        { "pages", required_argument, nullptr, 0 },
        { "dir", required_argument, nullptr, 0 },
        { nullptr, 0, nullptr, 0 },
    };

    while (true) {
        int option_index = 0;

        c = getopt_long(argc, argv, "i:p:d:", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        file_name = optarg;
                        break;
                    case 1:
                        char* end_ptr;
                        num_pages = strtol(optarg, &end_ptr, 10);
                        break;
                    case 2:
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
            case 'p':
                char* end_ptr;
                num_pages = strtol(optarg, &end_ptr, 10);
                break;
            case 'd':
                out_dir = optarg;
                break;
            case '?':
                fprintf(stderr, "Invalid short argument\n");
                return -1;
            default:
                fprintf(stderr, "getopt_long returned unexpected code\n");
                return -1;
        }
    }

    return 0;
}

static void printUsage()
{
    fprintf(stderr, "===============================\n");
    fprintf(stderr, "Usage: formgen [options]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "-i | --input    Input file name\n");
    fprintf(stderr, "-p | --pages    Number of pages [1]\n");
    fprintf(stderr, "-d | --dir      Output directory [.]\n");
    fprintf(stderr, "===============================\n");
}

void bye()
{
    delete form;
    delete page;
    if (in_file.is_open())
        in_file.close();
    if (out_file != nullptr)
        fclose(out_file);
}

int main(int argc, char** argv)
{
    char single_line[MAX_LINE_LEN];

    char* token0;
    char* token1;
    char* token2;
    char* end_ptr;

    unsigned int field_num, sub_field_num;

    atexit(bye);

    parseArgs(argc, argv);

    if (file_name == nullptr) {
        fprintf(stderr, "===============================\n");
        fprintf(stderr, "File name required!\n");
        printUsage();
        return -1;
    }

    if (num_pages > 100U) {
        fprintf(stderr, "===============================\n");
        fprintf(stderr, "To many pages (>100)!\n");
        printUsage();
        return -1;
    }

    in_file.open(file_name, std::ios_base::in);
    if (!in_file.is_open()) {
        fprintf(stderr, "Could not open file: \"%s\"\n", file_name);
        return -1;
    }

    in_file.getline(single_line, MAX_LINE_LEN - 1);
    if (strlen(single_line) > 0) {

        token0 = strtok(single_line, "\t");
        token1 = strtok(nullptr, "\t");

        if ((token0 == nullptr) || (token1 == nullptr)) {
            fprintf(stderr, "Invalid file format");
            return -1;
        }

        if (!strcmp("A0", token1))
            page = new Page(Page::A0, false);
        else if (!strcmp("A0L", token1))
            page = new Page(Page::A0, true);
        else if (!strcmp("A1", token1))
            page = new Page(Page::A1, false);
        else if (!strcmp("A1L", token1))
            page = new Page(Page::A1, true);
        else if (!strcmp("A2", token1))
            page = new Page(Page::A2, false);
        else if (!strcmp("A2L", token1))
            page = new Page(Page::A2, true);
        else if (!strcmp("A3", token1))
            page = new Page(Page::A3, false);
        else if (!strcmp("A3L", token1))
            page = new Page(Page::A3, true);
        else if (!strcmp("A4", token1))
            page = new Page(Page::A4, false);
        else {
            fprintf(stderr, "Unknown page format: \"%s\"\n", token1);
            return -1;
        }

        if (!strcmp("Drawing", token0))
            form = new FormDrawing;
        else if (!strcmp("Text", token0))
            form = new FormText;
        else if (!strcmp("TextPlus", token0))
            form = new FormTextPlus;
        else if (!strcmp("ProgramText", token0))
            form = new ProgramText;
        else if (!strcmp("ProgramTextPlus", token0))
            form = new ProgramTextPlus;
        else {
            fprintf(stderr, "Unknown form type: \"%s\"\n", token0);
            return -1;
        }
    }
    else {
        fprintf(stderr, "File \"%s\" is empty!\n", file_name);
        return -1;
    }

    do {
        in_file.getline(single_line, MAX_LINE_LEN - 1);

        if (strlen(single_line) == 0)
            continue;

        token0 = strtok(single_line, "\t");
        token1 = token0 ? strtok(nullptr, "\t") : nullptr;
        token2 = token1 ? strtok(nullptr, "\t") : nullptr;

        if ((token0 == nullptr) || (token1 == nullptr) || (token2 == nullptr)) {
            fprintf(stderr, "Invalid file format");
            return -1;
        }

        field_num = strtol(token0, &end_ptr, 10);
        sub_field_num = strtol(token1, &end_ptr, 10);

        form->setField(field_num, sub_field_num, token2);

    } while (!in_file.eof());

    char file_name[256], tmp[256];

    unsigned int page_num = 1;

    unsigned int flags = 0;

    for (unsigned int i = 0; i < num_pages; i++) {
        if (i == (num_pages - 1)) {
            page->setSize(Page::A4, false);
            flags = LAST_PAGE;
        }

        sprintf(file_name, "%s/Sheet_%02u.svg", out_dir, page_num);
        out_file = fopen(file_name, "w");

        if (out_file == nullptr) {
            fprintf(stderr, "Could not open file fo writing: \"%s\"\n", file_name);
            return -1;
        }

        page->begin(out_file);

        if (num_pages > 1) {
            sprintf(tmp, "%u", page_num);
            form->setField(7, 0, tmp);
        }
        else
            form->setField(7, 0, "");
        sprintf(tmp, "%u", num_pages);

        form->setField(8, 0, tmp);

        form->draw(out_file, *page, page_num, flags);

        page->end(out_file);

        page_num++;
    }

    return 0;
}
