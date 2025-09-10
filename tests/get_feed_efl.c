#define EFL_PACK_LAYOUT_PROTECTED
#define _POSIX_C_SOURCE 200809L
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
Eo* fake_list;
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
	Eo* root = efl_add(EFL_UI_BOX_CLASS, fake_list,
	                   efl_gfx_hint_margin_set(efl_added, 5, 5, 5, 0),
	                   efl_gfx_color_set(efl_part(efl_added, "background"), 84, 84, 84, 150)
	                   );
	
	Eo* header = efl_add(EFL_UI_BOX_CLASS, root,
	                     efl_ui_layout_orientation_set(efl_added, EFL_UI_LAYOUT_ORIENTATION_HORIZONTAL),
	                     efl_gfx_hint_fill_set(efl_added, EINA_FALSE, EINA_FALSE),
	                     efl_gfx_hint_align_set(efl_added, 0.0, 0.0),
	                     efl_pack(root, efl_added));
	                     
	efl_add(EFL_UI_TEXTBOX_CLASS, header,
	        efl_text_set(efl_added, status->account.display_name),
	        efl_gfx_hint_margin_set(efl_added, 0, 5, 0, 0),
	        efl_pack(header, efl_added));
	
	efl_add(EFL_UI_TEXTBOX_CLASS, header,
	        efl_text_set(efl_added, status->account.acct),
	        efl_text_font_weight_set(efl_added, EFL_TEXT_FONT_WEIGHT_BOLD),
	        efl_pack(header, efl_added));
	
	Eo* content = efl_add(EFL_UI_TEXTBOX_CLASS, root,
	                      efl_text_interactive_selection_allowed_set(efl_added, EINA_FALSE),
	                      efl_text_wrap_set(efl_added, EINA_TRUE),
	                      efl_gfx_hint_fill_set(efl_added, EINA_TRUE, EINA_FALSE),
	                      efl_text_markup_set(efl_added, status->content),
	                      efl_text_interactive_editable_set(efl_added, EINA_FALSE),
	                      efl_gfx_color_set(efl_part(efl_added, "background"), 84, 84, 84, 150),
	                      efl_gfx_hint_margin_set(efl_added, 0, 0, 0, 5),
	                      efl_pack(root, efl_added));
	//efl_pack_layout_update(fake_list);
	efl_pack_layout_update(fake_list);
	return root;
}

static void
gui_add_status(struct mstdnt_status* status)
{
	efl_pack_end(fake_list, gui_create_status(status));
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
mstdnt_results_cb(void* _data EINA_UNUSED, const Efl_Event* event EINA_UNUSED)
{
	Eo* data = _data;
	mstdnt_await(&mstdnt, 0, NULL, 0);
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
						 efl_event_callback_add(efl_added, EFL_LOOP_FD_EVENT_READ, mstdnt_results_cb, efl_added));
		}
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
	              efl_gfx_hint_size_min_set(efl_added, EINA_SIZE2D(300, 440)));
	              
	posts = efl_add(EFL_UI_SCROLLER_CLASS,
	                box,
	                //efl_text_interactive_selection_allowed_set(efl_added, EINA_FALSE),
	                //efl_gfx_hint_weight_set(efl_added, 1.0, 0.9),
	                //efl_gfx_hint_align_set(efl_added, 0.5, 0.5),
	                efl_pack(box, efl_added));
	
	fake_list = efl_add(EFL_UI_BOX_CLASS,
	                    posts,
	                    efl_content_set(posts, efl_added));

	textbox_instance = efl_add(EFL_UI_TEXTBOX_CLASS,
	                           box,
	                           efl_gfx_hint_weight_set(efl_added, 1.0, 0.1),
	                           efl_gfx_color_set(efl_added, 0, 255, 0, 255),
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

