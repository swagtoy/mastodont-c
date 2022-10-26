/*
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <mastodont_static.h>
#include <mastodont_fetch.h>

int mstdnt_instance_panel(mastodont_t* api,
                             struct mstdnt_args* m_args,
mstdnt_request_cb_t cb_request,
void* cb_args,
                             struct mstdnt_fetch_data* html)
{
    CURL* curl = curl_easy_init();
    int status = mstdnt_fetch_curl(api,
                                      curl,
                                      m_args,
                                      "instance/panel.html",
                                      html,
                                      CURLOPT_HTTPGET,
                                      NULL);
    curl_easy_cleanup(curl);
    return status;
}

int mstdnt_terms_of_service(mastodont_t* api,
                               struct mstdnt_args* m_args,
mstdnt_request_cb_t cb_request,
void* cb_args,
                               struct mstdnt_fetch_data* html)
{
    CURL* curl = curl_easy_init();
    int status = mstdnt_fetch_curl(api,
                                      curl,
                                      m_args,
                                      "static/terms-of-service.html",
                                      html,
                                      CURLOPT_HTTPGET,
                                      NULL);
    curl_easy_cleanup(curl);
    return status;
}
