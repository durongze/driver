
#include<alsa/asoundlib.h>

int main(void)
{
	int unmute, chn;
	int al, ar, idx = 0;
	snd_mixer_t *mixer;
	snd_mixer_elem_t * elem;
	snd_mixer_open(&mixer, 0);
	snd_mixer_attach(mixer, "default");
	snd_mixer_selem_register(mixer, NULL, NULL);//注册混音器
	snd_mixer_load(mixer);
	for(elem=snd_mixer_first_elem(mixer); elem; elem=snd_mixer_elem_next(elem)) {
		/* 设定音量的范围 0 ~ 100 */
		snd_mixer_selem_set_playback_volume_range(elem, 0, 100);
		/* 取得左右声道的音量 */
		snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &al);
		snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, &ar);
		/*平均音量 */
		printf("al[%d] volume is %d\n", ++idx, al);
		printf("ar[%d] volume is %d\n", idx, ar);
		/*设定左右声道的音量 */
		snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, 49);
		snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, 49);
    }
    return 0;
}
