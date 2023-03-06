#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mastodont.h>

#define EFL_BETA_API_SUPPORT 1
 
#include <Efl_Ui.h>

Eo* fd;
mastodont_t mstdnt;

static void
gui_quit_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
	efl_exit(0);
}

static void
_efl_del_cb(void *data, const Efl_Event *ev EINA_UNUSED)
{
   Eina_Bool *dead = data;

   *dead = EINA_TRUE;
}


int
tl_callback(mstdnt_request_cb_data* cb_data, void* args)
{
	struct mstdnt_statuses* statuses = MSTDNT_CB_DATA(cb_data);
	
	for (int i = 0; i < statuses->len; ++i)
	{
		struct mstdnt_status* status = statuses->statuses + i;
	}

	return MSTDNT_REQUEST_DONE;
}

static void
update_mstdnt_fds(void)
{
	int nfds;
	// Get                        
	fd_set set;
	
	mstdnt_get_fds(&mstdnt, &set, &nfds);
	
	for (int i = 0; i < nfds; ++i)
	{
		fd = efl_add(EFL_LOOP_FD_CLASS,
                     efl_main_loop_get(),
                     efl_loop_fd_set(efl_added, set.fd_array[i]),
                     efl_event_callback_add(efl_added, EFL_LOOP_FD_EVENT_READ, _eo_read_cb, &set));
	}
}
 
static void
gui_setup()
{
	Eo *win, *box;
	
	win = efl_add(EFL_UI_WIN_CLASS,
	              efl_main_loop_get(),
	              efl_ui_win_type_set(efl_added, EFL_UI_WIN_TYPE_BASIC),
	              efl_text_set(efl_added, "Hello World"),
	              efl_ui_win_autodel_set(efl_added, EINA_TRUE));

	efl_event_callback_add(win, 
	                       EFL_UI_WIN_EVENT_DELETE_REQUEST,
	                       gui_quit_cb,
	                       NULL);
	
 
	box = efl_add(EFL_UI_BOX_CLASS,
	              win,
	              efl_content_set(win, efl_added),
	              efl_gfx_hint_size_min_set(efl_added, EINA_SIZE2D(360, 240)));

#if 0
	efl_add(EFL_UI_TEXTBOX_CLASS,
	        box,
	        efl_text_markup_set(efl_added, "Super Treebird"),
	        //efl_text_interactive_selection_allowed_set(efl_added, EINA_FALSE),
	        //efl_gfx_hint_weight_set(efl_added, 1.0, 0.9),
	        //efl_gfx_hint_align_set(efl_added, 0.5, 0.5),
	        efl_pack(box, efl_added));
#endif 
 
	efl_add(EFL_UI_BUTTON_CLASS,
	        box,
	        efl_text_set(efl_added, "Fetch posts asynchronously"),
	        efl_gfx_hint_weight_set(efl_added, 1.0, 0.5),
	        efl_pack(box, efl_added),
	        efl_event_callback_add(efl_added, EFL_INPUT_EVENT_CLICKED,
                                   gui_quit_cb, efl_added));
}
 
EAPI_MAIN void
efl_main(void *edata EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   
	mstdnt_global_curl_init();
	
	
	if (mstdnt_init(&mstdnt) != 0)
	{
		fputs("Couldn't initialize Mastodont-c!", stderr);
		exit(1);
	}
	
	gui_setup();

#if 0
	struct mstdnt_args m_args = {
		.url = instance,
		.token = NULL,
		.flags = 0,
	};
	
	mstdnt_timeline_public(&mstdnt, &m_args, tl_callback, NULL, (struct mstdnt_timeline_args){.limit=20});
	
	mstdnt_await(&data, 0, NULL, 0);

	// Cleanup	
	//free(instance);
	//mstdnt_cleanup(&data);
	//mstdnt_global_curl_cleanup();
#endif	
}
EFL_MAIN()

