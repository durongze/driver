#include<alsa/asoundlib.h>

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

int main(int argc, char** argv)
{
    int stop;
    int err;
    char *device_in = NULL;
    char *device_out = NULL;
    int fd_in = -1,fd_out = -1;
    snd_rawmidi_t *handle_in = 0,*handle_out = 0;
    if (device_in) {
    	err = snd_rawmidi_open(&handle_in,NULL,device_in,0);	//打开MIDI设备
    	if (err) {
    		fprintf(stderr,"snd_rawmidi_open %s failed: %d\n",device_in,err);
    	}
    }
    if (device_out) {
    	err = snd_rawmidi_open(NULL,&handle_out,device_out,0);
    	if (err) {
    		fprintf(stderr,"snd_rawmidi_open %s failed: %d\n",device_out,err);
    	}
    }
    if ((handle_in) && (handle_out))
    {
    	while (!stop) 
    	{
    		unsigned char ch;
    		snd_rawmidi_read(handle_in,&ch,1);//读MIDI设备
    		snd_rawmidi_write(handle_out,&ch,1);//写MIDI设备
    		snd_rawmidi_drain(handle_out);
    	}
    }
    if (handle_in)
    {
    	snd_rawmidi_drain(handle_in);
    	snd_rawmidi_close(handle_in); //关闭midi输入设备
    }
    if (handle_out) 
    {
    	snd_rawmidi_drain(handle_out); 
    	snd_rawmidi_close(handle_out);//关闭midi输出设备
    }
    return 0;
}
