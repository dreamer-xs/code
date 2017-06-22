#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#define COL_COUNT 27
#define MAX 512

typedef struct __pg_data_t {
            char code[MAX];
            char release_flag[MAX];
            char streaming_bit_rate[MAX];
            char abstract[MAX];
            char duration[MAX];
            char vod_tvod_flag[MAX];
            char created_timestamp[MAX];
            char id[MAX];
            char updated_timestamp[MAX];
            char storage_file_location[MAX];
            char title[MAX];
            char sequence[MAX];
            char note[MAX];
            char filmtype[MAX];
            char filesize[MAX];
            char file_source_URI[MAX];
            char encode_type[MAX];
            char description[MAX];
            char cs_url[MAX];
            char publish_flag[MAX];
            char HD_SD_flag[MAX];
            char streaming_coded[MAX];
            char language[MAX];
            char storage_name[MAX];
            char checksum[MAX];
            char short_title[MAX];
            char program_code[MAX];
}pg_data_t;

/* Create a bunch of objects as demonstration. */
void create_objects(pg_data_t* pg_data)
{
	cJSON *root,*fmt,*img,*thm,*fld;char *out;int i;	/* declare a few. */
	
    const char* pg_key[100] = {"code", "release_flag", "streaming_bit_rate", "abstract", "duration", "vod_tvod_flag", "created_timestamp", "id", "updated_timestamp", "storage_file_location", "title", "sequence", "note", "filmtype", "filesize", "file_source_URI", "encode_type", "description", "cs_url", "publish_flag", "HD_SD_flag", "streaming_coded", "language", "storage_name", "checksum", "short_title", "program_code"};

    /*
        printf("%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s\n",
                pg_data->code,
                pg_data->release_flag,
                pg_data->streaming_bit_rate,
                pg_data->abstract,
                pg_data->duration,
                pg_data->vod_tvod_flag,
                pg_data->created_timestamp,
                pg_data->id,
                pg_data->updated_timestamp,
                pg_data->storage_file_location,
                pg_data->title,
                pg_data->sequence,
                pg_data->note,
                pg_data->filmtype,
                pg_data->filesize,
                pg_data->file_source_URI,
                pg_data->encode_type,
                pg_data->description,
                pg_data->cs_url,
                pg_data->publish_flag,
                pg_data->HD_SD_flag,
                pg_data->streaming_coded,
                pg_data->language,
                pg_data->storage_name,
                pg_data->checksum,
                pg_data->short_title,
                pg_data->program_code
                );
    */

	root=cJSON_CreateObject();	

    i = 0;
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->code);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->release_flag);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->streaming_bit_rate);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->abstract);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->duration);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->vod_tvod_flag);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->created_timestamp);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->id);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->updated_timestamp);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->storage_file_location);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->title);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->sequence);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->note);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->filmtype);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->filesize);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->file_source_URI);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->encode_type);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->description);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->cs_url);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->publish_flag);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->HD_SD_flag);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->streaming_coded);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->language);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->storage_name);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->checksum);
    cJSON_AddStringToObject(root,pg_key[i++], pg_data->short_title);
    cJSON_AddStringToObject(root,pg_key[i],   pg_data->program_code);

	/* Print to text, Delete the cJSON, print it, release the string. */

	out=cJSON_Print(root);
    cJSON_Delete(root);
    //printf("%s\n",out);
    int len =strlen(out);

    for(i=0; i<len; i++)
    {
        if('\n' == out[i])
            continue;
        //if('"' == out[i] && ' ' == out[i+1] & '"' == out[i+2] )
        //   continue;
        printf("%c",out[i]);
    }
    printf("\n");
    free(out);
}

int main (int argc, const char * argv[])
{

    FILE* fd = NULL;
    char buf[MAX*2];

    if(2 != argc)
    {
        printf("Usage: %s input\n",argv[0]);
        return -1;
    }

    fd = fopen(argv[1], "r");

    if(fd < 0)
    {
        printf("Failed to open %s\n",argv[1]);
        return -1;
    }

    pg_data_t pg_data;
    memset(&pg_data, 0, sizeof(pg_data));

    while (fgets(buf,MAX*2,fd))
    {

        sscanf(buf,"%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s",
                pg_data.code,
                pg_data.release_flag,
                pg_data.streaming_bit_rate,
                pg_data.abstract,
                pg_data.duration,
                pg_data.vod_tvod_flag,
                pg_data.created_timestamp,
                pg_data.id,
                pg_data.updated_timestamp,
                pg_data.storage_file_location,
                pg_data.title,
                pg_data.sequence,
                pg_data.note,
                pg_data.filmtype,
                pg_data.filesize,
                pg_data.file_source_URI,
                pg_data.encode_type,
                pg_data.description,
                pg_data.cs_url,
                pg_data.publish_flag,
                pg_data.HD_SD_flag,
                pg_data.streaming_coded,
                pg_data.language,
                pg_data.storage_name,
                pg_data.checksum,
                pg_data.short_title,
                pg_data.program_code
                );
        /*
        printf("===========================================================\n");
        printf("%s\n",buf);
        printf("%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s\n",
                pg_data.code,
                pg_data.release_flag,
                pg_data.streaming_bit_rate,
                pg_data.abstract,
                pg_data.duration,
                pg_data.vod_tvod_flag,
                pg_data.created_timestamp,
                pg_data.id,
                pg_data.updated_timestamp,
                pg_data.storage_file_location,
                pg_data.title,
                pg_data.sequence,
                pg_data.note,
                pg_data.filmtype,
                pg_data.filesize,
                pg_data.file_source_URI,
                pg_data.encode_type,
                pg_data.description,
                pg_data.cs_url,
                pg_data.publish_flag,
                pg_data.HD_SD_flag,
                pg_data.streaming_coded,
                pg_data.language,
                pg_data.storage_name,
                pg_data.checksum,
                pg_data.short_title,
                pg_data.program_code
                );
        */

        create_objects(&pg_data);
        memset(buf, 0, sizeof(buf));
    }
	
    fclose(fd);
	return 0;
}
