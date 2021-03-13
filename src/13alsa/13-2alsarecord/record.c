#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
	  
int dump_pcm(int prefix, short *buf, int sz)
{
    printf(" %d\n", prefix);
    for (int i = 0; i < sz; ++i){
        printf("%04X ", buf[i]); 
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}
int main (int argc, char *argv[])
{
	int i;
	int err;
	short buf[128];
	int rate=22050;
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;
    char *dev_name = "default"; 
    if (argc == 2) {
        dev_name = argv[1];
    }

	if ((err = snd_pcm_open (&capture_handle, dev_name, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		fprintf (stderr, "cannot open audio device %s (%s)\n", dev_name, snd_strerror (err));
		exit (1);
	}
	   
	fprintf (stderr, "open audio device %s (%s)\n", dev_name, snd_strerror (err));
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	fprintf (stderr, "allocate hardware parameter structure (%s)\n", snd_strerror (err));
			 
	if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	fprintf (stderr, "initialize hardware parameter structure (%s)\n", snd_strerror (err));

	if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf (stderr, "cannot set access type (%s)\n", snd_strerror (err));
		exit (1);
	}
	fprintf (stderr, "set access type (%s)\n", snd_strerror (err));

	if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n", snd_strerror (err));
		exit (1);
	}
	fprintf (stderr, "set sample format (%s)\n", snd_strerror (err));

	if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n", snd_strerror (err));
		exit (1);
	}
	fprintf (stderr, "set sample rate (%s)\n", snd_strerror (err));

	if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0) {
		fprintf (stderr, "cannot set channel count (%s)\n", snd_strerror (err));
		exit (1);
	}
	fprintf (stderr, "set channel count (%s)\n", snd_strerror (err));

	if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n", snd_strerror (err));
		exit (1);
	}
	fprintf (stderr, "set parameters (%s)\n", snd_strerror (err));

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (capture_handle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror (err));
		exit (1);
	}
	fprintf (stderr, "prepare audio interface for use (%s)\n", snd_strerror (err));

	for (i = 0; i < 1497; ++i) {
		err = snd_pcm_readi (capture_handle, buf, 128);
		if (err != 128) {
			fprintf (stderr, "read from audio interface failed (%s)\n", snd_strerror (err));
			exit (1);
		}
        else{
            dump_pcm(i, buf, sizeof(buf) / sizeof(buf[0]));
        }
	}

	snd_pcm_close (capture_handle);
	exit (0);
}
