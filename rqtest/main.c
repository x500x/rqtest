#include "curl/curl.h"
#include <stdio.h>
#include <stdlib.h>

//using namespace std;

typedef void *HANDLE;

struct STACKINFO {
    struct STACKINFO *next;
    struct STACKINFO *last;
    HANDLE Stack;
};

struct STACKINFO *headinfo;
struct STACKINFO *nowinfo;
/*
将数据写入栈
返回值:
成功->0
失败不为0
*/
int PushStack(HANDLE h) {
    if(!nowinfo)
    {
        nowinfo=headinfo=(struct STACKINFO *)malloc(sizeof(struct STACKINFO));
        if(!nowinfo)
        {
            printf("malloc err");
            return 1;
        }
        headinfo->last=NULL;
    }
    else
    {
        nowinfo->next=(struct STACKINFO *)malloc(sizeof(struct STACKINFO));
        if(!(nowinfo->next))
        {
            printf("malloc err");
            return 2;
        }
        nowinfo->next->last=nowinfo;
        nowinfo=nowinfo->next;

    }

    nowinfo->Stack=h;
    nowinfo->next=NULL;

    return 0;
}
/*
得到栈顶的内容
返回值:
成功:为栈顶的内容
失败:为NULL
*/
HANDLE PopStack() {
    if(!nowinfo) return NULL;
    struct STACKINFO *temp=nowinfo;

    HANDLE h=nowinfo->Stack;
    if(nowinfo->last) {
        nowinfo=nowinfo->last;
    }
    else
    {
        nowinfo=headinfo=NULL;
    }
    free(temp);
    return h;
}

/*
初始化栈，将以前的栈的内存释放
*/
void InitStack() {
    while(nowinfo) {
        if(nowinfo->last) {
            nowinfo=nowinfo->last;
            free(nowinfo->next);
            continue;
        }
        free(nowinfo);
        nowinfo=headinfo=NULL;
    }
}
/*
判断栈是否为空
返回值:
空->1 非空->0
*/
inline int IsStackEmpty() {
    if(!nowinfo)return 1;
    return 0;
}
/*
int main(int argc, char** argv) {
    headinfo=NULL;
    nowinfo=NULL;

    InitStack();
    PushStack((HANDLE)"123456\n");
    PushStack((HANDLE)"ttt\n");
    //InitStack();
    // printf("\nHANDLEsize:%d\n",sizeof(void *));

    while(!IsStackEmpty()) {
        printf(PopStack());
    }

    
        // printf(PopStack());
        // printf(PopStack());
        
    printf("\nHANDLEsize:%d\n",sizeof(void *));
    return 0;
}
*/
size_t responseBodyCallback(void *ptr, size_t size, size_t nmemb, void *stream) {
    int *pResponse = (int *)stream;
	//{"zt":1
	if(0==*pResponse)
	    if('1'==*(((char *)ptr+6))) *pResponse=1;
	
	return size * nmemb;
}

int main(int argc, char* argv[]){
    headinfo=NULL;
    nowinfo=NULL;

    InitStack();
    
    printf("https://wwkk.lanzouo.com/b00y9smr3a\n密码:6666\n\n");
    CURL *hnd = curl_easy_init();
    //重要!禁用ssl证书检查
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_URL, "https://pc.woozooo.com/html5up.php");
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Linux; Android 13; Pixel 7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Mobile Safari/537.36");
    //headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate");
    headers = curl_slist_append(headers, "content-type: multipart/form-data");
    headers = curl_slist_append(headers, "origin: https://pc.woozooo.com");
    headers = curl_slist_append(headers, "x-requested-with: mark.via");
    headers = curl_slist_append(headers, "sec-fetch-site: same-origin");
    headers = curl_slist_append(headers, "sec-fetch-mode: cors");
    headers = curl_slist_append(headers, "sec-fetch-dest: empty");
    headers = curl_slist_append(headers, "referer: https://pc.woozooo.com/mydisk.php?");
    headers = curl_slist_append(headers, "accept-language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
    
    curl_easy_setopt(hnd, CURLOPT_COOKIE, "PHPSESSID=bbahu9n33i4embo2q7lhad9s8i4p13i1; _uab_collina=173634568362009593157997; ylogin=4315297; uag=7096a941b5b46547583a88c02551760f; phpdisk_info=BTVVZAVmATgBMVA5CmcEV1M3Bg0JYVY5V20JbgYyU2hUYAI4BGAAPQQxVQwJNVA5VGUBOwpgVjRXMQM2UmUHZAVnVTYFMwE5ATZQOAppBGZTZQZgCWVWNlcyCWAGM1NkVGICZgRuADsENlU2CVpQO1Q8AToKZFYzV20DY1JsBzMFMFVg");
    
    int response=0;//0->have not use;1 for success;-1 for failure
    int success_count=0;
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, responseBodyCallback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void*)&response);
    
    curl_mime *mime = curl_mime_init(hnd);
    curl_mimepart *part;
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "task");
    curl_mime_data(part, "1", CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "vie");
    curl_mime_data(part, "2", CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "ve");
    curl_mime_data(part, "2", CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "id");
    curl_mime_data(part, "WU_FILE_0", CURL_ZERO_TERMINATED);
    // part = curl_mime_addpart(mime);
    // curl_mime_name(part, "name");
    // curl_mime_data(part, "769.mp4", CURL_ZERO_TERMINATED);
    // part = curl_mime_addpart(mime);
    // curl_mime_name(part, "type");
    // curl_mime_data(part, "application/octet-stream", CURL_ZERO_TERMINATED);
    // curl_mime_name(part, "size");
    // curl_mime_data(part, "6693945", CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "folder_id_bb_n");
    //curl_mime_data(part, "11275842", CURL_ZERO_TERMINATED);
    curl_mime_data(part, "11278500", CURL_ZERO_TERMINATED);
    
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "upload_file");
    // curl_mime_filename(part, "769.mp4");
    for(int i=1;i<argc;++i){
        curl_mime_filedata(part, argv[i]);
    
        curl_easy_setopt(hnd, CURLOPT_MIMEPOST, mime);
        
        printf("\ni=%d  v=%s\n\n",i,argv[i]);
        
        CURLcode ret = curl_easy_perform(hnd);
        if(1==response) ++success_count;
        if(-1==response) PushStack((HANDLE)i);
        response=0;
        //cout<<endl;
        //printf("\ni=%d;;v=%s\n\n",i,argv[i]);
        printf("\n\n");
    }
    //cout<<"all task finished"<<endl;
    printf("all task finished\n");
    printf("total=%d,\tsuccess=%d\n",argc-1,success_count);
    if(success_count!=argc-1){
        printf("以下为失败的文件\n");
        while(!IsStackEmpty()) {
            printf("%s\n",argv[((int)PopStack())]);
        }
    }
    
    printf("请按任意键继续\n");
    getchar();
}