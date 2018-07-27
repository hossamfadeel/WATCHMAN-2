#include "cmd_parser.h"
#include "reg_read.h"
#include "reg_write.h"
#include <stdlib.h>

void command_parser(struct pbuf *p, int* regmap){
	char* payload = p->payload;
	int cmd_buf_size = 0;
	const char delimiter[2] = "/";

	//Tokenize the string using delimiter
	char* token = strtok(payload, delimiter);

	command_buffer[cmd_buf_size] = token;
	cmd_buf_size++;
	int reg_addr, reg_val;
	while (token != NULL && cmd_buf_size < 1000) {

		token = strtok(NULL, delimiter);
		command_buffer[cmd_buf_size] = token;
		cmd_buf_size++;
	}
	int index_send_buffer = 2;
	char reg_val_string[100];

	command_buffer[cmd_buf_size] = NULL;
	//Goes through the command buffer and interprets each command
	for(int i = 2; i < cmd_buf_size-2; i++){

		printf("this: %s\n", command_buffer[i]);
		if(strcmp(command_buffer[i],"ping") == 0){
			strncpy(return_buffer[index_send_buffer], "pong", 4);
			printf("return_buffer for ping: %s\n", return_buffer[index_send_buffer]);
			index_send_buffer++;
		}
		else if(strcmp(command_buffer[i],"read") == 0){
			strncpy(return_buffer[index_send_buffer], "read", 4);
			strncpy(return_buffer[index_send_buffer+1], command_buffer[i+1], 100);
			reg_addr=strtol(command_buffer[i+1],NULL,10);
			itoa(reg_read(reg_addr, regmap), reg_val_string, 10);
			strncpy(return_buffer[index_send_buffer+2], reg_val_string, 4);
			printf("return_buffer for read:\ncmd-%s \naddr:%s \nval:%s \n",
					return_buffer[index_send_buffer], return_buffer[index_send_buffer+1] , return_buffer[index_send_buffer+2]);
			index_send_buffer += 3;
			i++;
		}
		else if(strcmp(command_buffer[i],"rite") == 0){

			reg_addr=strtol(command_buffer[i+1],NULL,10);
			reg_val=strtol(command_buffer[i+2],NULL,10);
			reg_write(reg_addr, reg_val, regmap);
			i+=2;
		}
		else{
			printf("%s is not a valid command\n", command_buffer[i]);
		}

	}

}
