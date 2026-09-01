/*============================================================================
Copyright (c) 2018-2025 Raspberry Pi
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
============================================================================*/

#include <locale.h>
#include <glib/gi18n.h>

#include "plugin.h"

#include "batt.h"

/*----------------------------------------------------------------------------*/
/* LXPanel plugin functions                                                   */
/*----------------------------------------------------------------------------*/

/* Constructor */
static GtkWidget *ptbatt_constructor (LXPanel *panel, config_setting_t *settings)
{
    /* Allocate and initialize plugin context */
    PtBattPlugin *pt = g_new0 (PtBattPlugin, 1);

    /* Allocate top level widget and set into plugin widget pointer. */
    pt->panel = panel;
    pt->settings = settings;
    pt->plugin = gtk_event_box_new ();
    lxpanel_plugin_set_data (pt->plugin, pt, batt_destructor);

    /* Read config */
    batt_set_values (pt);
    lxplug_read_settings (pt->settings, conf_table);

    batt_init (pt);

    return pt->plugin;
}

/* Handler for system config changed message from panel */
static void ptbatt_configuration_changed (LXPanel *, GtkWidget *plugin)
{
    PtBattPlugin *pt = lxpanel_plugin_get_data (plugin);
    batt_update_display (pt);
}

/* Apply changes from config dialog */
static gboolean ptbatt_apply_configuration (gpointer user_data)
{
    PtBattPlugin *pt = lxpanel_plugin_get_data (GTK_WIDGET (user_data));
    lxplug_write_settings (pt->settings, conf_table);
    batt_set_num (pt);
    return FALSE;
}

/* Display configuration dialog */
static GtkWidget *ptbatt_configure (LXPanel *panel, GtkWidget *plugin)
{
    return lxpanel_generic_config_dlg_new (_(PLUGIN_TITLE), panel,
        ptbatt_apply_configuration, plugin,
        conf_table);
}

int module_lxpanel_gtk_version = 1;
char module_name[] = PLUGIN_NAME;

/* Plugin descriptor. */
LXPanelPluginInit fm_module_init_lxpanel_gtk = {
    .name = PLUGIN_TITLE,
    .gettext_package = GETTEXT_PACKAGE,
    .description = N_("Monitors laptop battery"),
    .new_instance = ptbatt_constructor,
    .reconfigure = ptbatt_configuration_changed,
    .config = ptbatt_configure
};

/* End of file */
/*----------------------------------------------------------------------------*/
