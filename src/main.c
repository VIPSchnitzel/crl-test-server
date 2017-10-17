
#include "main.h"
#include "util.h"

#define ADDRESS "0.0.0.0"
#define PORT 50042
#define BACKLOG 2048
#define FC_TRI_URI "/crl/v1/test/fctri"
#define AREA_TRI_URI "/crl/v1/test/areatri"
#define FC_MONO_URI "/crl/v1/test/fcmono"
#define AREA_MONO_URI "/crl/v1/test/areamono"
#define CRL_MAX_SCAN_BUFFER 3000


evbase_t *evbase;
evhtp_t *htp;
size_t crl_width_dim, crl_spatial_dim;
size_t mode_area_tri;
size_t mode_area_mono;
size_t mode_fc_tri;
size_t mode_fc_mono;
size_t start_index_arr[4] = {0, 0, 0, 0};
size_t chunk_index_arr[4] = {0, 0, 0, 0};

void setup_htp_event(void);
void crl_get_img_test_cb (evhtp_request_t* req, void* arg);
uint8_t crl_generate_colour(size_t mode, size_t channel_it, size_t spat_it);


int main ()
{
    mode_area_mono = 0;
    mode_area_tri = 1;
    mode_fc_mono = 2;
    mode_fc_tri = 3;
    crl_width_dim = 50;
    crl_spatial_dim = 640;
    
    setup_htp_event();

    event_base_loop(evbase, 0);

    evhtp_unbind_socket(htp);
    evhtp_free(htp);
    event_base_free(evbase);

    return 0;
}


void setup_htp_event()
{
    evbase = event_base_new();
    htp = evhtp_new(evbase, NULL);
    evhtp_set_cb(htp, FC_TRI_URI, crl_get_img_test_cb, &mode_fc_tri);
    evhtp_set_cb(htp, AREA_TRI_URI, crl_get_img_test_cb, &mode_area_tri);
    evhtp_set_cb(htp, FC_MONO_URI, crl_get_img_test_cb, &mode_fc_mono);
    evhtp_set_cb(htp, AREA_MONO_URI, crl_get_img_test_cb, &mode_area_mono);

    evhtp_bind_socket(htp, ADDRESS, PORT, BACKLOG);

}

void crl_get_img_test_cb (evhtp_request_t* req, void* arg)
{
    uint8_t *pixels_in;
    size_t spat_it, width_it, channel_it;
    size_t selected_mode = *(size_t *) arg;
    size_t scan_diff = crl_width_dim;
    size_t selected_channels;
    if(selected_mode == 2 || selected_mode == 3){ 
        selected_channels = 3;
    }else{
        selected_channels = 1;
    }
    size_t img_pixel_size = crl_spatial_dim * scan_diff * selected_channels;

    crl_add_default_headers(req);

    switch(req->method){
        case htp_method_GET:
            pixels_in = malloc(img_pixel_size);
            for(spat_it = 0; spat_it < crl_spatial_dim; spat_it ++){
                for(width_it = 0; width_it < crl_width_dim; width_it ++){
                    for(channel_it = 0; channel_it < selected_channels; channel_it ++){
                        pixels_in[spat_it * crl_width_dim * selected_channels + width_it * selected_channels + channel_it] = crl_generate_colour(selected_mode, channel_it, spat_it); 
                    }
                }
            }
            
            crl_add_data_to_req_out(req, pixels_in, img_pixel_size, -1);
            start_index_arr[selected_mode] = (start_index_arr[selected_mode] % CRL_MAX_SCAN_BUFFER);
            crl_add_dimension_headers(req, crl_spatial_dim, CRL_MAX_SCAN_BUFFER, selected_channels);
            crl_add_chunk_headers(req, start_index_arr[selected_mode], (start_index_arr[selected_mode] + scan_diff) % CRL_MAX_SCAN_BUFFER);
            start_index_arr[selected_mode] += scan_diff;
            chunk_index_arr[selected_mode] += scan_diff;
            evhtp_send_reply(req, EVHTP_RES_OK);
            
            free(pixels_in);
            break;
        default:
            crl_add_header(req, CRL_ALLOW_GET);
            evhtp_send_reply(req, EVHTP_RES_METHNALLOWED);
            break;
    }
}

uint8_t crl_generate_colour(size_t mode, size_t channel_it, size_t spat_it)
{
    size_t selected_colour;
    switch(mode){
        case AREA_MONO:
            return 255;
            break;
        case AREA_TRI:
            if(spat_it < crl_width_dim / 3){
                return 255;
            }
            if(spat_it < 2 * (crl_width_dim / 3)){
                return 122;
            }
            return 30;
        break;
        case FC_MONO:
            if(channel_it == 0){
                return 200;
            }
            return 0;
            break;
        case FC_TRI:
            if(spat_it < crl_width_dim / 3){
                selected_colour = 0;
            }
            if(spat_it < 2 * (crl_width_dim / 3)){
                selected_colour = 1;
            }
            selected_colour = 2;
            if(channel_it == selected_colour){
                return 200;
            }
            return 0;
        break;
        
    }
    return 0;
}
