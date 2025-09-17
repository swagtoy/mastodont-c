/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_FETCH_H
#define MASTODONT_FETCH_H
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include "mastodont_types.h"

/** Used to store the response from CURL */
struct mstdnt_fetch_data
{
    char* response;
    size_t size;

    // Callback from user
    mstdnt_request_cb_t callback;
    void* callback_args;
    int (*json_cb)(cJSON*, void*, mstdnt_request_cb_data*);
    void* json_args;
};

struct mstdnt_fd
{
    int fd;
    short events;
    short revents;
};

/* Note: Don't quite rely on these values being equal to curl's! */
enum mstdnt_fds_events
{
    MSTDNT_POLLIN = CURL_WAIT_POLLIN,
    MSTDNT_POLLPRI = CURL_WAIT_POLLPRI,
    MSTDNT_POLLOUT = CURL_WAIT_POLLOUT,
};

enum mstdnt_fetch_await
{
    MSTDNT_AWAIT_ALL,
    MSTDNT_AWAIT_ONCE,
};

size_t mstdnt_curl_write_callback(char* ptr, size_t _size, size_t nmemb, void* _content);
void mstdnt_fetch_data_cleanup(struct mstdnt_fetch_data* res);

/**
 * @brief Attempts to fetch, async or blocking.
 *
 * @param mstdnt Mastodont struct
 * @param curl curl API
 * @param args Mastodont General args passed
 * @param url URL of request
 * @param results Results from response
 * @return Number of connections in progress, or -1 on error
 */
int mstdnt_fetch_curl_async(mastodont_t* mstdnt,
                            CURL* curl,
                            struct mstdnt_args* args,
                            mstdnt_request_cb_t cb_request,
                            void* cb_args,
                            int (*json_cb)(cJSON*, void*, mstdnt_request_cb_data*),
                            void* json_args,
                            char* url,
                            CURLoption request_t,
                            char* request_t_custom);

/**
 * \brief Blocks until a transfer is complete.
 *
 * You can also check curl's socket, but mastodont offers ways to do that
 *  in case of change
 *
 * \param mstdnt Mastodont struct
 * \param opt Option, MSTDNT_AWAIT_ALL means it will keep going until
 *  there are no more transfers. MSTDNT_AWAIT_ONCE will run
 * \param extra_fds Set of file descriptors to poll alongside
 *  the mastodont data.
 * \param nfds Length of extra_fds
 * \return 1 on error
 */
int mstdnt_await(mastodont_t* mstdnt,
                 enum mstdnt_fetch_await opt,
                 struct mstdnt_fd extra_fds[],
                 size_t nfds);
            

/**
 * Returns a array of file descriptors to network sockets (if any)
 *
 * These file descriptors only notify when ready to read.
 *
 * \remark If you need the write-ready sockets available, poke at CURLM*
 directly
 * \remark Usually required if you want to interface Mastodont with
 *         application event loops (i.e. GTK, QT, EFL)
 *
 * \param  read   Reference ptr to read set
 * \param  write  Reference ptr to write set
 * \param  error  Reference ptr to error set
 * \param  nfds   Number of file descriptors to check for reading.
 * \return 0 on success, 1 on error
 */
int
mstdnt_get_fds(mastodont_t* mstdnt,
               fd_set* read,
               fd_set* write,
               fd_set* error,
               int* nfds);


/**
 * Dispatch any and all queue'd up requests. (Typically set with
 *  mstdnt_request_opts->flags | MSTDNT_REQ_FLAG_QUEUE_REQUEST)
 *
 * \remark Queueing requests won't improve performance, as typically
 *          when you call `mstdnt_await` it will slurp up all requests anyway.
 *
 * \param  flags  Request flags to decide how the requests will be dispatched.
 * \return Number of currently running requests.
 */
int mstdnt_dispatch_requests(mastodont_t *mstdnt, int flags);

void mstdnt_init_socket_callbacks(mastodont_t *mstdnt);

void mstdnt_request_cb_cleanup(mstdnt_request_cb_data* data);

#endif /* MASTODONT_FETCH_H */
