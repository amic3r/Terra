
#include "stdafx.h"

#include "structure/SList.h"

#include "string.h"

#include "ttime.h"

static const char * hex_index = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz"
                                "?!01-[]()*; @$%^&-_+={}:/.";
#define hex_len 78

unsigned int strrcspn(const char *_str,const char *_control)
{
	unsigned int len = 0;
	
	//sanity check
	if(!_str) return 0;

	len = strlen(_str);

	if(!_control) return len+1;

	{
		unsigned i = len;
		const char *c = 0;

		for(c = _str+len; c != _str; c--) {
			if(strchr(_control,*c)) return i;
			i--;
		}

		if(strchr(_control,*c)) return 0;
	}

	return len+1;
}

_inline void string_replace_op(char *target,const char *match, const char *replacement,unsigned int repllen)
{
	char *curptr = target;

	do {
		curptr = strstr(curptr,match);
		if(curptr) {
			strncpy(curptr,replacement,repllen);
			curptr += repllen;
		}
	} while(curptr);
}

_inline void string_copy_replace_op(char *target,const char *source,const char *match, const char *replacement)
{
	const char *srcptr = source;
	char *tarptr = target;
	unsigned int srclen = strlen(source);
	unsigned int matchlen = strlen(match);
	unsigned int repllen = strlen(replacement);
	unsigned int i = 0;

	while(srcptr) {
		*tarptr = *srcptr;
		if(*srcptr == match[0] && i + matchlen <= srclen) {
			if(strncmp(srcptr,match,matchlen) == 0) {
				strncpy(tarptr,replacement,repllen);
				srcptr += matchlen-1;
				tarptr += repllen-1;
			}
		}
		srcptr = *(srcptr+1) == '\0' ? 0 : srcptr+1;
		tarptr +=1;
	}
	*tarptr = '\0';
}

unsigned int string_num_occurences(const char *target,const char *match)
{
	const char *curptr = target;
	unsigned int matchlen = strlen(match);
	unsigned int counter = 0;

	do {
		curptr = strstr(curptr,match);
		if(curptr) {
			counter +=1;
			curptr += matchlen;
		}
	} while(curptr);

	return counter;
}

/*
 * Replace a substring of a string with another string
 */
char *string_replace(const char *source, const char *match, const char *replacement, unsigned int hint_numoccurence)
{
	char *result = 0;
	int bufferPosition = 0;
	unsigned int srclen = 0;
	unsigned int matchlen = 0;
	unsigned int repllen = 0;
	unsigned int reslen = 0;

	if(source == 0 || match == 0 || replacement == 0 || source[0] == '\0' || match[0] == '\0' || replacement[0] == '\0')
		return 0;
	
	srclen = strlen(source);
	matchlen = strlen(match);
	repllen = strlen(replacement);
	reslen = srclen + 1;

	if(matchlen != repllen) {
		if(hint_numoccurence == 0) {
			//find number of occurences
			hint_numoccurence = string_num_occurences(source,match);
		}

		reslen -= (int)(matchlen - repllen)*(int)hint_numoccurence;
	}
	result = (char *) malloc(reslen);

	string_copy_replace_op(result,source,match,replacement);
	return result;
}

unsigned char string_replace_inplace(char *source, const char *match, const char *replacement) {
	unsigned int repllen = 0;

	if(source == 0 || match == 0 || replacement == 0 || source[0] == '\0' || match[0] == '\0' || replacement[0] == '\0')
		return 1;

	repllen = strlen(replacement);

	if(strlen(match) != repllen)
		return 2;

	string_replace_op(source,match,replacement,repllen);

	return 0;
}

void SafetyString(char *string)
{
	int strlength = strlen(string);
	string_replace_op(string,"%"," ",strlength);
}

/*char *string_split(const char *source, const char *substr, unsigned int amount, unsigned int end, unsigned int *size)
{
	//TODO fix this
	char *result = 0, *ptr = 0;
	if(!source || !substr || !*substr) return 0;

	if(end) {
		result = (char *) malloc(sizeof(char)*(end+1));
		strncpy(result,source,end);
		result[end] = '\0';
	} else {
		result = strdup(source);
	}
	*size = 1;
	ptr = strstr(result,substr);

	while (ptr && amount) {
		*ptr = 0;
		*size +=1;
		ptr = strstr(ptr+1,substr);
		amount--;
	}

	return result;
}*/

int string_split(SList *sto, const char *string, const char *substr)
{
	char *str;
	size_t len;
	if(!sto || !string || !substr || !*substr) return 1;

	str = strdup(string);
	len = strlen(substr);
	slist_append(sto,str);

	while (str = strstr(str,substr)) {
		*str = 0;

		str+=len;
		slist_append(sto,str);
	}

	return 0;
}

char *add_character_to_string(const char *string, char character, unsigned int start, unsigned int end)
{
	unsigned int len = strlen(string) + (character ? 1 : 0) + 1;
	int nextindex = character ? start+1 : start;
	char *newstring = 0;

	if(start != end) len -= end-start;

	newstring = (char *) malloc(sizeof(char) * len);

	if(start != 0) _snprintf(newstring,start,string);
	newstring[start] = character;
	_snprintf(newstring+nextindex,len-start,string+end);
	
	return newstring;
}

void StripTrailingWhitespace(char *string)
{
	int i = strlen(string)-1;
	for(; i >= 0; --i)
		if(string[i] == ' ' || string[i] == '\n' || string[i] == '\r')
			string[i] = '\x0';
		else
			break;
}

char *LowerCaseString(const char *thestring)
{
	const char *srcptr = thestring;
	char *thecopy = 0, *ptr = 0;

	if(!thestring) return 0;

	thecopy = (char *) malloc(strlen(thestring)+ 1);
	ptr = thecopy;

	while(srcptr != '\0') {
		*ptr = tolower(*srcptr);
		srcptr += 1;
		ptr +=1;
	}
	*ptr = '\0';

	return thecopy;
}

char *string_ase_encrypt(const char *src, unsigned char key)
{
	unsigned int len = 0;
	char *result = 0, *resptr = 0;
	const char *ptr = src;
	unsigned char last_char = 0, tmp = 0;
	
	if(!src) return 0;
	
	len = strlen(src);
	if(!len) return 0;

	result = (char *) malloc(sizeof(char)*(len+1));
	if(!result)
		AppAbort("Memory Allocation Failed");

	last_char = key ^ ((int)(fetchHighResTime()/125)*8 % 255);
	resptr = result;

	for (; *ptr != 0; ptr++, resptr++) {
		tmp = *ptr ^ last_char;
		last_char = *ptr ^ tmp;

		*resptr = hex_index[tmp % hex_len];
	}
	*resptr = 0;

	return result;
}

char *string_password_encrypt(const char *src)
{
	unsigned int len = 0;
	char *result = 0;

	if(!src) return 0;

	len = strlen(src);
	if(!len) return 0;

	result = (char *) malloc(sizeof(char) *(len+1));
	memset(result, '*', len);
	result[len] = 0;

	return result;
}

char *double_char_in_string(const char *string, const char escchar)
{
	char *escstring = 0;

	if(string) {
		const char *esc = 0;
		unsigned int size = strlen(string)*2;
		char *newstring = (char *) malloc(sizeof(char)*size), *cur = 0;
		if(!newstring) return 0;
		cur = newstring;

		do {
			esc = strchr(string, escchar);
			if(esc) {
				unsigned int escsize = esc-string +1;
				_snprintf(cur,escsize,string); cur += escsize;
				_snprintf(cur,1,"%c",escchar); cur += 1;

				string = esc+1;
			} else {
				_snprintf(cur,size,string);
			}
		} while(esc);

		escstring = strdup(newstring);
		free(newstring);
	}

	return escstring;
}

char *remove_character_duplication(const char *string, const char escchar)
{
	char *escstring = 0;

	if(string) {
		const char *esc = 0;
		unsigned int size = strlen(string)+1;
		char *newstring = (char *) malloc(sizeof(char)*size), *cur = 0;
		char doubled[] = {escchar,escchar, 0};
		if(!newstring) return 0;
		cur = newstring;

		do {
			esc = strstr(string, doubled);
			if(esc) {
				unsigned int escsize = esc-string +1;
				_snprintf(cur,escsize,string); cur += escsize;

				string = esc+2;
			} else {
				_snprintf(cur,size,string);
			}
		} while(esc);

		escstring = strdup(newstring);
		free(newstring);
	}

	return escstring;
}

gint string_compare_glib_full(gconstpointer a,gconstpointer b,gpointer c)
{
	//TODO: consider c
	return strcmp((const char *) a,(const char *) b);
}
