// Regression checks for proxy-texture mipmap capability calculations.
#include <cstdio>
#include <GL/gl.h>
#include "gl/texture.h"

static int fails = 0;

static void eq(const char* what, bool got, bool want) {
    if (got != want) {
        std::printf("  FAIL %-50s got %d want %d\n", what, got ? 1 : 0, want ? 1 : 0);
        ++fails;
    }
}

int main() {
    std::printf("proxy mipmap capability checks\n");

    // The proxy call for mip level L supplies that level's own dimensions.
    // A level-4 image of 512 texels represents a 8192-wide base extent.
    eq("512 @ level 4 fits max 8192", mg_texture_proxy_level_fits(512, 4, 8192), true);
    eq("512 @ level 5 exceeds max 8192", mg_texture_proxy_level_fits(512, 5, 8192), false);
    eq("256 @ level 3 fits max 2048", mg_texture_proxy_level_fits(256, 3, 2048), true);

    // Large levels must fail cleanly instead of invoking signed-shift UB.
    eq("1 @ level 31 exceeds INT_MAX", mg_texture_proxy_level_fits(1, 31, 2147483647), false);
    eq("negative level rejected", mg_texture_proxy_level_fits(64, -1, 4096), false);
    eq("zero extent rejected", mg_texture_proxy_level_fits(0, 0, 4096), false);

    std::printf("\n%s (%d failures)\n", fails ? "FAILED" : "all checks passed", fails);
    return fails != 0;
}
