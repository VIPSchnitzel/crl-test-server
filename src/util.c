#include "util.h"
 
void crl_add_default_headers(evhtp_request_t *req)
{
    crl_add_header(req, CRL_KEEP_ALIVE);
    crl_add_header(req,CRL_TOKEN);
    crl_add_header(req, CRL_SERVER_AGENT);
}

void crl_add_header(evhtp_request_t *req, int number)
{
    evhtp_header_t *new_header;
    switch(number){
        case CRL_ALLOW_GET:
            new_header = evhtp_header_new("Allow", "GET", 1, 1);
        break;
        case CRL_ALLOW_POST:
            new_header = evhtp_header_new("Allow", "POST", 1, 1);
        break;
        case CRL_ALLOW_POST_GET:
            new_header = evhtp_header_new("Allow", "GET, POST", 1, 1);
        break;
        case CRL_CONTENT_CSV:
            new_header = evhtp_header_new("Content-Type", "text/csv", 1, 1);
        break;
        case CRL_CONTENT_JSON:
            new_header = evhtp_header_new("Content-Type", "text/json", 1, 1);
        break;
        case CRL_CONTENT_PNG:
            new_header = evhtp_header_new("Content-Type", "image/png", 1, 1);
        break;
        case CRL_NO_CACHE:
            new_header = evhtp_header_new("Cache-Control", "no cache, no store, must revalidate", 1, 1);
        break;
        case CRL_RETRY_AFTER:
            new_header = evhtp_header_new("Retry-After", "1", 1, 1);
        break;
        case CRL_KEEP_ALIVE:
            new_header = evhtp_header_new("Connection", "keep-alive", 1, 1);
        break;
        case CRL_SERVER_AGENT:
            new_header = evhtp_header_new("Server", CRL_SERVER_ID, 1, 1);
        break;
        case CRL_TOKEN:
            new_header = evhtp_header_new("Corelator-Token", "random", 1, 1);
        break;
        default:
            fprintf(stderr, "unknown header number");
            return;
        
    }
    evhtp_headers_add_header(req->headers_out, new_header);

}

void crl_add_chunk_headers(evhtp_request_t *req, size_t start, size_t end)
{
    evhtp_header_t *new_start;
    evhtp_header_t *new_end;
    char start_string[CRL_SIZE_MAX_STRLEN];
    char end_string[CRL_SIZE_MAX_STRLEN];

    snprintf(start_string, CRL_SIZE_MAX_STRLEN, "%zu", start);
    snprintf(end_string, CRL_SIZE_MAX_STRLEN, "%zu", end);

    new_start = evhtp_header_new("Start-Index", start_string, 1, 1);
    new_end = evhtp_header_new("End-Index", end_string, 1, 1);
    evhtp_headers_add_header(req->headers_out, new_start);
    evhtp_headers_add_header(req->headers_out, new_end);

}

void crl_add_dimension_headers(evhtp_request_t *req, size_t width, size_t length, size_t channels)
{
    evhtp_header_t *new_width, *new_length, *new_channels;
    char length_string[CRL_SIZE_MAX_STRLEN];
    char width_string[CRL_SIZE_MAX_STRLEN];
    char channel_string[CRL_SIZE_MAX_STRLEN];    

    snprintf(length_string, CRL_SIZE_MAX_STRLEN, "%zu", length);
    snprintf(width_string, CRL_SIZE_MAX_STRLEN, "%zu", width);
    snprintf(channel_string, CRL_SIZE_MAX_STRLEN, "%zu", channels);

    new_width = evhtp_header_new("height", width_string, 1, 1);
    new_length = evhtp_header_new("length", length_string, 1, 1);
    new_channels = evhtp_header_new("channels", channel_string, 1, 1);
    evhtp_headers_add_header(req->headers_out, new_width);
    evhtp_headers_add_header(req->headers_out, new_length);
    evhtp_headers_add_header(req->headers_out, new_channels);
}

void crl_add_data_to_req_out(evhtp_request_t *req, void *data, size_t data_size, int crl_content_header){
    evhtp_header_t *length_header;

    char buffer_size_string[CRL_SIZE_MAX_STRLEN] = { '\0' };
    if(data != NULL){
        evbuffer_add(req->buffer_out, data, data_size);
        snprintf(buffer_size_string, CRL_SIZE_MAX_STRLEN,"%zu", evbuffer_get_length(req->buffer_out));
    }else{
        snprintf(buffer_size_string, CRL_SIZE_MAX_STRLEN,"%zu", data_size);
    }
    length_header = evhtp_header_new("Content-Length", buffer_size_string, 1, 1);
    evhtp_headers_add_header(req->headers_out, length_header);
    if(crl_content_header > -1 && crl_content_header < CRL_HEADERS_LENGTH){
        crl_add_header(req, crl_content_header);
    }
}

