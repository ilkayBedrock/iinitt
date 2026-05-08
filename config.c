#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

struct Service {
	char           *starting_command;
	int 		wait_after_start;
};

/*
 * THIS TOKENIZER CODE COMING FROM EASY. LICENSED UNDER GPL-3
 */
int
tokenizer(char *bufin, char *bufout[], int max_count)
{
	int 		token_count = 0;
	const char     *p = bufin;
	while (*p != '\0') {
		if (*p == '#') {
			break;
		} else if (*p == ',') {
			char           *sub = malloc(2);
			sub[0] = ',';
			sub[1] = '\0';
			bufout[token_count++] = sub;
			p++;
		} else if (*p == ':') {
			char           *sub = malloc(2);
			sub[0] = ':';
			sub[1] = '\0';
			bufout[token_count++] = sub;
			p++;
		} else if (isspace(*p)) {
			p++;
			continue;
		} else if (*p == '\0') {
			break;
		} else if (*p == '"') {
			char 		in_string = *p;
			const char     *string_start = ++p;
			while (*p && *p != in_string)
				p++;
			int 		stringlen = p - string_start;
			char           *in_string_tokens = malloc(stringlen + 1);
			strncpy(in_string_tokens, string_start, stringlen);
			in_string_tokens[stringlen] = '\0';
			bufout[token_count++] = in_string_tokens;
			if (*p)
				p++;
		} else {
			const char     *word_start = p;
			while (*p && !isspace(*p) && *p != ',' && *p != ':')
				p++;
			int 		word_len = p - word_start;
			char           *word = malloc(word_len + 1);
			strncpy(word, word_start, word_len);
			word[word_len] = '\0';
			bufout[token_count++] = word;
		}
		if (token_count >= max_count)
			break;
	}
	bufout[token_count] = NULL;
	return token_count;
}

void 
parse_config_line(char *line)
{
	char           *bufout[10];
	int 		tokenize = tokenizer(line, bufout, 10);
	for (int i = 0; i < tokenize; i++) {
		if (strcmp(bufout[i], "run")) {
			char           *service_name = bufout[++i];
			i++;
			char           *service_command = bufout[++i];
	//TODO:	Start service here.
		} else if (strcmp(bufout[i], "enable_disable")) {
	//TODO:	enable_disable services here
		}
	}
}

int 
main()
{
	char           *my_tokens = "run NetworkManager, /usr/sbin/NetworkManager";
	char           *bufout[10];
	int 		tokenize = tokenizer(my_tokens, bufout, 10);
	for (int i = 0; i < tokenize; i++) {
		printf("%s\n", bufout[i]);
	}
	//out:
	// run
	// NetworkManager
	// ,
	// /usr/sbin/NetworkManager
	//
	for (int i = 0; i< tokenize; i++) {
		free(bufout[i]);
	}
	return 0;
}
