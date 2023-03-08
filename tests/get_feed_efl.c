#define _POSIX_C_SOURCE 200112L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mastodont.h>

#include <signal.h>
#define EFL_BETA_API_SUPPORT 1
 
#include <Efl_Ui.h>

void update_mstdnt_fds(void);

Eo* textbox_instance;
Eo* fd;
Eo* posts;
mastodont_t mstdnt;

static void
gui_quit_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
	mstdnt_cleanup(&mstdnt);
	mstdnt_global_curl_cleanup();
	efl_exit(0);
}

static Eo*
gui_create_status(struct mstdnt_status* status)
{
	Eo* root = efl_add(EFL_UI_BOX_CLASS,
	                   efl_name_set(efl_added, "Status"));
	
	Eo* avatar = efl_add();
	
	return root;
}

static void
gui_add_status(struct mstdnt_status* status)
{
	efl_pack_end(gui_create_status(status));
}

int
tl_callback(mstdnt_request_cb_data* cb_data, void* args)
{
	struct mstdnt_statuses* statuses = MSTDNT_CB_DATA(cb_data);
	
	for (int i = 0; i < statuses->len; ++i)
	{
		struct mstdnt_status* status = statuses->statuses + i;
		gui_add_status(status);
	}

	return MSTDNT_REQUEST_DONE;
}

static void
gui_fetch_posts(void* data EINA_UNUSED, const Efl_Event* event EINA_UNUSED)
{
	printf("Got %s...\n", efl_text_get(textbox_instance));
	
	struct mstdnt_args m_args = {
		.url = efl_text_get(textbox_instance),
		.token = NULL,
		.flags = 0,
	};
	
	mstdnt_timeline_public(&mstdnt, &m_args, tl_callback, NULL,
	                       (struct mstdnt_timeline_args){.limit=20});
	
	update_mstdnt_fds();
}

static void
mstdnt_results_cb(void* data EINA_UNUSED, const Efl_Event* event EINA_UNUSED)
{
	printf("Mastodont data ready to be read!\n");
	mstdnt_await(&mstdnt, 0, NULL, 0);
}

static void
mstdnt_update_cb(void* data EINA_UNUSED, const Efl_Event* event EINA_UNUSED)
{
	int running;
	curl_multi_perform(mstdnt.curl, &running);
}

void
update_mstdnt_fds()
{
	int nfds;
	// Get                        
	fd_set read;
	fd_set write;
	fd_set error;
	FD_ZERO(&read);
	FD_ZERO(&write);
	FD_ZERO(&error);
	
	mstdnt_get_fds(&mstdnt, &read, &write, &error, &nfds);
	
	for (int i = 0; i <= nfds; ++i)
	{
		printf("FD_ISSET: %i\n", i);
		if (FD_ISSET(i, &read))
		{
			efl_add(EFL_LOOP_FD_CLASS,
						 efl_main_loop_get(),
						 efl_loop_fd_set(efl_added, i),
						 efl_event_callback_add(efl_added, EFL_LOOP_FD_EVENT_READ, mstdnt_results_cb, &i));
		}
#if 0
		if (FD_ISSET(i, &write))
		{
			efl_add(EFL_LOOP_FD_CLASS,
						 efl_main_loop_get(),
						 efl_loop_fd_set(efl_added, i),
						 efl_event_callback_add(efl_added, EFL_LOOP_FD_EVENT_WRITE, mstdnt_update_cb, &i));
		}
#endif
	}
}

static void
gui_setup(void)
{
	Eo *win, *box;
	
	win = efl_add(EFL_UI_WIN_CLASS,
	              efl_main_loop_get(),
	              efl_ui_win_type_set(efl_added, EFL_UI_WIN_TYPE_BASIC),
	              efl_text_set(efl_added, "mastodont-c test"),
	              efl_ui_win_autodel_set(efl_added, EINA_TRUE),
	              efl_event_callback_add(efl_added, 
	                                     EFL_UI_WIN_EVENT_DELETE_REQUEST,
	                                     gui_quit_cb,
	                                     NULL));
	
	box = efl_add(EFL_UI_BOX_CLASS,
	              win,
	              efl_content_set(win, efl_added),
	              efl_gfx_hint_size_min_set(efl_added, EINA_SIZE2D(360, 440)));
	              
	posts = efl_add(EFL_UI_LIST_CLASS,
	                box,
	                //efl_text_interactive_selection_allowed_set(efl_added, EINA_FALSE),
	                //efl_gfx_hint_weight_set(efl_added, 1.0, 0.9),
	                //efl_gfx_hint_align_set(efl_added, 0.5, 0.5),
	                efl_pack(box, efl_added));

	textbox_instance = efl_add(EFL_UI_TEXTBOX_CLASS,
	                           box,
	                           efl_gfx_hint_weight_set(efl_added, 1.0, 0.1),
	                           efl_pack(box, efl_added));
		
	efl_add(EFL_UI_BUTTON_CLASS,
	        box,
	        efl_text_set(efl_added, "Fetch posts asynchronously"),
	        efl_gfx_hint_weight_set(efl_added, 1.0, 0.2),
	        efl_pack(box, efl_added),
	        efl_event_callback_add(efl_added, EFL_INPUT_EVENT_CLICKED,
                                   gui_fetch_posts, NULL));
}
 
EAPI_MAIN void
efl_main(void *edata EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
	// Initialize Mastodont library
	mstdnt_global_curl_init();
	
	if (mstdnt_init(&mstdnt) != 0)
	{
		fputs("Couldn't initialize Mastodont-c!", stderr);
		exit(1);
	}
	
	gui_setup();
}
EFL_MAIN()

