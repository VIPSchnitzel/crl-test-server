#ifndef CORE_TEST_UTIL_H
#define CORE_TEST_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <event2/buffer.h>
#include <evhtp/evhtp.h>
#include <pthread.h>
#include <math.h>

#define CRL_SIZE_MAX_STRLEN 30
#define CRL_SERVER_ID "the most fancy corelator testserver of all (unix)"

 
enum crl_common_header_location
{
    CRL_CONTENT_JSON,
    CRL_KEEP_ALIVE,
    CRL_NO_CACHE,
    CRL_RETRY_AFTER,
    CRL_CONTENT_PNG,
    CRL_SERVER_AGENT,
    CRL_ALLOW_POST,
    CRL_ALLOW_POST_GET,
    CRL_ALLOW_GET,
    CRL_CONTENT_CSV,
    CRL_TOKEN,
    CRL_HEADERS_LENGTH /*<- must always remain last entry*/
};

enum crl_test_modes
{
    AREA_MONO,
    AREA_TRI,
    FC_MONO,
    FC_TRI
};

void crl_add_default_headers(evhtp_request_t *req);
void crl_add_header(evhtp_request_t *req, int number);
void crl_add_chunk_headers(evhtp_request_t *req, size_t start, size_t end);
void crl_add_dimension_headers(evhtp_request_t *req, size_t width, size_t length, size_t channels);
void crl_add_data_to_req_out(evhtp_request_t *req, void *data, size_t data_size, int crl_content_header);

#endif // CORE_TEST_UTIL_H
