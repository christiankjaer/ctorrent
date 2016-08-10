#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/sha.h>
#include "bencoding.h"

char* peer_id = "minegentorrentdeadde";

int main(int argc, char *argv[])
{
    FILE* stream = fopen(argv[1], "r");
    bencoding_t* metainfo = read_bencoding(stream);
    fclose(stream);
    char info_hash[] = "12345678901234567890";

    bencoding_t* ann = find_in_dict(metainfo, "announce");

    char url[1024];
    memcpy(url, ann->data.b_string.buffer, ann->data.b_string.size);
    url[ann->data.b_string.size] = '\0';

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    char* hash = curl_easy_escape(curl, info_hash, 20);
    char req_string[1024];
    snprintf(req_string, 1024, "%s?info_hash=%s&peer_id=%s&port=%d&downloaded=0&uploaded=0&left=1234789", url, hash, peer_id, 6881);
    puts(req_string);
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, req_string);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
    destroy_bencoding(metainfo);
    return 0;
}
