#include <stdio.h>
#include "curl/curl.h"
#include <vector>
using namespace std;


size_t write_data(void* buffer, size_t size, size_t nmemb, void* stream) {
	
	//Write the buffer passed from the handle to a file, and return the amount written
	size_t  written = fwrite(buffer, size, nmemb, (FILE*)stream);
	return written;
}


void print_version_info() {
	//Get our version info and print it out on runtime!
	curl_version_info_data* curl_version_data = curl_version_info(CURLVERSION_NOW);
	printf("Current Curl Version: %s", curl_version_data->version);
}


void build_handle_list(vector<CURL*> *handle_list, CURL* *base_handle, int n) {

	for (int i = 0; i < n; i++) {
		handle_list->push_back(curl_easy_duphandle(*base_handle));
	}

	printf("handles meant to be built: %d\nhandles built: %d", n, handle_list->size());
}

int main()
{
	CURLcode success;
	vector<CURL*> handle_list;
	auto base_filename = "api";


	/*Lets intialize the curl library, this function needs to be called once and only once.*/
	curl_global_init(CURL_GLOBAL_ALL);
	print_version_info();


	//Creates an easy curl handle, that easy_handle will point to
	CURL *init_handle = curl_easy_init();


	//Setting up our handles options
	curl_easy_setopt(init_handle, CURLOPT_URL, "https://randomuser.me/api/");

	curl_easy_setopt(init_handle, CURLOPT_WRITEFUNCTION, write_data);
\
	//Create our output file and if file is valid edit options
	FILE* outputfile;
	fopen_s(&outputfile,"api_response_all.json", "wb");
	if (outputfile) {
		curl_easy_setopt(init_handle, CURLOPT_WRITEDATA, outputfile);

		build_handle_list(&handle_list, &init_handle, 10);
	}



	for (CURL *handle  : handle_list) {
		curl_easy_perform(handle);
		curl_easy_cleanup(handle);
	}
	 


	fclose(outputfile);
	curl_global_cleanup();
}

//https://randomuser.me/api/