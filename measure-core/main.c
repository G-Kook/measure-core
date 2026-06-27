#include "gen10.h"
#include "gen11.h"

#define __GEN 11

int main()
{
	if (__GEN == 10) gen10_draw_idvg();
	if (__GEN == 11) gen11_draw_idvg();
	return 0;
}