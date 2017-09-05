#include <utility_func.h>
#include <colors.h>

char const* color_names[NUM_COLORS] = {"Black", "Red", "Orange", "Yellow", "Green", "Blue", "White"};
unsigned colors[NUM_COLORS];

static char const* visual_class[] = {
   "StaticGray",
   "GrayScale",
   "StaticColor",
   "PseudoColor",
   "TrueColor",
   "DirectColor"
};

int colors_fill(Display* disp, int screen_num) {
    int default_depth;
    Visual* default_visual;
    Colormap default_cmap;
    XVisualInfo visual_info;
    XColor exact_def;
    int i=5;

    default_depth = DefaultDepth(disp, screen_num);
    default_visual = DefaultVisual(disp, screen_num);
    default_cmap = DefaultColormap(disp, screen_num);
    DEBUG_PRINT("default_depth=%d\n", default_depth);
    while(!XMatchVisualInfo(disp, screen_num, default_depth, i--, &visual_info))
        ;

    DEBUG_PRINT("Found %s class visual at default depth (i=%d)\n", visual_class[++i], i);

    if (i < 2) {
        DEBUG_PRINT("Non-color display\n");
        return 0;
    }

    if (visual_info.visual != default_visual) {
        DEBUG_PRINT("Found visual that is not default visual\n");
    }

    for (int col=0; col < NUM_COLORS; ++col) {
        DEBUG_PRINT("Allocating %s...\n", color_names[col]);
        if (!XParseColor(disp, default_cmap, color_names[col], &exact_def)) {
            DEBUG_PRINT("Color name %s is not in database. Exiting\n", color_names[col]);
            return -1;
        }
        DEBUG_PRINT("Color %s: R:G:B:::%d:%d:%d\n", color_names[col], exact_def.red,
            exact_def.green, exact_def.blue);
        if (!XAllocColor(disp, default_cmap, &exact_def)) {
        	DEBUG_PRINT("Unable to allocate color\n");
        	return -1;
        }
        DEBUG_PRINT("Allocated color, pixel value is %u\n", exact_def.pixel);
        colors[col] = exact_def.pixel;
    }
    return 0;
}

unsigned get_color(ENUM_COLORS color) {
    if (color >= 0 && color < NUM_COLORS)
        return colors[color];
    return -1;
}
