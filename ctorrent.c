#include <stdio.h>
#include "bencoding.h"
#include <curl/curl.h>

int main(int argc, char *argv[])
{
    FILE* stream = fopen(argv[1], "r");
    bencoding_t* metainfo = read_bencoding(stream);

    bencoding_t* ann = find_in_dict(metainfo, "announce");
    print_bencoding(ann);
    return 0;
}
