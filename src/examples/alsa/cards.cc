/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

        The linuxapi package is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2.1 of the License, or (at your option) any later version.

        The linuxapi package is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
        License along with the GNU C Library; if not, write to the Free
        Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
        02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <alsa/asoundlib.h>	// for snd_card_next(3), snd_config_update_free_global(3)
#include <us_helper.h>	// for CHECK_NOT_NEGATIVE()

/*
 * Counts how many sound cards ALSA finds in the system.
 * EXTRA_LINK_FLAGS=-lasound
 */

int main(int argc, char** argv, char** envp) {
	int totalCards=0;
	int cardNum=-1;
	while(true) {
		// Get next sound card's card number. When "cardNum"==-1, then ALSA
		// fetches the first card, otherwise pass the previous card
		CHECK_NOT_NEGATIVE(snd_card_next(&cardNum));
		// No more cards? ALSA sets "cardNum" to -1 if so
		if(cardNum==-1)
			break;
		char* name=NULL;
		CHECK_NOT_NEGATIVE(snd_card_get_name(cardNum, &name));
		printf("[%d],[%s]\n", cardNum, name);
		// Another card found, so bump the count
		totalCards++;
	}
	printf("ALSA found %i cards\n", totalCards);
	// ALSA allocates some mem to load its config file when we call
	// snd_card_next. Now that we're done getting the info, let's tell ALSA
	// to unload the info and free up that mem
	snd_config_update_free_global();
	return EXIT_SUCCESS;
}
