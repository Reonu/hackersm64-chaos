#include <ultra64.h>

#include "sm64.h"
#include "make_const_nonconst.h"

ALIGNED8 const u8 nuke_textures_dma[] = {
#include "actors/amp/nuke0.rgba16.inc.c"
    ,
#include "actors/amp/nuke1.rgba16.inc.c"
    ,
#include "actors/amp/nuke2.rgba16.inc.c"
    ,
#include "actors/amp/nuke3.rgba16.inc.c"
    ,
#include "actors/amp/nuke4.rgba16.inc.c"
    ,
#include "actors/amp/nuke5.rgba16.inc.c"
    ,
#include "actors/amp/nuke6.rgba16.inc.c"
    ,
#include "actors/amp/nuke7.rgba16.inc.c"
    ,
#include "actors/amp/nuke8.rgba16.inc.c"
    ,
#include "actors/amp/nuke9.rgba16.inc.c"
    ,
#include "actors/amp/nuke10.rgba16.inc.c"
    ,
#include "actors/amp/nuke11.rgba16.inc.c"
    ,
#include "actors/amp/nuke12.rgba16.inc.c"
    ,
#include "actors/amp/nuke13.rgba16.inc.c"
    ,
#include "actors/amp/nuke14.rgba16.inc.c"
    ,
#include "actors/amp/nuke15.rgba16.inc.c"
    ,
#include "actors/amp/nuke16.rgba16.inc.c"
    ,
#include "actors/amp/nuke17.rgba16.inc.c"
    ,
#include "actors/amp/nuke18.rgba16.inc.c"
    ,
#include "actors/amp/nuke19.rgba16.inc.c"
    ,
#include "actors/amp/nuke20.rgba16.inc.c"
    ,
#include "actors/amp/nuke21.rgba16.inc.c"
    ,
#include "actors/amp/nuke22.rgba16.inc.c"
    ,
#include "actors/amp/nuke23.rgba16.inc.c"
    ,
#include "actors/amp/nuke24.rgba16.inc.c"
    ,
#include "actors/amp/nuke25.rgba16.inc.c"
    ,
#include "actors/amp/nuke26.rgba16.inc.c"
    ,
#include "actors/amp/nuke27.rgba16.inc.c"
    ,
#include "actors/amp/nuke28.rgba16.inc.c"
    ,
#include "actors/amp/nuke29.rgba16.inc.c"
    ,
#include "actors/amp/nuke30.rgba16.inc.c"
    ,
#include "actors/amp/nuke31.rgba16.inc.c"
    ,
#include "actors/amp/nuke32.rgba16.inc.c"
    ,
#include "actors/amp/nuke33.rgba16.inc.c"
    ,
#include "actors/amp/nuke34.rgba16.inc.c"
    ,
#include "actors/amp/nuke35.rgba16.inc.c"
    ,
#include "actors/amp/nuke36.rgba16.inc.c"
    ,
#include "actors/amp/nuke37.rgba16.inc.c"
    ,
#include "actors/amp/nuke38.rgba16.inc.c"
    ,
#include "actors/amp/nuke39.rgba16.inc.c"
    ,
#include "actors/amp/nuke40.rgba16.inc.c"
    ,
#include "actors/amp/nuke41.rgba16.inc.c"
    ,
#include "actors/amp/nuke42.rgba16.inc.c"
    ,
#include "actors/amp/nuke43.rgba16.inc.c"
    ,
#include "actors/amp/nuke44.rgba16.inc.c"
    ,
#include "actors/amp/nuke45.rgba16.inc.c"
    ,
#include "actors/amp/nuke46.rgba16.inc.c"
    ,
#include "actors/amp/nuke47.rgba16.inc.c"
    ,
#include "actors/amp/nuke48.rgba16.inc.c"
    ,
#include "actors/amp/nuke49.rgba16.inc.c"
    ,
#include "actors/amp/nuke50.rgba16.inc.c"
    ,
#include "actors/amp/nuke51.rgba16.inc.c"
    ,
#include "actors/amp/nuke52.rgba16.inc.c"
    ,
#include "actors/amp/nuke53.rgba16.inc.c"
    ,
#include "actors/amp/nuke54.rgba16.inc.c"
    ,
#include "actors/amp/nuke55.rgba16.inc.c"
    ,
#include "actors/amp/nuke56.rgba16.inc.c"
    ,
#include "actors/amp/nuke57.rgba16.inc.c"
    ,
#include "actors/amp/nuke58.rgba16.inc.c"
    ,
#include "actors/amp/nuke59.rgba16.inc.c"
    ,
#include "actors/amp/nuke60.rgba16.inc.c"
    ,
#include "actors/amp/nuke61.rgba16.inc.c"
    ,
#include "actors/amp/nuke62.rgba16.inc.c"
    ,
#include "actors/amp/nuke63.rgba16.inc.c"
    ,
#include "actors/amp/nuke64.rgba16.inc.c"
    ,
#include "actors/amp/nuke65.rgba16.inc.c"
    ,
#include "actors/amp/nuke66.rgba16.inc.c"
    ,
#include "actors/amp/nuke67.rgba16.inc.c"
    ,
#include "actors/amp/nuke68.rgba16.inc.c"
    ,
#include "actors/amp/nuke69.rgba16.inc.c"
    ,
#include "actors/amp/nuke70.rgba16.inc.c"
    ,
#include "actors/amp/nuke71.rgba16.inc.c"
    ,
#include "actors/amp/nuke72.rgba16.inc.c"
    ,
#include "actors/amp/nuke73.rgba16.inc.c"
    ,
#include "actors/amp/nuke74.rgba16.inc.c"
    ,
#include "actors/amp/nuke75.rgba16.inc.c"
    ,
#include "actors/amp/nuke76.rgba16.inc.c"
    ,
#include "actors/amp/nuke77.rgba16.inc.c"
    ,
#include "actors/amp/nuke78.rgba16.inc.c"
    ,
#include "actors/amp/nuke79.rgba16.inc.c"
    ,
#include "actors/amp/nuke80.rgba16.inc.c"
    ,
#include "actors/amp/nuke81.rgba16.inc.c"
    ,
#include "actors/amp/nuke82.rgba16.inc.c"
    ,
#include "actors/amp/nuke83.rgba16.inc.c"
    ,
#include "actors/amp/nuke84.rgba16.inc.c"
    ,
#include "actors/amp/nuke85.rgba16.inc.c"
    ,
#include "actors/amp/nuke86.rgba16.inc.c"
    ,
#include "actors/amp/nuke87.rgba16.inc.c"
    ,
#include "actors/amp/nuke88.rgba16.inc.c"
    ,
#include "actors/amp/nuke89.rgba16.inc.c"
    ,
#include "actors/amp/nuke90.rgba16.inc.c"
    ,
#include "actors/amp/nuke91.rgba16.inc.c"
    ,
#include "actors/amp/nuke92.rgba16.inc.c"
    ,
#include "actors/amp/nuke93.rgba16.inc.c"
    ,
#include "actors/amp/nuke94.rgba16.inc.c"
    ,
#include "actors/amp/nuke95.rgba16.inc.c"
    ,
#include "actors/amp/nuke96.rgba16.inc.c"
    ,
#include "actors/amp/nuke97.rgba16.inc.c"
    ,
#include "actors/amp/nuke98.rgba16.inc.c"
    ,
#include "actors/amp/nuke99.rgba16.inc.c"
    ,
#include "actors/amp/nuke100.rgba16.inc.c"
    ,
#include "actors/amp/nuke101.rgba16.inc.c"
    ,
#include "actors/amp/nuke102.rgba16.inc.c"
    ,
#include "actors/amp/nuke103.rgba16.inc.c"
    ,
#include "actors/amp/nuke104.rgba16.inc.c"
    ,
#include "actors/amp/nuke105.rgba16.inc.c"
    ,
#include "actors/amp/nuke106.rgba16.inc.c"
    ,
#include "actors/amp/nuke107.rgba16.inc.c"
    ,
#include "actors/amp/nuke108.rgba16.inc.c"
    ,
#include "actors/amp/nuke109.rgba16.inc.c"
    ,
#include "actors/amp/nuke110.rgba16.inc.c"
};