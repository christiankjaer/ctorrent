#include <stdio.h>
#include <curl/curl.h>
#include "bencoding.h"

int main(int argc, char *argv[])
{
    FILE* stream = fopen(argv[1], "r");
    bencoding_t* metainfo = read_bencoding(stream);

    bencoding_t* ann = find_in_dict(metainfo, "announce");
    char* pieces = NULL;
    int n = get_pieces(metainfo, &pieces);

    printf("Piecelength: %d\n", n);
    for (int i = 0; i < 20; ++i) {
        printf("%%%02X", (int) pieces[i]+128);
    }
    putchar('\n');

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://google.dk");
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
    return 0;
}
