#include "framebuffer.h"

void runtime_test()
{
    for (int j = 0; j < 25; j++)
        for (int i = 0; i < 80; i++) {
            fb_set_foreground_color(j);
            fb_set_background_color(9 - j);
            fb_set_blink(i+j % 2 == 0);
            fb_write_char('0' + (char)(j % 10));
        }


    fb_write("Hello LittleOS! This is a test.");

    fb_set_foreground_color(FB_WHITE);
    fb_set_background_color(FB_BLACK);

    // Try with this very long string with \n's
    fb_write("Lorem ipsum dolor sit amet, sed munere graeco at. Luptatum appellantur per et, ei mei nihil patrioque. Ipsum audire accusam ea vix. Aperiri deterruisset eu mea. Cu eam nonumes petentium mnesarchum, cu facilisis maluisset ullamcorper sit. Eu audire apeirian eum. Laudem verterem usu at, laoreet adipisci abhorreant cu usu, ex vis inimicus ullamcorper. Volumus placerat erroribus et nec, prima malis accumsan pri in. Illud alienum petentium ad his, vis atqui copiosae dissentiunt cu. Te sit inani ullamcorper. Veritus deserunt ea eam. Eu unum legere corpora cum, errem vocibus id vix. Ea noster scriptorem vis, vix id unum partem. Pri salutandi iudicabit repudiare at. Eos et zril iriure persecuti. Sit tation audiam consequuntur ei. Nam mazim antiopam ea, duo choro aeterno explicari et, at usu oblique virtute neglegentur. Nam natum dicam homero in, et debet ludus laboramus nam, vidisse albucius nominavi cu per. Ad nonumy tacimates dissentiunt sed, usu albucius invidunt in, eum blandit similique ei. Nostro reprimique ius ad, an mei rebum viris viderer. Mea ut dicat graece, eu his elitr platonem. Purto quodsi at vel. Has modo esse dolorum ad, sea vivendo dolores omittantur ea. Vel harum commodo ullamcorper et, fuisset principes nam at, cum harum volutpat id. Everti fastidii corrumpit eum et. Meliore antiopam ut quo. An mea inani assentior, omittam apeirian usu ex. Modo albucius eleifend eos at, esse accusata gloriatur est ne. Vix reque pericula eu, viris ocurreret vel et, autem indoctum\n\nquone.");
}
