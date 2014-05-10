
#include "stdafx.h"

#include "tstring.h"

#include "ttime.h"

#include "talloc.h"

static const char * tHexIndex = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz"
                                "?!01-[]()*; @$%^&-_+={}:/.";
#define tHexLen 78

int TStringAdjustSize(char **text,size_t oldsize,size_t newsize)
{
	if(TRAlloc((void **) &text, newsize)) return 1;

	if(newsize > oldsize) memset(text+oldsize,'\0',newsize);

	return 0;
}

size_t TStringRCSpn(const char *_str,const char *_control)
{
	size_t len = 0;
	
	//sanity check
	if(!_str) return 0;

	len = strlen(_str);

	if(!_control) return len+1;

	{
		size_t i = len;
		const char *c = 0;

		for(c = _str+len; c != _str; c--) {
			if(strchr(_control,*c)) return i;
			i--;
		}

		if(strchr(_control,*c)) return 0;
	}

	return len+1;
}

inline void TStringReplaceOp(char *target,const char *match, const char *replacement,size_t repllen)
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

inline void TStringCopyReplaceOp(char *target,const char *source,const char *match, const char *replacement)
{
	const char *srcptr = source;
	char *tarptr = target;
	size_t srclen = strlen(source);
	size_t matchlen = strlen(match);
	size_t repllen = strlen(replacement);
	size_t i = 0;

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

size_t TStringNumOccurences(const char *target,const char *match)
{
	const char *curptr = target;
	size_t matchlen = strlen(match);
	size_t counter = 0;

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
char *TStringReplace(const char *source, const char *match, const char *replacement, size_t hint_numoccurence)
{
	char *result = 0;
	int bufferPosition = 0;
	size_t srclen = 0;
	size_t matchlen = 0;
	size_t repllen = 0;
	size_t reslen = 0;

	if(source == 0 || match == 0 || replacement == 0 || source[0] == '\0' || match[0] == '\0' || replacement[0] == '\0')
		return 0;
	
	srclen = strlen(source);
	matchlen = strlen(match);
	repllen = strlen(replacement);
	reslen = srclen + 1;

	if(matchlen != repllen) {
		if(hint_numoccurence == 0) {
			//find number of occurences
			hint_numoccurence = TStringNumOccurences(source,match);
		}

		reslen -= (int)(matchlen - repllen)*(int)hint_numoccurence;
	}
	result = (char *) TAlloc(reslen);
	if(result) TStringCopyReplaceOp(result,source,match,replacement);
	return result;
}

unsigned char TStringReplaceInplace(char *source, const char *match, const char *replacement) {
	size_t repllen = 0;

	if(source == 0 || match == 0 || replacement == 0 || source[0] == '\0' || match[0] == '\0' || replacement[0] == '\0')
		return 1;

	repllen = strlen(replacement);

	if(strlen(match) != repllen)
		return 2;

	TStringReplaceOp(source,match,replacement,repllen);

	return 0;
}

void TSTringSafetyString(char *string)
{
	int strlength = strlen(string);
	TStringReplaceOp(string,"%"," ",strlength);
}

char **TSTringSplit(const char *string, const char *substr, size_t *size)
{
	char **sto;
	char *str;
	size_t len;
	if(!size || !string || !substr || !*substr) return 0;

	str = strdup(string);
	len = strlen(substr);
	*size = 1;

	while (str = strstr(str,substr)) {
		*str = 0;

		str+=len;
		*size += 1;
	}

	sto = (char **)TAlloc(sizeof(char *) * (*size));
	if(sto) {
		size_t i = 1;
		char *ptr;
		sto[0] = str;

		while (i < *size) {
			ptr = strchr(sto[i-1],0)+len;
			sto[i++] = ptr;
		}
	} else free(str);
	return sto;
}

char *TSTringAddCharacter(const char *string, char character, size_t start, size_t end)
{
	size_t len = strlen(string) + (character ? 1 : 0) + 1;
	int nextindex = character ? start+1 : start;
	char *newstring = 0;

	if(start != end) len -= end-start;

	newstring = (char *) TAlloc(sizeof(char) * len);
	if(newstring) {
		if(start != 0) _snprintf(newstring,start,string);
		newstring[start] = character;
		_snprintf(newstring+nextindex,len-start,string+end);
	}
	
	return newstring;
}

void TSTringStripTrailingWhitespace(char *string)
{
	int i = strlen(string)-1;
	for(; i >= 0; --i)
		if(string[i] == ' ' || string[i] == '\n' || string[i] == '\r')
			string[i] = '\x0';
		else
			break;
}

char *TSTringLowerCase(const char *thestring)
{
	const char *srcptr = thestring;
	char *thecopy = 0, *ptr = 0;

	if(!thestring) return 0;

	thecopy = (char *) TAlloc(strlen(thestring)+ 1);

	if(thecopy) {
		ptr = thecopy;

		while(srcptr != '\0') {
			*ptr = tolower(*srcptr);
			srcptr += 1;
			ptr +=1;
		}
		*ptr = '\0';
	}

	return thecopy;
}

char *TSTringAseEncrypt(const char *src, unsigned char key)
{
	size_t len = 0;
	char *result = 0, *resptr = 0;
	const char *ptr = src;
	unsigned char last_char = 0, tmp = 0;
	
	if(!src) return 0;
	
	len = strlen(src);
	if(!len) return 0;

	result = (char *) TAlloc(sizeof(char)*(len+1));
	if(!result) return 0;

	last_char = key ^ ((int)(TTimeFetchTime()/125)*8 % 255);
	resptr = result;

	for (; *ptr != 0; ptr++, resptr++) {
		tmp = *ptr ^ last_char;
		last_char = *ptr ^ tmp;

		*resptr = tHexIndex[tmp % tHexLen];
	}
	*resptr = 0;

	return result;
}

char *TSTringPasswordEncrypt(const char *src)
{
	size_t len = 0;
	char *result = 0;

	if(!src) return 0;

	len = strlen(src);
	if(!len) return 0;

	result = (char *) TAlloc(sizeof(char) *(len+1));
	if(result) {
		memset(result, '*', len);
		result[len] = 0;
	}

	return result;
}

char *TSTringDoubleChars(const char *string, const char escchar)
{
	char *escstring = 0;

	if(string) {
		const char *esc = 0;
		size_t size = strlen(string)*2;
		char *newstring = (char *) TAlloc(sizeof(char)*size), *cur = 0;
		if(!newstring) return 0;
		cur = newstring;

		do {
			esc = strchr(string, escchar);
			if(esc) {
				size_t escsize = esc-string +1;
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

char *TSTringRemoveDuplication(const char *string, const char escchar)
{
	char *escstring = 0;

	if(string) {
		const char *esc = 0;
		size_t size = strlen(string)+1;
		char doubled[] = {escchar,escchar, 0};
		char *newstring = (char *) TAlloc(sizeof(char)*size), *cur = 0;
		if(!newstring) return 0;
		cur = newstring;

		do {
			esc = strstr(string, doubled);
			if(esc) {
				size_t escsize = esc-string +1;
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

gint TStringCompareGlibFull(gconstpointer a,gconstpointer b,gpointer c)
{
	//TODO: consider c
	return strcmp((const char *) a,(const char *) b);
}
