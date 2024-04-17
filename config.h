static const char text[]        = "#2D353B";
static const char background[]  = "#232A2E";
//static const char *red        = "^b#E67E80^";
static const char green[]       = "#A7C080";
static const char yellow[]      = "#DBBC7F";
static const char blue[]        = "#7FBBB3";
static const char magenta[]     = "#D699B6";
static const char cyan[]        = "#83C092";

static const Component status[] = {
  /* function          pollrate per 1/4s  (static) icon  background color */
  { snprintf_updates,  1200,              "⬆️ ",          cyan    },
  { snprintf_ram,      80,                "🐏 ",         magenta },
  { snprintf_volume,   1,                 "",            green   },
  { snprintf_date,     240,               "📅 ",         blue    },
  { snprintf_time,     1,                 "",            yellow  },
};
